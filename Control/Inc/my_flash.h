#ifndef __MY_FLASH_H
#define __MY_FLASH_H

#include "stm32f4xx_HAL.h"

#define MY_SECTOR_ADDR 0x081E0000
#define MY_SECTOR FLASH_SECTOR_23

// typedef struct MY_FLASH_DATA
// {
//     float 
// }

uint32_t flash_read_word(uint32_t faddr);
void flash_read(uint32_t read_addr, uint32_t *pBuff, uint16_t num_to_read);
void flash_write(uint32_t *pBuff, uint16_t len);

#endif