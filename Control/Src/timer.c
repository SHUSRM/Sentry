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
#include "tele_control.h"

uint16_t Timetick1ms = 0;
float output[4];

void Timer_interrupt(void)
{
  LED_G_OFF;

	Timetick1ms++;
	MPU6050_GetData();

  switch_control();

  if(Timetick1ms % 9 == 0)
  {
    motor_control();
  }

  // output[0] = mpu6050.Gyro.Origin.x;
  // output[1] = mpu6050.Gyro.Origin.y;
  // output[2] = mpu6050.Gyro.Origin.z;
  // output[0] = underpan_motor[0].output;
	output[1] = underpan_motor[0].ref;
  output[2] = track_len;
  output[3] = track_position;

  sendware(output, sizeof(output));

  // Underpan_motor_output();
  //Cloud_motor_output(200, 200);
	

	if (Timetick1ms>999) Timetick1ms=0;
  
	renew_tele_timer();
	LED_G_ON;
}
