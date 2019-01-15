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

//�Զ�ģʽ
uint8_t auto_mod;


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
}

void underpan_control(float speed_ref)
{
    PID_Calc(&underpan_motor[0], speed_ref, underpan_para[0].rotation_rate);
	PID_Calc(&underpan_motor[1], -speed_ref, underpan_para[1].rotation_rate);
    Underpan_motor_output(underpan_motor[0].output, underpan_motor[1].output, 0, 0);
}


void rounds_control(float speed_ref)
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
	underpan_control(movement_dir * speed_ref * SGN(track_len));
}

void cloud_control(void)
{
	PID_Calc(&cloud_pitch_speed_pid, 0, mpu6050.Gyro.Origin.y);
}

void motor_control(void)
{
	//���̡���̨
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
	//������Ħ����
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

// //����ϵͳ
// struct JUDGE judge;

// RxPID rxPID;


/****************************************/
/****************function****************/
/****************************************/