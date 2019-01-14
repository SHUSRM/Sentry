#include "tele_control.h"

/************遥控器数据***************/
uint8_t teledata_rx[18];
TELE_CON_DATA tele_data;

//计时器，200ms没收到遥控器数据就表示遥控器离线
uint8_t tele_timer;


/*****接收遥控器数据*****/
void telecontroller_data(void)
{	
//	uint8_t w,a,s,d,press_l,press_r,shift;
//	int16_t speed;

	tele_data.ch0=((teledata_rx[0]| (teledata_rx[1] << 8)) & 0x07ff)-1024;						//右摇杆：左右
	tele_data.ch1=(((teledata_rx[1] >> 3) | (teledata_rx[2] << 5)) & 0x07ff)-1024;				//右摇杆：上下
	tele_data.ch2=(((teledata_rx[2] >> 6) | (teledata_rx[3] << 2) | (teledata_rx[4] << 10)) & 0x07ff)-1024;			//左：左右
	tele_data.ch3=(((teledata_rx[4] >> 1) | (teledata_rx[5] << 7)) & 0x07ff)-1024; 				//左：上下
	tele_data.sl=((teledata_rx[5] >> 4)& 0x000C) >> 2;					//左上开关：上中下对应132
	tele_data.sr=((teledata_rx[5] >> 4)& 0x0003);						//右上
	tele_data.x=teledata_rx[6] | (teledata_rx[7] << 8);
	tele_data.y=teledata_rx[8] | (teledata_rx[9] << 8); 
	tele_data.z=teledata_rx[10] | (teledata_rx[11] << 8);
	tele_data.press_l=teledata_rx[12];
	tele_data.press_r=teledata_rx[13];
	tele_data.key=teledata_rx[14] | (teledata_rx[15] << 8);
	tele_data.resv=teledata_rx[16]|(teledata_rx[17]<<8);

}

//清空遥控器数据
void clear_teledata(void)
{
    tele_data.ch0 = 0;
    tele_data.ch1 = 0;
    tele_data.ch2 = 0;
    tele_data.ch3 = 0;
    tele_data.sl = 0;
    tele_data.sr = 0;
    tele_data.x = 0;
    tele_data.y = 0;
    tele_data.z = 0;
    tele_data.press_l = 0;
    tele_data.press_r = 0;
    tele_data.key = 0;
    tele_data.resv = 0;
}

//定时器中断更新遥控器状态
void renew_tele_timer(void)
{
    tele_timer--;
    if (tele_timer == 0)    
    {
        clear_teledata();
        tele_timer = 200;
    }
}

//dma中断更新遥控器状态
void renew_tele_dma(void)
{
    tele_timer = 200;
}