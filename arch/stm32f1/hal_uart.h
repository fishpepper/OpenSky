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

void hal_uart_init();
static void hal_uart_init_gpio(void);
static void hal_uart_init_rcc(void);
static void hal_uart_init_mode(void);
static void hal_uart_init_nvic(void);
void hal_uart_start_transmission(uint8_t *buffer, uint8_t len);

void SBUS_USART_IRQHANDLER(void);

#endif  // HAL_UART_H_
