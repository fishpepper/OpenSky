#ifndef __HAL_CC25XX_H__
#define __HAL_CC25XX_H__

#include "cc2510fx.h"

#define hal_cc25xx_init() {}
#define hal_cc25xx_set_register(reg, val) { reg = val; }
#define hal_cc25xx_strobe(val) { RFST = val; }

#define CC25XX_FIFO FIFO

#endif // __HAL_CC25XX_H__
