#ifndef __HAL_SOFT_SERIAL_H__
#define __HAL_SOFT_SERIAL_H__
#include "hal_cc25xx.h"
#include "cc2510fx.h"
#include "config.h"

#ifndef HUB_TELEMETRY_ON_SBUS_UART

void hal_soft_serial_init(void);
void hal_soft_serial_init_gpio(void);
void hal_soft_serial_init_interrupts(void);

// at 9600 baud a bit duration is 1/9600s = 104.166667us
// the counter counts in 1/1.625th of us -> 104.1667us * 1.625
// = 169,27... -> 169 -> 0.16% error (thats ok..)
#define HAL_SOFTSERIAL_BIT_DURATION_TICKS  (169)

#define HUB_TELEMETRY_PIN_HI() (HUB_TELEMETRY_PORT & (1<<HUB_TELEMETRY_PIN))
#define HUB_TELEMETRY_PIN_LO() (! HUB_TELEMETRY_PIN_HI())

#define HAL_SOFT_SERIAL_UPDATE_TOP_VALUE(x)   { T4CC0 = x; }

extern void hal_soft_serial_update_interrupt(void) __interrupt T4_VECTOR;
extern void hal_soft_serial_startbit_interrupt(void) __interrupt P0INT_VECTOR;

#endif

#endif // __HAL_SOFT_SERIAL_H__

