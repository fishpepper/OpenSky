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

#include "telemetry.h"
#include "soft_serial.h"
#include "uart.h"
#include "debug.h"
#include "wdt.h"

volatile EXTERNAL_MEMORY telemetry_buffer_t telemetry_buffer;
volatile EXTERNAL_MEMORY uint8_t telemetry_expected_id;

#ifndef SBUS_ENABLED 
  #ifdef HUB_TELEMETRY_ON_SBUS_UART
    #error ERROR: SBUS is not enabled, can not use/share port with telemery
  #endif
#endif

void telemetry_init(void) {
    debug("telemetry: init\n"); debug_flush();

    // mark as invalid
    telemetry_expected_id = 0;

    // setup buffer
    telemetry_buffer.write    = 0;
    telemetry_buffer.read     = 0;
    telemetry_buffer.read_ok  = 0;

#ifdef HUB_TELEMETRY_ON_SBUS_UART
    // re-use sbus uart, no init necessary
    debug("telemetry: using sbus uart\n"); debug_flush();
    // attach callback
    uart_set_rx_callback(&telemetry_rx_callback);
#else
    // init software serial port
    soft_serial_init();

    // attach callback
    soft_serial_set_rx_callback(&telemetry_rx_callback);
#endif

    #if TELEMETRY_DO_TEST
    telemetry_rx_echo_test();
    #endif
}


static void telemetry_rx_echo_test(void){
    // just for testing purposes...
    volatile EXTERNAL_MEMORY uint8_t data;

    while(1){
        wdt_reset();
        if (telemetry_pop(&data)){
            // debug_putc(data);
            debug_putc(' ');
            debug_put_hex8(data);
            if (data == 0x5E) debug_put_newline();
        }
    }
}


// hub telemetry input will be forwarded in frsky frames
// see http://www.rcgroups.com/forums/showthread.php?t=2547257 for a documentation
//
// buffer[0]  = bytes used
// buffer[1]  = last received telemetry id
// buffer[2]  = byte 1
// ...
// buffer[11] = byte 10
//
// NOTES:
// * buffer[ 0] corresponds to frsky_buffer[ 6]
// * buffer[11] corresponds to frsky_buffer[17]
// * not all 10 bytes has to be filled in, only the number of bytes that were received
//   will be sent in one frame
//
void telemetry_fill_buffer(volatile EXTERNAL_MEMORY uint8_t *buffer, uint8_t telemetry_id) {
    uint8_t telemetry_bytecount = 0;
    uint8_t i;

    if (telemetry_id == telemetry_expected_id) {
        // fine, last packet was received by rx
        // update read buffer:
        telemetry_buffer.read_ok = telemetry_buffer.read;
    } else {
        // rx re-requests last packet
        // update read pointer to last ack'ed packet
        telemetry_buffer.read = telemetry_buffer.read_ok;
    }


    // fetch all stored bytes (max 10)
    for(i=2; i<2+10; i++){
        if (!telemetry_pop(&buffer[i])){
            break;
        }
        telemetry_bytecount++;
    }

    // set up header
    buffer[0] = telemetry_bytecount;
    buffer[1] = telemetry_id;

    // next expected id = next id:
    telemetry_expected_id = (telemetry_id + 1) & 0x1F;
}


//fetch byte from buffer, returns 0 on no data available, 1 on success
uint8_t telemetry_pop(volatile EXTERNAL_MEMORY uint8_t *byte) {
    if (telemetry_buffer.read == telemetry_buffer.write){
        // no data available
        return 0;
    }

    // fetch byte
    *byte = telemetry_buffer.data[telemetry_buffer.read];
    telemetry_buffer.read = (telemetry_buffer.read + 1) & (TELEMETRY_BUFFER_SIZE-1);

    return 1;
}

void telemetry_rx_callback(uint8_t data) {
    uint8_t next;

    // push 1 byte into fifo:
    next = (telemetry_buffer.write + 1) & (TELEMETRY_BUFFER_SIZE-1);

    if (telemetry_buffer.read == next){
        // no more space in buffer - byte is discarded!
        return;
    }

    // space available -> add data byte
    telemetry_buffer.data[telemetry_buffer.write] = data;
    telemetry_buffer.write = next;
}
