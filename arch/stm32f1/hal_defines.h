#ifndef __HAL_DEFINES_H__
#define __HAL_DEFINES_H__

#include "stm32f10x_rcc.h"

#define EXTERNAL_MEMORY
#define EXTERNAL_DATA

// auto selector for APB1/APB2:
#define RCC_APBxPeriphClockCmd(rcc, p, s) { if (rcc == 1) { RCC_APB1PeriphClockCmd(p,s); }else{ RCC_APB2PeriphClockCmd(p, s); } }
#define RCC_APBxPeriphResetCmd(rcc, p, s) { if (rcc == 1) { RCC_APB1PeriphResetCmd(p,s); }else{ RCC_APB2PeriphResetCmd(p, s); } }

#define LO(w) (w & 0xFF)
#define HI(w) ((w>>8) & 0xFF)

#endif // __HAL_DEFINES_H__
