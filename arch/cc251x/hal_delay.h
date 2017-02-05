#ifndef __HAL_DELAY_H__
#define __HAL_DELAY_H__
#include <stdint.h>

void hal_delay_ms(uint16_t ms);
void hal_delay_us(uint16_t us);
#define hal_delay_45nop(void) { uint8_t n=45; while (n--) { NOP(); } }

#endif
