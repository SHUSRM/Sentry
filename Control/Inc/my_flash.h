#ifndef __MY_FLASH_H
#define __MY_FLASH_H

#include "stm32f4xx_HAL.h"

#define MY_SECTOR_ADDR 0x081E0000
#define MY_SECTOR FLASH_SECTOR_23

typedef struct MY_FLASH_DATA
{
    float track_len;

    uint32_t end_word;  //保证读写过程中数据完整
}MY_FLASH_DATA;

extern MY_FLASH_DATA my_data;

uint32_t flash_read_word(uint32_t faddr);
void flash_read(uint32_t *pBuff, uint16_t num_to_read);
void flash_write(uint32_t *pBuff, uint16_t len);

void read_para(MY_FLASH_DATA * data);
void write_para(MY_FLASH_DATA * data);

#endif