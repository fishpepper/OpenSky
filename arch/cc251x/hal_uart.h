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

#ifndef HAL_UART_H_
#define HAL_UART_H_

#include <stdint.h>
#include "hal_defines.h"

// for a 26MHz Crystal:
#define CC2510_BAUD_E_115200  12
#define CC2510_BAUD_M_115200  34
// best match for 100kbit/s = 99975.5859375 bit/s
// baudrate = (((256.0 + baud_m)*2.0**baud_e) / (2**28)) * 26000000.0
#define CC2510_BAUD_E_100000  11
#define CC2510_BAUD_M_100000 248
#define CC2510_BAUD_E_57600   11
#define CC2510_BAUD_M_57600   34

union hal_uart_config_t{
  uint8_t byte;
  struct {
        uint8_t START : 1;  // start bit level
        uint8_t STOP  : 1;  // stop bit level
        uint8_t SPB   : 1;  // stop bits (0=1, 1=2)
        uint8_t PARITY: 1;  // parity (on/off)
        uint8_t BIT9  : 1;  // 9 bit mode
        uint8_t D9    : 1;  // 9th bit level or parity type
        uint8_t FLOW  : 1;  // flow control
        uint8_t ORDER : 1;  // data bit order (LSB or MSB first)
  } bit;
};

void hal_uart_init(void);
static void hal_uart_set_mode(EXTERNAL_MEMORY union hal_uart_config_t *cfg);
void hal_uart_start_transmission(uint8_t *data, uint8_t len);

#ifdef HUB_TELEMETRY_ON_SBUS_UART
#if (SBUS_UART == USART0_P1) || (SBUS_UART == USART0_P0)
  #define HAL_UART_RX_ISR(void) hal_uart_rx_interrupt(void) __interrupt URX0_VECTOR
  #define HAL_UART_RX_ISR_CLEAR_FLAG() { URX0IF = 0; }
  #define HAL_UART_RX_GETCH() (U0DBUF)
#else
  #define HAL_UART_RX_ISR(void) hal_uart_rx_interrupt(void) __interrupt URX1_VECTOR
  #define HAL_UART_RX_ISR_CLEAR_FLAG() { URX1IF = 0; }
  #define HAL_UART_RX_GETCH() (U1DBUF)
#endif  // SBUS_UART == ...

  void HAL_UART_RX_ISR(void);
#endif  // HUB_TELEMETRY_ON_SBUS_UART

#endif  // HAL_UART_H_
