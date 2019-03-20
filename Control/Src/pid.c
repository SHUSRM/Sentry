/**
  *@file timer.c
  *@date 2018-12-17
  *@author Vacuo.W
  *@brief 
  */
  
#include "pid.h"
#include "control.h"
#include "can_my.h"
#include "mpu6050.h"
#include "filter.h"

/************CLOUD***************/
PID_Regulator_t cloud_pitch_speed_pid;
PID_Regulator_t cloud_pitch_position_pid;
PID_Regulator_t cloud_yaw_speed_pid;
PID_Regulator_t cloud_yaw_position_pid;
PID_Regulator_t underpan_motor[4];
PID_Regulator_t dan_pid;

float mySin[1000] = sin1000Hz;

void PID_Calc(PID_Regulator_t *pid, float ref, float fdb)
{
	pid->ref = ref;
	pid->fdb = fdb;

	//位置式
	if (pid->type == positional)
	{
		pid->err[1] = pid->err[0];
		pid->err[0] = pid->ref - pid->fdb;
		pid->inte += pid->err[0] * pid->ki;

		//死区
		if(pid->err[0] > -pid->deadZone && pid->err[0] < pid->deadZone)	return;

		if(pid->inte > pid->componentKiMax)//积分限幅
			pid->inte = pid->componentKiMax;
		else if (pid->inte < -pid->componentKiMax)
			pid->inte = -pid->componentKiMax;

		pid->output = (
					   pid->err[0] * pid->kp					//P
					 + pid->inte								//I
					 + (pid->err[0] - pid->err[1]) * pid->kd	//D
					  );
	}
	else	
	//增量式
	{
		pid->err[2] = pid->err[1];
		pid->err[1] = pid->err[0];
		pid->err[0] = pid->ref - pid->fdb;

		if(pid->err[0] > -pid->deadZone && pid->err[0] < pid->deadZone)	return;

		pid->output += (
						(pid->err[0] - pid->err[1]) * pid->kp
					  + (pid->err[0]) * pid->ki
					  + (pid->err[0] - 2 * pid->err[1] + pid->err[2]) * pid->kd
					   );
	}
	//输出限幅
	if(pid->output > pid->outputMax)
		pid->output = pid->outputMax;
	else if (pid->output < -pid->outputMax)
		pid->output = -pid->outputMax;	
}

void PID_Init(PID_Regulator_t *pid,float kp,float ki,float kd,float componentKiMax,float deadZone, float outputMax, PID_type type)
{
	pid->kp = kp;
	pid->ki = ki;
	pid->kd = kd;
	pid->inte = 0;

	pid->componentKiMax = componentKiMax;
	pid->deadZone = deadZone;

	pid->outputMax = outputMax;
	pid->type = type;
}


void ALLPID_Init()
{
	int i;
	PID_Init(&dan_pid, 2, 0.1,0, 4000, 0, 4000, increment);

	PID_Init(&cloud_pitch_position_pid,	8, -0.000, 20, 5, 0, 10000, positional);
	PID_Init(&cloud_pitch_speed_pid	  , -0.7, -0.05, -1.5, 7000, 0, 3000, increment);
	
	PID_Init(&cloud_yaw_position_pid, -9, -0.0, -5, 50, 0, 12000, positional);
	PID_Init(&cloud_yaw_speed_pid	, 1.5, 0.01, 0.1, 1000, 0, 5000, increment);

	for (i = 0;i < 4; i++)
	{
		PID_Init(&underpan_motor[i], 5, 0.03, 0, 8000, 0, 16000, increment);
	}
	
}
