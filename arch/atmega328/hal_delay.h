#ifndef __HAL_DELAY_H__
#define __HAL_DELAY_H__
#include "hal_timeout.h"
#include <util/delay.h>

#define hal_delay_ms(ms) _delay_ms(ms);
#define hal_delay_us(us) _delay_us(us);

#endif // __HAL_DELAY_H__
