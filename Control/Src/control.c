/**
  *@file control.c
  *@date 2018-10-7
  *@author Vacuo.SB
  *@brief 
  */

#include "control.h"
#include "mpu6050.h"
#include "pid.h"
#include "tele_control.h"
#include "my_flash.h"
#include "can_my.h"
#include "debug.h"
#include "stm32f4xx_hal.h"
#include "filter.h"
#include "timer.h"

//运行模式
uint8_t run_mod;

//模块开关
uint8_t underpan_switch;
uint8_t cloud_ctrl_switch;
uint8_t shoot_switch;

//轨道边界
uint8_t track_mod;
float track_position;
float track_len;
int8_t movement_dir;
float speed_ref;

//云台
float yaw_position_ref;
float pitch_position_ref = pitch_mid;
//自动模式
uint8_t auto_mod;
int16_t deDanTimer;

//aoto aim
CAMERA camera; 

//右边的开关选择一级模式：轨道调试、手动控制和自动模式
//左边的开关选择二级模式：轨道调试：设置起点、终点和测量轨道长度
//						手动控制：射击开关
//						自动模式：云台自瞄、底盘运动和全自动运行
//不收到数据时为DEFAULT模式，即比赛的运行状态
void switch_control(void)
{
    switch (tele_data.sr)
    {
        case TOP:	//调试模式
            run_mod = DEBUG;
			//从上或下拨到中间档，准备开始测量轨道
			if(tele_data.sl == MIDDLE && (track_mod == SET_START || track_mod == SET_END))
			{
				track_mod = START_MEASURE;
				track_len = 0;
			}
			//从中间拨到两边，停止测量轨道长度
			else if((tele_data.sl == TOP || tele_data.sl == BOTTOM) && track_mod == MEASURE_LEN)
			{
				track_mod = END_MEASURE;
				my_data.track_len = track_len;
				write_para(&my_data);
			}
			//保持中间档，测量轨道长度
			else if(tele_data.sl == MIDDLE)
			{
				track_mod = MEASURE_LEN;
				track_len += underpan_para[0].rotation_rate;			
			}
			//保持最上档，确定运行起点
			else if(tele_data.sl == BOTTOM)
			{
				track_mod = SET_START;
				track_position = 0;
			}
			//保持最下档，确定运行终点
			else if(tele_data.sl == TOP)
			{
				track_mod = SET_END;
				track_position = track_len;
			}
			else
			{
				run_mod = DEFAULT;
			}
            break;
        case MIDDLE:	//手动模式
            run_mod = MANUAL;
			//左边拨到最上档手动射击
			if (tele_data.sl == TOP)
			{
				shoot_switch = 1;
			}
			else
			{
				shoot_switch = 0;
			}
            break;
        case BOTTOM:	//自动模式
            run_mod = AUTO;
			switch (tele_data.sl)
			{
				case MIDDLE:	//全自动
					auto_mod = FULL_AUTO;
					break;
				case TOP:		//仅云台自动运行（扫描+自瞄）
					auto_mod = CLOUD_AUTO;
					break;
				case BOTTOM:	//仅底盘自动（巡航）
					auto_mod = UNDERPAN_AUTO;
					break;
			}
            break;
        default:
            run_mod = DEFAULT;
            break;
    }
	track_position += underpan_para[0].rotation_rate;
}

void underpan_control(void)
{
    PID_Calc(&underpan_motor[0], speed_ref, underpan_para[0].rotation_rate);
	PID_Calc(&underpan_motor[1], -speed_ref, underpan_para[1].rotation_rate);
    Underpan_motor_output(underpan_motor[0].output, underpan_motor[1].output, 0, 0);
}


float direction_control(float speed_ref)
{
	//控制往返方向
	if(ABS(track_position) < ABS(track_len * 0.05))
	{
		movement_dir = 1;	//正向运动，从起点到终点
		LED_R_ON;
	}
	else if(ABS(track_position) > ABS(track_len *0.95))
	{
		movement_dir = -1;	//反向，从终点到起点
		LED_R_ON;
	}
	else
	{
		LED_R_OFF;
	}
	//速度方向 = 运动方向 * 轨道方向
	return movement_dir * speed_ref * SGN(track_len);
}

void manual_cloud(void)
{
	yaw_position_ref -= tele_data.ch2 / 40;
	pitch_position_ref -= tele_data.ch3 / 40;
	if (pitch_position_ref >= pitch_mid + pitch_limit)
	{
		pitch_position_ref = pitch_mid + pitch_limit;
	}
	else if (pitch_position_ref <= pitch_mid - pitch_limit)
	{
		pitch_position_ref = pitch_mid - pitch_limit;
	}
	// if (yaw_position_ref >= yaw_mid + yaw_limit)
	// {
	// 	yaw_position_ref = yaw_mid + yaw_limit;
	// }
	// else if (yaw_position_ref <= yaw_mid - yaw_limit)
	// {
	// 	yaw_position_ref = yaw_mid - yaw_limit;
	// }
}

