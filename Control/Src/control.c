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

//����ģʽ
uint8_t run_mod;

//ģ�鿪��
uint8_t underpan_switch;
uint8_t cloud_ctrl_switch;
uint8_t shoot_switch;

//����߽�
uint8_t track_mod;
float track_position;
float track_len;
int8_t movement_dir;
float speed_ref;

//��̨
float yaw_position_ref;
float pitch_position_ref = pitch_mid;
//�Զ�ģʽ
uint8_t auto_mod;
int16_t deDanTimer;

//aoto aim
CAMERA camera; 

//�ұߵĿ���ѡ��һ��ģʽ��������ԡ��ֶ����ƺ��Զ�ģʽ
//��ߵĿ���ѡ�����ģʽ��������ԣ�������㡢�յ�Ͳ����������
//						�ֶ����ƣ��������
//						�Զ�ģʽ����̨���顢�����˶���ȫ�Զ�����
//���յ�����ʱΪDEFAULTģʽ��������������״̬
void switch_control(void)
{
    switch (tele_data.sr)
    {
        case TOP:	//����ģʽ
            run_mod = DEBUG;
			//���ϻ��²����м䵵��׼����ʼ�������
			if(tele_data.sl == MIDDLE && (track_mod == SET_START || track_mod == SET_END))
			{
				track_mod = START_MEASURE;
				track_len = 0;
			}
			//���м䲦�����ߣ�ֹͣ�����������
			else if((tele_data.sl == TOP || tele_data.sl == BOTTOM) && track_mod == MEASURE_LEN)
			{
				track_mod = END_MEASURE;
				my_data.track_len = track_len;
				write_para(&my_data);
			}
			//�����м䵵�������������
			else if(tele_data.sl == MIDDLE)
			{
				track_mod = MEASURE_LEN;
				track_len += underpan_para[0].rotation_rate;			
			}
			//�������ϵ���ȷ���������
			else if(tele_data.sl == BOTTOM)
			{
				track_mod = SET_START;
				track_position = 0;
			}
			//�������µ���ȷ�������յ�
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
        case MIDDLE:	//�ֶ�ģʽ
            run_mod = MANUAL;
			//��߲������ϵ��ֶ����
			if (tele_data.sl == TOP)
			{
				shoot_switch = 1;
			}
			else
			{
				shoot_switch = 0;
			}
            break;
        case BOTTOM:	//�Զ�ģʽ
            run_mod = AUTO;
			switch (tele_data.sl)
			{
				case MIDDLE:	//ȫ�Զ�
					auto_mod = FULL_AUTO;
					break;
				case TOP:		//����̨�Զ����У�ɨ��+���飩
					auto_mod = CLOUD_AUTO;
					break;
				case BOTTOM:	//�������Զ���Ѳ����
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
	//������������
	if(ABS(track_position) < ABS(track_len * 0.05))
	{
		movement_dir = 1;	//�����˶�������㵽�յ�
		LED_R_ON;
	}
	else if(ABS(track_position) > ABS(track_len *0.95))
	{
		movement_dir = -1;	//���򣬴��յ㵽���
		LED_R_ON;
	}
	else
	{
		LED_R_OFF;
	}
	//�ٶȷ��� = �˶����� * �������
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
	//����100msʱ���Ѽ������ø����л����˵�ģʽ
	if (deDanTimer >= 100)
	{
		deDanTimer = -100;
	}
	//�˵�ģʽ
	else if (deDanTimer < 0)
	{
		dan_ref = 2000;
		deDanTimer ++;
	}
	//������⣬��ʱ�������ۼ�
	else if (shoot_switch == 1 && ABS(dan_pid.output) == dan_pid.outputMax)
	{
		deDanTimer++;
	}
	//��������
	else 
	{
		deDanTimer = 0;
	}
	PID_Calc(&dan_pid, shoot_switch*(dan_ref), dan.speed);
}

void motor_control(void)
{
	//����
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
	
	//������Ħ����
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

// //����ϵͳ
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