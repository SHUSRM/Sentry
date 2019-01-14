#ifndef __TELE_CONTROL_H
#define __TELE_CONTROL_H

#include "stm32f4xx_HAL.h"

//遥控器开关位置
#define TOP     0x01
#define BOTTOM  0x02
#define MIDDLE  0x03


/*****teler*******/
typedef struct TELE_CON_DATA
{
	int16_t ch0;
	int16_t ch1;
	int16_t ch2;
	int16_t ch3;

	uint8_t sl;
	uint8_t sr;
	
	int16_t x;
	int16_t y;
	int16_t z;
	uint16_t key;   

	uint16_t resv;
	uint8_t press_l;
	uint8_t press_r;
	
}TELE_CON_DATA;

extern uint8_t teledata_rx[18];
extern TELE_CON_DATA tele_data;
extern uint8_t tele_timer;

void telecontroller_data(void);
void clear_teledata(void);
void renew_tele_timer(void);


#endif