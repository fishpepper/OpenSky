/*
    Copyright 2017 fishpepper <AT> gmail.com

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http:// www.gnu.org/licenses/>.

   author: fishpepper <AT> gmail.com
*/

#ifndef HAL_SOFT_SERIAL_H_
#define HAL_SOFT_SERIAL_H_

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

#define HUB_TELEMETRY_PIN_HI() (HUB_TELEMETRY_PORT & (1 << HUB_TELEMETRY_PIN))
#define HUB_TELEMETRY_PIN_LO() (!HUB_TELEMETRY_PIN_HI())

#define HAL_SOFT_SERIAL_UPDATE_TOP_VALUE(x) { T4CC0 = x; }

extern void hal_soft_serial_update_interrupt(void) __interrupt T4_VECTOR;
extern void hal_soft_serial_startbit_interrupt(void) __interrupt P0INT_VECTOR;

#endif  // HUB_TELEMETRY_ON_SBUS_UART

#endif  // HAL_SOFT_SERIAL_H_

