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

#include "hal_debug.h"
#include "debug.h"
#include "delay.h"
#include "main.h"
#include <stdint.h>
#include "hal_defines.h"

#if DEBUG

EXTERNAL_MEMORY debug_buffer_t debug_buffer;
EXTERNAL_MEMORY uint8_t debug_init_done = 0;

void debug_init(void) {
    debug_uart_init();
    debug_init_done = 1;

    debug_put_newline();
    debug("### OpenSky - ");
    debug(DEFINE_TO_STR(BUILD_TARGET));
    debug(" - (c) by github.com/fishpepper ###\n"); debug_flush();
    debug("uart: init done\n");
}

void debug_uart_init(void) {
    hal_debug_init();

    debug_buffer.write = 0;
    debug_buffer.read = 0;

    // wait some time for uart to become stable
    delay_us(100);
}

void DEBUG_ISR(void) {
    if (HAL_DEBUG_ISR_FLAG_SET()) {
        // clear flag
        HAL_DEBUG_ISR_CLEAR_FLAG();

        // finished with sending?
        if (debug_buffer.read == debug_buffer.write) {
            // no data in fifo -> disable tx int:
            HAL_DEBUG_ISR_DISABLE();
            return;
        }

        // else: data to tx
        HAL_DEBUG_TX_DATA(debug_buffer.data[debug_buffer.read]);

        // handle out pointer
        debug_buffer.read = (debug_buffer.read+1) & DEBUG_TX_BUFFER_AND_OPERAND;
    }
}

void debug_uart_test(void) {
    debug("uart: running test\n"); debug_flush();
    while (1) {
        debug("TEST12345\n");
        delay_ms(500);
    }
}

void debug_putc(uint8_t ch) {
    // add \r to newlines
    if (ch == '\n') debug_putc('\r');

    // disable interrupts
    hal_debug_int_disable();

    if (hal_debug_int_enabled()) {
        // int already active, copy to buffer!
        debug_buffer.data[debug_buffer.write] = ch;
        debug_buffer.write = (debug_buffer.write + 1) & DEBUG_TX_BUFFER_AND_OPERAND;

        // check if free space in buffer:
        if (debug_buffer.write == debug_buffer.read) {
            // no more space in buffer! this will loose some data!
            // add LOST data tag (for visual debugging lost data)
            debug_buffer.data[(debug_buffer.write-1) & DEBUG_TX_BUFFER_AND_OPERAND] = '$';

            /*LED_RED_ON();
            LED_GREEN_OFF();
            while (1) {
                LED_RED_ON();
                LED_GREEN_ON();
                delay_ms(200);
                LED_RED_OFF();
                LED_GREEN_OFF();
                delay_ms(200);
            }*/
            return;
        }
    } else {
        // no int active. send first byte and reset buffer indices
        debug_buffer.write  = debug_buffer.read;

        hal_debug_start_transmission(ch);
    }

    // re enable interrupts
    hal_debug_int_enable();
}

void debug_flush(void) {
    if (!debug_init_done) {
        return;
    }
    // wait until uart buffer is empty
    // once TX INT is disabled our buffer is empty again
    while (hal_debug_int_enabled()) {}
}


void debug(uint8_t *data) {
    uint8_t c = *data++;
    while (c) {
        debug_putc(c);
        c = *data++;
    }
}


// put hexadecimal number to debug out.
void debug_put_hex8(uint8_t val) {
    uint8_t lo = val&0x0F;
    uint8_t hi = val >> 4;
    if (hi < 0x0A) {
        hi = '0' + hi;
    } else {
        hi = 'A' - 0x0A + hi;
    }

    if (lo < 0x0A) {
        lo = '0' + lo;
    } else {
        lo = 'A' - 0x0A + lo;
    }
    debug_putc(hi);
    debug_putc(lo);
}

// put 16bit hexadecimal number to debug out
void debug_put_hex16(uint16_t val) {
    debug_put_hex8(val>>8);
    debug_put_hex8(val & 0xFF);
}

// put 32bit hexadecimal number to debug out
void debug_put_hex32(uint32_t val) {
    debug_put_hex8(val>>24);
    debug_put_hex8(val>>16);
    debug_put_hex8(val>> 8);
    debug_put_hex8(val & 0xFF);
}

// output a signed 8-bit number to uart
void debug_put_int8(int8_t c) {
    uint8_t tmp;
    uint8_t mul;
    uint8_t l;
    uint8_t uint_s;

    if (c < 0) {
        debug_putc('-');
        uint_s = -c;
    } else {
        uint_s = c;
    }

    l = 0;
    for (mul = 100; mul > 0; mul = mul / 10) {
        tmp = '0';
        while (uint_s >= mul) {
            uint_s -= mul;
            tmp++;
            l = 1;
        }
        if ((l == 0) && (tmp == '0') && (mul != 1)) {
            // dont print spacer
            // debug_putc(' ');
        } else {
            debug_putc(tmp);
        }
    }
}

// output an unsigned 8-bit number to uart
void debug_put_uint8(uint8_t c) {
    uint8_t tmp;
    uint8_t mul;
    uint8_t l;

    l = 0;
    for (mul = 100; mul > 0; mul = mul / 10) {
        tmp = '0';
        while (c >= mul) {
            c -= mul;
            tmp++;
            l = 1;
        }
        if ((l == 0) && (tmp == '0') && (mul != 1)) {
            // dont print spacer
            // debug_putc(' ');
        } else {
            debug_putc(tmp);
        }
    }
}

// output an unsigned 16-bit number to uart
// FIXME: this routine is somewhat buggy when used with the ISR fifo debug_putc
//       it is still unclear why this is causing problems?!
void debug_put_uint16(uint16_t c) {
    uint8_t tmp;
    uint8_t l = 0;
    // loop unrolling is better (no int16 arithmetic)
    /*for (mul=10000; mul>0; mul = mul/10) {
        uint16_t mul;

        l=0;
                tmp = '0';
                while (c>=mul) {
                        c -= mul;
                        tmp++;
                        l=1;
                }
                if ((l==0) && (tmp == '0') && (mul!=1)) {
                        // debug_putc(' ');
                } else {
                        debug_putc(tmp);
                }
        }*/
    tmp = 0;
    while (c >= 10000L) {
        c -= 10000L;
        tmp++;
        l = 1;
    }
    if (tmp != 0) debug_putc('0' + tmp);

    tmp = 0;
    while (c >= 1000L) {
        c -= 1000L;
        tmp++;
        l = 1;
    }
    if (l || (tmp != 0)) debug_putc('0' + tmp);

    tmp = 0;
    while (c >= 100) {
        c -= 100;
        tmp++;
        l = 1;
    }
    if (l || (tmp != 0)) debug_putc('0' + tmp);

    tmp = 0;
    while (c >= 10) {
        c -= 10;
        tmp++;
        l = 1;
    }
    if (l || (tmp != 0)) debug_putc('0' + tmp);

    debug_putc('0' + (uint8_t)c);
}

void debug_put_newline(void) {
    debug_putc('\n');
}

#endif  // DEBUG
