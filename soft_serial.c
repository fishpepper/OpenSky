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
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

   author: fishpepper <AT> gmail.com
*/

#include "soft_serial.h"
#include "hal_soft_serial.h"
#include "debug.h"
#include "main.h"
#include "config.h"

volatile EXTERNAL_MEMORY uint16_t soft_serial_databits;
volatile EXTERNAL_MEMORY uint16_t soft_serial_databit_count;
soft_serial_rx_callback_t soft_serial_rx_callback;

void soft_serial_init(void) {
    debug("soft_serial: init\n"); debug_flush();

    soft_serial_databits = 0;
    soft_serial_databit_count = 0;
    soft_serial_rx_callback = 0;

    hal_soft_serial_init();
}


void soft_serial_set_rx_callback(soft_serial_rx_callback_t callback){
    debug("soft_serial: stored callback 0x");
    debug_put_hex32((uint32_t)callback);
    debug_put_newline();
    debug_flush();

    soft_serial_rx_callback = callback;
}

void soft_serial_process_startbit(void) {
    // debugging
    debug_putc('>');

    // this is the startbit -> re synchronize the timer to this
    // by setting the next cc interrupt to 1/2 bit length:
    HAL_SOFT_SERIAL_UPDATE_TOP_VALUE(HAL_SOFTSERIAL_BIT_DURATION_TICKS / 2);

    // reset incoming bit data:
    soft_serial_databits = 0;
    // sample 8N1 -> 1 start-, 8 data-, and 1 stop-bit
    soft_serial_databit_count = 1+8+1;
}

uint8_t soft_serial_process_databit(void) {
    // handle data
    if (soft_serial_databit_count != 0){
        // sample bits
        #if HUB_TELEMETRY_INVERTED
        if (HAL_SOFT_SERIAL_PIN_LO()){
        #else
        if (HAL_SOFT_SERIAL_PIN_HI()){
        #endif
            soft_serial_databits |= 1;
        }
        soft_serial_databits <<= 1;
        soft_serial_databit_count--;
    } else {
        // process incoming data byte
        if ((soft_serial_databits & (1<<9)) != 0){
            // FRAME ERROR: start bit invalid
            debug_putc('S'); 
        }else if ((soft_serial_databits & (1<<0)) != 1){
            // FRAME ERROR: stop bit invalid
            debug_putc('s');
        }else{
            // fine, data byte received
            uint8_t data_byte;
            data_byte = (soft_serial_databits >> 1) & 0xFF;
            debug("RX: 0x"); debug_put_hex8(data_byte); debug_put_newline();

            // process data
            if (soft_serial_rx_callback != 0) {
                // execute callback
                soft_serial_rx_callback(data_byte); 
            }

            // finished data byte
            return 1;
        }
    }

    // not yet finished
    return 0;
}
