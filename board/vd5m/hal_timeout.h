#ifndef __HAL_TIMEOUT_H__
#define __HAL_TIMEOUT_H__

#include "cc2510fx.h"

extern volatile uint16_t hal_timeout_countdown;

void hal_timeout_init(void);
void hal_timeout_set(uint16_t timeout_ms);
uint8_t hal_timeout_timed_out(void);

void hal_timeout_interrupt(void) __interrupt T3_VECTOR;

#endif
