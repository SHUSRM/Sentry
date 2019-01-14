/**
  *@file usart.c
  *@date 2018-10-11
  *@author Vacuo.W
  *@brief 
  */

#include "usart_my.h"
#include "control.h"
#include <math.h>
#include "pid.h"
#include "tele_control.h"

//struct CAMERA camera;

extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart4;
extern UART_HandleTypeDef huart6;

uint8_t cheat_ready=1;
uint8_t a;
double angle;

uint8_t RX_PID_Count = 0;
uint8_t RX_PID_Buf[20];
uint8_t	RX_PID_Sum = 0;
uint8_t	pidReadBuf;
uint8_t charBuf[4];
PID_Regulator_t 	*pidAdjust;

void sendware(void *wareaddr, uint32_t waresize)
{
	#define CMD_WARE     3
	uint8_t cmdf[2] = {CMD_WARE, ~CMD_WARE};    
	uint8_t cmdr[2] = {~CMD_WARE, CMD_WARE};   
	HAL_UART_Transmit(&huart4, (uint8_t *)cmdf, sizeof(cmdf), 5000);
	HAL_UART_Transmit(&huart4, (uint8_t *)wareaddr, waresize ,5000);
	HAL_UART_Transmit(&huart4, (uint8_t *)cmdr, sizeof(cmdr), 5000);
}
uint8_t data[10];

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	/************小电脑串口数据处理*************/
    if (huart->Instance==USART2)
    {
		// 	//sendware(camera.recieve,sizeof(camera.recieve));
        // switch (camera.count)
        // {
        //     case 0:
        //         if (camera.recieve[0]=='&') camera.count=1;
        //         else camera.count=0;
		// 						data[camera.count] = camera.recieve[0];
        //         break;
        //     case 1:
        //         if (camera.recieve[0]=='%') camera.count=2;
        //         else camera.count=0;
		// 						data[camera.count] = camera.recieve[0];
        //         break;
        //     case 2:
        //         camera.sum = '%'+'&';
        //         camera.x = camera.recieve[0]<<8;
        //         camera.sum += camera.recieve[0];
        //         camera.count=3;
		// 						data[camera.count] = camera.recieve[0];
        //         break;
        //     case 3:
        //         camera.x += camera.recieve[0];
        //         camera.sum += camera.recieve[0];
        //         camera.count=4;
		// 						data[camera.count] = camera.recieve[0];
        //         break;
        //     case 4:
        //         if (camera.recieve[0]=='-') camera.x = -camera.x;
        //         camera.sum += camera.recieve[0];
        //         camera.count=5;
		// 						data[camera.count] = camera.recieve[0];
        //         break;
        //     case 5:
        //         camera.y = camera.recieve[0]<<8;
        //         camera.sum += camera.recieve[0];
        //         camera.count=6;
		// 						data[camera.count] = camera.recieve[0];
        //         break;
        //     case 6:
        //         camera.y += camera.recieve[0];
        //         camera.sum += camera.recieve[0];
        //         camera.count=7;
		// 						data[camera.count] = camera.recieve[0];
        //         break;
        //     case 7:
        //         if (camera.recieve[0]=='-') camera.y = -camera.y;
        //         camera.sum += camera.recieve[0];
        //         camera.count=8;
		// 						data[camera.count] = camera.recieve[0];
        //         break;
        //     case 8:
        //         if (camera.sum==camera.recieve[0])
        //         {
		// 							data[9] = camera.recieve[0];
		// 								//sendware()
        //             camera.transmit[0]='R';
        //             HAL_UART_Transmit(&huart2,camera.transmit,1,1000);
										
		// 								if (tele_data.s1==1){
		// 									if (cheat_ready==1){				
		// 										angle=atan(camera.y*0.00222)*1004.05;
		// 										pitch=cloud_pitch.Bmechanical_angle+angle;
		// 										angle=atan(camera.x*0.00222)*1304.05;
		// 										yaw=cloud_yaw. Bmechanical_angle-angle ;
		// 										cheat_ready=0;

		// 									}
		// 										if(cloud_yaw.Bmechanical_angle-yaw>-30&&cloud_yaw.Bmechanical_angle-yaw<30)
		// 										if(cloud_pitch.Bmechanical_angle-pitch>-100&&cloud_pitch.Bmechanical_angle-pitch<100)
		// 											cheat_ready=1;
											
		// 								}}
        //         else {
        //             camera.x=0;
        //             camera.y=0;
        //             camera.transmit[0]='W';
        //             HAL_UART_Transmit(&huart2,camera.transmit,1,1000);
        //         }
        //         camera.count=0;
        //         break;
							
				
				

	
		// 			if(pitch<(pitch_mid-600))pitch=pitch_mid-600;
		// 			if(pitch>(pitch_mid+600))pitch=pitch_mid+600;
		// 			if(yaw<(yaw_mid-800))yaw=yaw_mid-800;
		// 			if(yaw>(yaw_mid+800))yaw=yaw_mid+800;
					
    // }   
	}
	/******************裁判系统串口数据处理********************/
	else if (huart->Instance==USART6)
	{
		
		
		
		
	// }
	
	// else if (huart->Instance == UART4)
	// {
	// 	rxPID.Buf[rxPID.Count & 0x7f] = rxPID.pidReadBuf;
	// 	//是否开始接收
	// 	if ((rxPID.Count & 0x7f) == 0 && rxPID.Buf[0] != '$')
	// 		return;

	// 	rxPID.Count++;

	// 	if ((rxPID.Count & 0x7f) == 8)
	// 	{
	// 		//接收正确
	// 		if (rxPID.Sum == rxPID.pidReadBuf)
	// 		{
	// 			for (int i = 0; i < 4; i++)
	// 				charBuf[i] = rxPID.Buf[i + 3];

	// 			switch (rxPID.Buf[1])
	// 			{
	// 			case 'p':
	// 				memcpy(&rxPID.pidAdjust->kp, charBuf, 4);
	// 				if (rxPID.Buf[2] == '-')
	// 					rxPID.pidAdjust->kp = -rxPID.pidAdjust->kp;
	// 				break;
	// 			case 'i':
	// 				memcpy(&rxPID.pidAdjust->ki, charBuf, 4);
	// 				if (rxPID.Buf[2] == '-')
	// 					rxPID.pidAdjust->ki = -rxPID.pidAdjust->ki;
	// 				break;
	// 			case 'd':
	// 				memcpy(&rxPID.pidAdjust->kd, charBuf, 4);
	// 				if (rxPID.Buf[2] == '-')
	// 					rxPID.pidAdjust->kd = -rxPID.pidAdjust->kd;
	// 				break;
	// 			}
	// 			rxPID.Sum = 0;
	// 			rxPID.Count = 0;
	// 		}
	// 		else
	// 		{
	// 			rxPID.Sum = 0;
	// 			rxPID.Count = 0;
	// 		}
	// 	}
	// 	else
	// 		rxPID.Sum += rxPID.pidReadBuf;
	}
    
 
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
	if(__HAL_UART_GET_FLAG(huart,UART_FLAG_ORE) != RESET) {
	
 __HAL_UART_CLEAR_OREFLAG(huart);
			a=huart->Instance->DR;
	}
		
	
}