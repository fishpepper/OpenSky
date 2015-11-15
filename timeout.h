#ifndef __TIMEOUT_H__
#define __TIMEOUT_H__

#include "cc2510fx.h"
#include "main.h"

extern volatile uint16_t timeout_countdown;

void timeout_init(void);
void timeout_set(uint16_t timeout_ms);
uint8_t timeout_timed_out(void);
void timeout_interrupt(void) __interrupt T3_VECTOR;

#endif
