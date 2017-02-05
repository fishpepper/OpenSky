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

#include "soft_serial.h"
#include "hal_soft_serial.h"
#include "debug.h"
#include "main.h"
#include "config.h"

#define SOFTSERIAL_DEBUG_RX 0

#ifndef HUB_TELEMETRY_ON_SBUS_UART

volatile EXTERNAL_MEMORY uint16_t soft_serial_databits;
volatile EXTERNAL_MEMORY uint16_t soft_serial_databit_count;
volatile soft_serial_rx_callback_t soft_serial_rx_callback;

void soft_serial_init(void) {
    debug("soft_serial: init\n"); debug_flush();

    soft_serial_databits = 0;
    soft_serial_databit_count = 0;
    soft_serial_rx_callback = 0;

    hal_soft_serial_init();
}

void soft_serial_set_rx_callback(soft_serial_rx_callback_t callback) {
    soft_serial_rx_callback = callback;
}

void soft_serial_process_startbit(void) {
    // reset incoming bit data:
    soft_serial_databits = 0;
    // sample 8N1 -> 1 start-, 8 data-, and 1 stop-bit
    soft_serial_databit_count = 1+8+1;
}

uint8_t soft_serial_process_databit(void) {
    // handle data
    if (soft_serial_databit_count != 0) {
        // sample bits
#ifdef HUB_TELEMETRY_INVERTED
        if (HUB_TELEMETRY_PIN_LO()) {
#else
        if (HUB_TELEMETRY_PIN_HI()) {
#endif
            soft_serial_databits |= (1<<10);
        }
        soft_serial_databit_count--;
        soft_serial_databits >>= 1;
    }

    if (soft_serial_databit_count == 0) {
        uint8_t data_byte;
        data_byte = (soft_serial_databits >> 1) & 0xFF;

        // process incoming data byte
        if ((soft_serial_databits & (1<<0)) != 0) {
            // FRAME ERROR: start bit invalid
            debug_putc('S');
        }else if ((soft_serial_databits & (1<<9)) == 0) {
            // FRAME ERROR: stop bit invalid
            debug_putc('s');
            // debug_put_hex16(soft_serial_databits); debug_put_newline();
        }else{
            // fine, data byte received
            // process data
            if (soft_serial_rx_callback != 0) {
                // execute callback
                soft_serial_rx_callback(data_byte);
            }
        }
#if SOFTSERIAL_DEBUG_RX
        debug("RX: 0x"); debug_put_hex8(data_byte); debug_put_newline();
        // debug_putc(data_byte);
#endif

        // finished data byte
        return 1;
    }

    // not yet finished
    return 0;
}

#endif
