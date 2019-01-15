/**
  *@file can.h
  *@date 2018-10-7
  *@author Vacuo.W
  *@brief 
  */
  
  
#ifndef _CAN_MY_H
#define _CAN_MY_H

#include "stm32f4xx_hal.h"
#include "main.h"

struct underpan_parameter 
{
	uint16_t mechanical_angle;	//位置角度
	int16_t rotation_rate;		//转速
	int16_t motor_current;		
	uint16_t motor_temperature;
	int16_t motor_output;
	int16_t set_speed;
	int16_t i_output;
	int16_t i_interg;
	int16_t s_interg;
	int32_t sum_current;
	uint8_t current_flag;
	int16_t current_store[10];
	uint8_t current_count;
	int16_t average_current;
};

typedef struct cloud_parameter 
{
	uint16_t mechanical_angle;//机械角度
	int16_t Bmechanical_angle;//变换后角度
	int16_t torque;//转矩电流测量
	int16_t torque_current;//转矩电流给定
	int16_t iout;

	int16_t motor_output;
	int16_t set_speed;
} cloud_parameter;

typedef struct dan_parameter 
{
	uint16_t mechanical_angle;//机械角度
	
	int16_t speed;//转矩电流测量
	int16_t torque_current;//转矩电流给定
	
	int16_t error[3];
	int16_t motor_output;
} dan_parameter;

extern CAN_HandleTypeDef hcan1;
extern CAN_TxHeaderTypeDef  Tx1Message;
extern CAN_RxHeaderTypeDef  Rx1Message;
static uint8_t aData[8];

extern struct underpan_parameter underpan_para[4];
extern struct cloud_parameter cloud_pitch,cloud_yaw;
extern struct dan_parameter dan;

void CAN1_Init(void);
void CAN_Getdata(CAN_HandleTypeDef *hcan,CAN_RxHeaderTypeDef *pHeader,uint8_t aData[]);
void HAL_CAN_RxFifo1MsgPendingCallback(CAN_HandleTypeDef *hcan);
void Underpan_motor_output(int16_t iq1,int16_t iq2,int16_t iq3,int16_t iq4);
void Cloud_motor_output(int16_t iq1,int16_t iq2,int16_t iq3);
void clearTxData(void);

#endif
