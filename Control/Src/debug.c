#include "debug.h"

void debug_init()
{
    LED_G_OFF;
    LED_R_OFF;
    return;
}

uint16_t flash_read_halfword(uint32_t faddr)
{
    return *(uint16_t*)faddr;
}