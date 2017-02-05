#ifndef __HAL_DELAY_H__
#define __HAL_DELAY_H__
#include "hal_timeout.h"

#define hal_delay_ms(ms) hal_timeout_delay_ms(ms)
// FIXME!
// #define hal_delay_us(us) { uint32_t _i; for (_i=0; _i<6*us; _i++) { asm("nop"); }; }
// #define hal_delay_us(us) { if (us < 100) { uint32_t i; for (i=0; i<600; i++) { }; } else { hal_timeout_delay_100us(us/100); }}
void hal_delay_us(uint32_t us);


#endif // __HAL_DELAY_H__