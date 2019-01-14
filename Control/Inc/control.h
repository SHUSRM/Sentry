#ifndef _CONTROL_H
#define _CONTROL_H

#include "stm32f4xx_HAL.h"

#define pitch_mid  -550	//云台pitch轴初值   800				-800black
#define yaw_mid -2600	//云台yaw轴初值			300		1000black

//运行模式
#define MANUAL  0x00
#define AUTO    0x01
#define DEFAULT 0x02
#define DEBUG   0x03

//轨道边界校正模式
#define SET_START	0x01
#define	SET_END 	0x02
#define MEASURE_LEN	0x03
#define START_MEASURE 0x04
#define END_MEASURE	0x05

//自动模式
#define FULL_AUTO   0x01
#define CLOUD_AUTO  0x02
#define UNDERPAN_AUTO   0x03

extern uint8_t run_mod;

extern uint8_t underpan_switch;
extern uint8_t cloud_ctrl_switch;
extern uint8_t shoot_switch;

extern uint8_t track_mod;
extern float track_position;
extern float track_len;
extern int8_t movement_dir;

extern uint8_t auto_mod;

void switch_control(void);
void underpan_control(float speed_ref);
void rounds_control(float speed_ref);
void cloud_control(void);
void motor_control(void);


//*************underpan**************//


// typedef struct CAMERA
// {
//     uint8_t recieve[1];
//     uint8_t count;
//     uint8_t transmit[1];
//     int16_t x;
//     int16_t y;
// 	int16_t x_last;
// 	int16_t y_last;
//     uint8_t sum;
// } CAMERA;

// typedef struct JUDGE
// {
//     uint8_t recieve[1];
//     uint8_t count;
//     uint8_t transmit[1];
// //    uint16_t x;
// //    uint16_t y;
// //    uint8_t sum;
// } JUDGE;

// typedef struct
// {
//     uint8_t Count;
// 	uint8_t Buf[20];
// 	uint8_t	Sum;
// 	uint8_t	pidReadBuf;
// 	PID_Regulator_t* 	pidAdjust;
// } RxPID;



// extern RxPID rxPID;
// extern struct CAMERA camera;
// extern struct JUDGE judge;
// extern int16_t pitch;
// extern int16_t yaw;



#endif
