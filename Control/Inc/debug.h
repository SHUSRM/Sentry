#ifndef __DEBUG_H
#define __DEBUG_H

#include "sys.h"

#define LED_R_ON PEout(7) = 0
#define LED_R_OFF PEout(7) = 1
#define LED_G_ON PFout(14) = 0
#define LED_G_OFF PFout(14) = 1
#define BEEP_ON PBout(4) = 1
#define BEEPOOFF PBout(4) = 0


void debug_init();

#endif