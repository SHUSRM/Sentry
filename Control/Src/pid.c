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

/************CLOUD***************/
PID_Regulator_t cloud_pitch_speed_pid;
PID_Regulator_t cloud_pitch_position_pid;
PID_Regulator_t cloud_yaw_speed_pid;
PID_Regulator_t cloud_yaw_position_pid;
PID_Regulator_t underpan_motor[4];
PID_Regulator_t dan_pid;

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

		if(pid->inte > pid->componentKiMax)//对积分项进行限制
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

void PID_Init(PID_Regulator_t *pid,float kp,float ki,float kd,float componentKiMax,float outputMax, PID_type type)
{
	pid->kp = kp;
	pid->ki = ki;
	pid->kd = kd;
	pid->inte = 0;

	pid->componentKiMax = componentKiMax;

	pid->outputMax = outputMax;
	pid->type = type;
}

void Cloud_Speed(void)
{
	//PITCH
	PID_Calc(&cloud_pitch_speed_pid, cloud_pitch_position_pid.output, mpu6050.Gyro.Origin.y);
	
	//YAW
	PID_Calc(&cloud_yaw_speed_pid, cloud_yaw_position_pid.output, mpu6050.Gyro.Origin.x);
}

void Cloud_Position(void)
{
	//PITCH
	PID_Calc(&cloud_pitch_position_pid, pitch_mid, cloud_pitch.Bmechanical_angle);
	
	//YAW
	PID_Calc(&cloud_yaw_position_pid, yaw_mid, cloud_yaw.Bmechanical_angle);
	
}

void ALLPID_Init()
{
	int i;
	PID_Init(&dan_pid, 2, 1, 0, 2000, 2000, increment);

	PID_Init(&cloud_pitch_position_pid,	0, 0, 0, 3000, 10000, positional);
	PID_Init(&cloud_pitch_speed_pid	  , 0, 1, 0, 2000, 5000, positional);
	
	PID_Init(&cloud_yaw_position_pid, 0, 0, 0, 1000, 12000, positional);
	PID_Init(&cloud_yaw_speed_pid	, 0, 1, 0, 1000, 5000, positional);

	for (i = 0;i < 4; i++)
	{
		PID_Init(&underpan_motor[i], 5, 0.03, 0, 8000, 16000, increment);
	}
	
}
