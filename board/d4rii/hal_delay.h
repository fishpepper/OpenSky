#ifndef __HAL_DELAY_H__
#define __HAL_DELAY_H__
#include "hal_timeout.h"

#define hal_delay_ms(ms) hal_timeout_delay_ms(ms)
//FIXME!
#define hal_delay_us(us) { uint32_t i; for(i=0; i<700; i++){ }; } 

#endif // __HAL_DELAY_H__