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

//自动模式
uint8_t auto_mod;


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
}

void underpan_control(float speed_ref)
{
    PID_Calc(&underpan_motor[0], speed_ref, underpan_para[0].rotation_rate);
	PID_Calc(&underpan_motor[1], -speed_ref, underpan_para[1].rotation_rate);
    Underpan_motor_output(underpan_motor[0].output, underpan_motor[1].output, 0, 0);
}


void rounds_control(float speed_ref)
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
	underpan_control(movement_dir * speed_ref * SGN(track_len));
}

void cloud_control(void)
{
	PID_Calc(&cloud_pitch_speed_pid, 0, mpu6050.Gyro.Origin.y);
}

void motor_control(void)
{
	//底盘、云台
	switch (run_mod)
	{
		case DEBUG:
			underpan_control(tele_data.ch0);
			break;
		case MANUAL:
			underpan_control(tele_data.ch0);
			break;
		case AUTO:
			rounds_control(500);
			break;
		default:
			rounds_control(0);
			break;
	}
	track_position += underpan_para[0].rotation_rate;
	//拨弹、摩擦轮
	if (shoot_switch == 1)
	{
		TIM12->CCR1 = 1600;
		TIM12->CCR2 = 1600;
	}
	else
	{
		TIM12->CCR1 = 800;
		TIM12->CCR2 = 800;
	}
	PID_Calc(&dan_pid, shoot_switch*(-1000), dan.speed);

	Cloud_motor_output(0, 0, dan_pid.output);
}

void para_init(void)
{
	read_para(&my_data);
	track_len = my_data.track_len;
	track_position = 0;
	movement_dir = 1;
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