void cloud_speed_control(void)
{
	dan_control();
	PID_Calc(&cloud_pitch_speed_pid, cloud_pitch_position_pid.output, mpuPitch50HZ.filtered_value);
	PID_Calc(&cloud_yaw_speed_pid, cloud_yaw_position_pid.output, mpuYaw50HZ.filtered_value);
	Cloud_motor_output(cloud_pitch_speed_pid.output, cloud_yaw_speed_pid.output, dan_pid.output);
	// Cloud_motor_output(cloud_pitch_speed_pid.output, 0, dan_pid.output);
}

void cloud_position_control(void)
{
	PID_Calc(&cloud_pitch_position_pid, pitch_position_ref, cloud_pitch.mechanical_angle);
	if(ABS(yaw_position_ref - cloud_yaw.mechanical_angle) > 4096)
	{
		yaw_position_ref = yaw_position_ref - SGN(yaw_position_ref - cloud_yaw.mechanical_angle) * 8191;
	}
	PID_Calc(&cloud_yaw_position_pid, yaw_position_ref, cloud_yaw.mechanical_angle);
}

void dan_control(void)
{
	float dan_ref = -2000;
	//卡弹100ms时，把计数器置负，切换到退弹模式
	if (deDanTimer >= 100)
	{
		deDanTimer = -100;
	}
	//退弹模式
	else if (deDanTimer < 0)
	{
		dan_ref = 2000;
		deDanTimer ++;
	}
	//卡弹检测，计时器正向累加
	else if (shoot_switch == 1 && ABS(dan_pid.output) == dan_pid.outputMax)
	{
		deDanTimer++;
	}
	//正常运行
	else 
	{
		deDanTimer = 0;
	}
	PID_Calc(&dan_pid, shoot_switch*(dan_ref), dan.speed);
}

void motor_control(void)
{
	//底盘
	switch (run_mod)
	{
		case DEBUG:
			speed_ref = tele_data.ch0 * 2;
            auto_cloud();
			break;
		case MANUAL:
			speed_ref = tele_data.ch0 * 2;
			manual_cloud();
			break;
		case AUTO:
			speed_ref = direction_control(1200);
            //auto_cloud();
			break;
		default:
			speed_ref = 0;
			break;
	}
	
	//拨弹、摩擦轮
	if (shoot_switch == 1)
	{
		TIM12->CCR1 = 1900;
		TIM12->CCR2 = 1900;
	}
	else
	{
		TIM12->CCR1 = 800;
		TIM12->CCR2 = 800;
	}
}

void para_init(void)
{
	read_para(&my_data);
	track_len = my_data.track_len;
	track_position = 0;
	movement_dir = 1;
	yaw_position_ref = cloud_yaw.mechanical_angle;
	// yaw_position_ref = yaw_mid;
}


// /************UNDERPAN***************/
// float underpan_P;//ok
// float underpan_I;//ok
// float IOUT_P;
// float IOUT_I;


// /************CLOUD***************/
// int16_t pitch;
// int16_t yaw;


/************DAN***************/



// /************CAMERA***************/
// struct CAMERA camera;

// //裁判系统
// struct JUDGE judge;

// RxPID rxPID;


/****************************************/
/****************function****************/
/****************************************/

void auto_cloud(void)
{
	float angle;
    if ((camera.x == 0 && camera.y == 0) || camera.ready == 0)
	{
		return;
	}
	angle=camera.y;
	if (camera.y < 15)
    {
        angle = atan(camera.y * 0.001798) * 1100;
    }
	else 
    {
        angle = atan(camera.y * 0.001798) * 1304.05;
    }
	
	pitch_position_ref -= 0*angle;
	if (camera.x < 15)
    {
        angle = atan(camera.x * 0.001798) * 1100;
    }
	else 
    {
        angle = atan(camera.x * 0.001798) * 1304.05;
    }
	yaw_position_ref -= angle ;
    if (pitch_position_ref >= pitch_mid + pitch_limit)
	{
		pitch_position_ref = pitch_mid + pitch_limit;
	}
	else if (pitch_position_ref <= pitch_mid - pitch_limit)
	{
		pitch_position_ref = pitch_mid - pitch_limit;
	}
    camera.x = 0;
    camera.y = 0;
}