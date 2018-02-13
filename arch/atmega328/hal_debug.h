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

#ifndef HAL_DEBUG_H_
#define HAL_DEBUG_H_

#include "hal_cc25xx.h"
#include "hal_uart.h"
#include <stdint.h>

#define hal_debug_init() hal_uart_init()
#define hal_debug_start_transmission(ch) hal_uart_start_transmission(ch)


#define hal_debug_int_enabled() (0)

#define hal_debug_int_enable() {}
#define hal_debug_int_disable() {}

#define DEBUG_ISR(void) hal_uart_interrupt(void)
#define HAL_DEBUG_ISR_FLAG_SET() (1)
#define HAL_DEBUG_ISR_CLEAR_FLAG() {}
#define HAL_DEBUG_ISR_DISABLE() {}
#define HAL_DEBUG_TX_DATA(data) hal_uart_tx_data(data)



#endif  // HAL_DEBUG_H_
