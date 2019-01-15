#ifndef _PID_H
#define _PID_H

#include "stm32f4xx_HAL.h"

#define SGN(x) ((x >= 0) ? 1 : -1)
#define ABS(x) ((x >= 0)? x : -x)
#define SQR(X) (X*X)

typedef enum PID_type
{
	positional,
	increment,
}PID_type;

typedef struct
{
	float ref;//输入：系统待调节量的给定值
	float fdb;//输入：系统待调节量的反馈值
	float inte;//积分值
	float err[3];
	float kp;
	float ki;
	float kd;

	float componentKiMax;

	float output;
	float outputMax;
	PID_type type;
}PID_Regulator_t;


extern PID_Regulator_t cloud_pitch_speed_pid;
extern PID_Regulator_t cloud_pitch_position_pid;
extern PID_Regulator_t cloud_yaw_speed_pid;
extern PID_Regulator_t cloud_yaw_position_pid;
extern PID_Regulator_t underpan_motor[4];
extern PID_Regulator_t dan_pid;


void PID_Calc(PID_Regulator_t *pid, float ref, float fdb);
void PID_Init(PID_Regulator_t *pid,float kp,float ki,float kd,float componentKiMax,float outputMax,PID_type type);
void Cloud_Speed(void);
void Cloud_Position(void);
void ALLPID_Init(void);




#endif
