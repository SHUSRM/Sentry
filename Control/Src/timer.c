/**
  *@file timer.c
  *@date 2018-10-7
  *@author Vacuo.W
  *@brief 
  */

#include "timer.h"
#include "control.h"
#include "can_my.h"
#include "pid.h"
#include "mpu6050.h"
#include "debug.h"

uint16_t Timetick1ms = 0;
float output[4];

void Timer_interrupt(void)
{
  LED_G_OFF;

	Timetick1ms++;
	MPU6050_GetData();


  if(Timetick1ms % 9 == 0)
  {
    underpan_motor[0].fdb = underpan_para[0].rotation_rate;
    underpan_motor[0].ref = 2000;
    PID_Calc(underpan_motor);
    Underpan_motor_output(underpan_motor[0].output, 0, 0, 0);
  }

  output[0] = underpan_motor[0].inte;
  output[1] = underpan_para[0].rotation_rate;
  output[2] = underpan_motor[0].output;

  // output[0] = mpu6050.Gyro.Origin.x;
  // output[1] = mpu6050.Gyro.Origin.y;
  // output[2] = mpu6050.Gyro.Origin.z;
  // output[0] = underpan_para[0].mechanical_angle;

  sendware(output, sizeof(output));

  // Underpan_motor_output();
  //Cloud_motor_output(200, 200);
	
	if (Timetick1ms>999) Timetick1ms=0;
	
	LED_G_ON;
}
