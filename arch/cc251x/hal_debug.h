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

void hal_debug_init(void);
void hal_debug_start_transmission(uint8_t ch);

#if (DEBUG_UART == USART0_P0) || (DEBUG_UART == USART0_P1)
  #define hal_debug_int_enabled() (IEN2 & (IEN2_UTX0IE))
#else
  #define hal_debug_int_enabled() (IEN2 & (IEN2_UTX1IE))
#endif  // DEBUG_UART == ...
#define hal_debug_int_enable() { sei(); }
#define hal_debug_int_disable() { cli(); }

static void hal_debug_set_mode(__xdata union hal_uart_config_t *cfg);
#if (DEBUG_UART == USART0_P0) || (DEBUG_UART == USART0_P1)
  #define DEBUG_ISR(void) hal_uart_tx_interrupt(void) __interrupt UTX0_VECTOR
  #define HAL_DEBUG_ISR_FLAG_SET() (1)
  #define HAL_DEBUG_ISR_CLEAR_FLAG() { UTX0IF = 0; }
  #define HAL_DEBUG_ISR_DISABLE() { IEN2 &= ~(IEN2_UTX0IE); }
  #define HAL_DEBUG_TX_DATA(data) { U0DBUF = data; }
#else
  #define DEBUG_ISR(void) hal_uart_tx_interrupt(void) __interrupt UTX1_VECTOR
  #define HAL_DEBUG_ISR_FLAG_SET() (1)
  #define HAL_DEBUG_ISR_CLEAR_FLAG() { UTX1IF = 0; }
  #define HAL_DEBUG_ISR_DISABLE() { IEN2 &= ~(IEN2_UTX1IE); }
  #define HAL_DEBUG_TX_DATA(data) { U1DBUF = data; }
#endif  // DEBUG_UART == ...

#endif  // HAL_DEBUG_H_
