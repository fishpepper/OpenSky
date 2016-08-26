#ifndef __HAL_SOFT_SERIAL_H__
#define __HAL_SOFT_SERIAL_H__
#include "hal_cc25xx.h"
#include "cc2510fx.h"

void hal_soft_serial_init(void);
void hal_soft_serial_init_gpio(void);
void hal_soft_serial_init_interrupts(void);

// at 9600 baud a bit duration is 1/9600s = 104.166667us
// the counter counts in 1/1.625th of us -> 104.1667us * 1.625
// = 169,27... -> 169 -> 0.16% error (thats ok..)
#define HAL_SOFTSERIAL_BIT_DURATION_TICKS  169

#define HAL_SOFT_SERIAL_PIN_HI() (SOFT_SERIAL_PORT & (1<<SOFT_SERIAL_PIN))
#define HAL_SOFT_SERIAL_PIN_LO() (! HAL_SOFT_SERIAL_PIN_HI())

#define HAL_SOFT_SERIAL_UPDATE_TOP_VALUE(x)   { T4CC0 = x; }

extern void hal_soft_serial_update_interrupt(void) __interrupt T4_VECTOR;
extern void hal_soft_serial_startbit_interrupt(void) __interrupt P0INT_VECTOR;

#endif // __HAL_SOFT_SERIAL_H__
