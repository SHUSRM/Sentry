#include "my_flash.h"
#include "stm32f4xx_hal_flash.h"
#include "stm32f4xx_hal_flash_ex.h"
#include "stm32f4xx_hal_flash_ramfunc.h"
#include "delay.h"

MY_FLASH_DATA my_data;

//读取指定地址的半字(16位数据)
//faddr:读地址(此地址必须为2的倍数!!)
//返回值:对应数据.
uint32_t flash_read_word(uint32_t faddr)
{
    return *(uint32_t*)faddr;
}

//读出指定长度的数据
//pBuffer:数据指针
//len:字(32位)数
void flash_read(uint32_t *pBuff, uint16_t len)
{
    uint32_t read_addr = MY_SECTOR_ADDR;
    int i;
    for (i = 0; i < len; i ++)
    {
        pBuff[i] = flash_read_word(read_addr);
        read_addr += 4;
    }
}

//pBuffer:数据指针
//len:字(32 位)数(就是要写入的 32 位数据的个数.) 
void flash_write(uint32_t *pBuffer, uint16_t len)
{
    FLASH_EraseInitTypeDef flash_erase_init;
    HAL_StatusTypeDef FlashStatus = HAL_OK;
    uint32_t SectorError = 0;
    uint32_t addrx = MY_SECTOR_ADDR;    //扇区23
    uint32_t offset = 0;

    if(addrx % 4) return;

    HAL_FLASH_Unlock();
    flash_erase_init.TypeErase = FLASH_TYPEERASE_SECTORS;
    flash_erase_init.Sector = MY_SECTOR;
    flash_erase_init.NbSectors = 1;
    flash_erase_init.VoltageRange = FLASH_VOLTAGE_RANGE_3;

    HAL_FLASHEx_Erase(&flash_erase_init, &SectorError);
    
		delay_ms(100);
    
    while(offset <= len - 1)
    {
        FlashStatus = FLASH_WaitForLastOperation(50000);
        if(FlashStatus != HAL_OK)
        {
            continue;
        }
        if(HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, addrx, *pBuffer) != HAL_OK)
        {
            break;
        }
        addrx += 4;
        pBuffer ++;
		offset ++;
    }
    HAL_FLASH_Lock();
}

//data:要读取数据的结构体指针
void read_para(MY_FLASH_DATA * data)
{
    uint32_t * data_point = (uint32_t *)data;
    uint32_t len = sizeof(*data) / 4;
    flash_read(data_point, len);
}

//data:要写入数据的结构体指针
void write_para(MY_FLASH_DATA * data)
{
    uint32_t * data_point = (uint32_t *)data;
    uint32_t len = sizeof(*data) / 4;
    flash_write(data_point, len);
}