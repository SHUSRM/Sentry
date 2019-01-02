#include "my_flash.h"
#include "stm32f4xx_hal_flash.h"
#include "stm32f4xx_hal_flash_ex.h"
#include "stm32f4xx_hal_flash_ramfunc.h"

//读取指定地址的半字(16位数据)
//faddr:读地址(此地址必须为2的倍数!!)
//返回值:对应数据.
uint32_t flash_read_word(uint32_t faddr)
{
    return *(uint32_t*)faddr;
}

//从指定地址开始读出指定长度的数据
//ReadAddr:起始地址
//pBuffer:数据指针
//NumToWrite:字(32位)数
void flash_read(uint32_t read_addr, uint32_t *pBuff, uint16_t num_to_read)
{
    int i;
    for (i = 0; i < num_to_read; i ++)
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

    if(HAL_FLASHEx_Erase(&flash_erase_init, &SectorError) != HAL_OK)
    {
        HAL_FLASH_Lock();
        return;
    }
    FlashStatus = FLASH_WaitForLastOperation(50000);
    if(FlashStatus == HAL_OK)
    {
        for(offset = 0; offset <= len - 1; offset++)
        {
            if(HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, addrx, *pBuffer) != HAL_OK)
            {
                return;
            }
            addrx += 4;
            pBuffer ++;
        }
    }
    HAL_FLASH_Lock();
}
