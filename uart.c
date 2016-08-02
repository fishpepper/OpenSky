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

#include "hal_uart.h"
#include "uart.h"
#include "debug.h"
#include "delay.h"
#include <stdint.h>
#include "hal_defines.h"

EXTERNAL_MEMORY uint8_t uart_tx_buffer[UART_TX_BUFFER_SIZE];
EXTERNAL_MEMORY volatile uint8_t uart_tx_buffer_in;
EXTERNAL_MEMORY volatile uint8_t uart_tx_buffer_out;

#define DEBUG_DEFINE_TO_STR(x) #x
#define DEBUG_DEFINE_TO_STR_VAL(x) DEBUG_DEFINE_TO_STR(x)

void uart_init(void){
    hal_uart_init();

    uart_tx_buffer_in = 0;
    uart_tx_buffer_out = 0;

    //wait some time for uart to become stable
    delay_us(100);
    debug_put_newline();
    debug("### OpenSky - ");
    debug(DEBUG_DEFINE_TO_STR_VAL(BUILD_TARGET));
    debug(" - (c) by github.com/fishpepper ###\n"); debug_flush();
    debug("uart: init done\n");
}

void uart_test(void){
    debug("uart: running test\n"); debug_flush();
    while(1){
        debug("TEST12345\n");
        delay_ms(500);
    }
}

void uart_putc(uint8_t ch){
    //add \r to newlines
    if (ch == '\n') uart_putc('\r');

    //disable interrupts
    hal_uart_int_enable(0);

    if (hal_uart_int_enabled()){
        //int already active, copy to buffer!
        uart_tx_buffer[uart_tx_buffer_in] = ch;
        uart_tx_buffer_in = (uart_tx_buffer_in + 1) & UART_TX_BUFFER_AND_OPERAND;

        //check if free space in buffer:
        if (uart_tx_buffer_in == uart_tx_buffer_out){
            //no more space in buffer! this will loose some data!
            //add LOST data tag (for visual debugging lost data)
            uart_tx_buffer[(uart_tx_buffer_in-1) & UART_TX_BUFFER_AND_OPERAND] = '$';

            /*LED_RED_ON();
            LED_GREEN_OFF();
            while(1){
                LED_RED_ON();
                LED_GREEN_ON();
                delay_ms(200);
                LED_RED_OFF();
                LED_GREEN_OFF();
                delay_ms(200);
            }*/
            return;
        }
    }else{
        //no int active. send first byte and reset buffer indices
        uart_tx_buffer_in  = uart_tx_buffer_out;

        hal_uart_start_transmission(ch);
    }

    //re enable interrupts
    hal_uart_int_enable(1);
}

void uart_flush(void){
    //wait until uart buffer is empty
    //once TX INT is disabled our buffer is empty again
    while(hal_uart_int_enabled()) {}
}


void uart_puts(uint8_t *data){
    uint8_t c = *data++;
    while(c){
        uart_putc(c);
        c = *data++;
    }
}


//put hexadecimal number to debug out.
void uart_put_hex8(uint8_t val){
    uint8_t lo = val&0x0F;
    uint8_t hi = val>>4;
    if (hi<0x0A){
        hi = '0' + hi;
    }else{
        hi = 'A' - 0x0A + hi;
    }

    if (lo<0x0A){
        lo = '0' + lo;
    }else{
        lo = 'A' - 0x0A + lo;
    }
    uart_putc(hi);
    uart_putc(lo);
}

//output a signed 8-bit number to uart
void uart_put_int8(int8_t c){
    uint8_t tmp;
    uint8_t mul;
    uint8_t l;
    uint8_t uint_s;

    if (c < 0){
        uart_putc('-');
        uint_s = -c;
    }else{
        uint_s = c;
    }

    l=0;
    for(mul=100; mul>0; mul = mul/10){
        tmp = '0';
        while(uint_s>=mul){
            uint_s -= mul;
            tmp++;
            l=1;
        }
        if ((l==0) && (tmp == '0') && (mul!=1)){
            //dont print spacer
            //uart_putc(' ');
        }else{
            uart_putc(tmp);
        }
    }
}

//output an unsigned 8-bit number to uart
void uart_put_uint8(uint8_t c){
    uint8_t tmp;
    uint8_t mul;
    uint8_t l;

    l=0;
    for(mul=100; mul>0; mul = mul/10){
        tmp = '0';
        while(c>=mul){
            c -= mul;
            tmp++;
            l=1;
        }
        if ((l==0) && (tmp == '0') && (mul!=1)){
            //dont print spacer
            //uart_putc(' ');
        }else{
            uart_putc(tmp);
        }
    }
}

//output an unsigned 16-bit number to uart
//FIXME: this routine is somewhat buggy when used with the ISR fifo uart_putc
//       it is still unclear why this is causing problems?!
void uart_put_uint16(uint16_t c){
    uint8_t tmp;
    uint8_t l=0;
    //loop unrolling is better (no int16 arithmetic)
    /*for(mul=10000; mul>0; mul = mul/10){
        uint16_t mul;

        l=0;
                tmp = '0';
                while(c>=mul){
                        c -= mul;
                        tmp++;
                        l=1;
                }
                if ((l==0) && (tmp == '0') && (mul!=1)){
                        //uart_putc(' ');
                }else{
                        uart_putc(tmp);
                }
        }*/
    tmp = 0;
    while(c>=10000L){
        c-=10000L;
        tmp++;
        l=1;
    }
    if (tmp != 0) uart_putc('0' + tmp);

    tmp = 0;
    while(c>=1000L){
        c-=1000L;
        tmp++;
        l=1;
    }
    if (l || (tmp != 0)) uart_putc('0' + tmp);

    tmp = 0;
    while(c>=100){
        c-=100;
        tmp++;
        l=1;
    }
    if (l || (tmp != 0)) uart_putc('0' + tmp);

    tmp = 0;
    while(c>=10){
        c-=10;
        tmp++;
        l=1;
    }
    if (l || (tmp != 0)) uart_putc('0' + tmp);

    uart_putc('0' + (uint8_t)c);
}

void uart_put_newline(void){
    uart_putc('\n');
}
