/*
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

#include "main.h"
#include "hal_defines.h"
#include "debug.h"
#include "wdt.h"
#include "delay.h"
#include "uart.h"
#include "hal_uart.h"

#ifdef SBUS_ENABLED
volatile uart_rx_callback_t uart_rx_callback;

void uart_init(void) {
    debug("uart: init\n"); debug_flush();

    uart_rx_callback = 0;

    hal_uart_init();
}

void uart_start_transmission(uint8_t *buf, uint8_t len) {
    // send data!
    hal_uart_start_transmission(buf, len);
}

void uart_set_rx_callback(uart_rx_callback_t callback) {
    uart_rx_callback = callback;
}
#endif

