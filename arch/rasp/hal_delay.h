#ifndef __HAL_DELAY_H__
#define __HAL_DELAY_H__
#include "hal_timeout.h"

#define hal_delay_us(us) hal_timeout_delay_us(us)
#define hal_delay_ms(ms) hal_timeout_delay_us((ms) * 1000)


#endif // __HAL_DELAY_H__
