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
#include "filter.h"
#include "usart_my.h"

uint16_t Timetick1ms = 0;
float output[4];

void Timer_interrupt(void)
{
  LED_G_OFF;

	Timetick1ms++;
	MPU6050_GetData();

  mFilterPutData(&mpuPitch50HZ, mpu6050.Gyro.Origin.y);
  mFilterPutData(&mpuYaw50HZ, mpu6050.Gyro.Origin.x - mpu6050.Gyro.Origin.z);


  switch_control();
  motor_control();

  if(Timetick1ms % 1 == 0)
  {
    cloud_speed_control();
  }

  if(Timetick1ms % 3 == 0)
  {
    cloud_position_control();
  }

  if(Timetick1ms % 9 == 0)
  {
    underpan_control();
  }

    output[0] = camera.x;
    output[1] = camera.y;
  // output[0] = underpan_para[0].rotation_rate;
  // output[1] = -underpan_para[1].rotation_rate;
  // output[2] = underpan_motor[0].output;
  // output[3] = -underpan_motor[1].output;

  // output[0] = mpuPitch50HZ.filtered_value;
  // output[1] = cloud_pitch_speed_pid.output;
  // output[2] = cloud_pitch_speed_pid.ref;
  // output[3] = cloud_pitch.mechanical_angle;

//   output[0] = mpuYaw50HZ.filtered_value;
//   output[1] = cloud_yaw_speed_pid.output;
//   output[2] = cloud_yaw_position_pid.fdb;
//   output[3] = yaw_position_ref;

   // output[0] = deDanTimer;
  // output[1] = dan_pid.output;
  // output[2] = dan.speed;

  // output[3] = dan.speed;
  // output[0] = cloud_pitch.Bmechanical_angle;
  // output[1] = cloud_yaw.Bmechanical_angle;
  // output[0] = underpan_motor[0].output;
	// output[1] = underpan_motor[0].ref;
  // output[2] = track_len;
  // output[3] = track_position;
  // output[0] = tim_timer;
  
	if (Timetick1ms % 20 == 0){
		sendware(output, sizeof(output));
	}
  // Underpan_motor_output();
  //Cloud_motor_output(200, 200);

	if (Timetick1ms>999) 
  {
    Timetick1ms=0;
  }
	renew_tele_timer();
	LED_G_ON;
}
