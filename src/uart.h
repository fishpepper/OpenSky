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

#ifndef UART_H__
#define UART_H__
#include <stdint.h>

typedef void (*uart_rx_callback_t)(uint8_t);
extern volatile uart_rx_callback_t uart_rx_callback;

void uart_init(void);
void uart_start_transmission(uint8_t *buf, uint8_t len);
void uart_set_rx_callback(uart_rx_callback_t cb);

#endif  // UART_H_

