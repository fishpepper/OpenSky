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

// see USART app note
// http://www.ti.com/lit/an/swra222b/swra222b.pdf
#include "uart.h"
#include "main.h"
#include "delay.h"
#include "led.h"
#include "debug.h"

/*#if DEBUG
NO! DO NOT USE PRINTF! (long runtimes etc)
//helper for printf
void putchar(char c){
    uart_putc(c);
}
#endif*/

//use 155200 baud
#define UART_BAUD_M CC2510_BAUD_M_115200
#define UART_BAUD_E CC2510_BAUD_E_115200

__xdata uint8_t uart_tx_buffer[UART_TX_BUFFER_SIZE];
__xdata volatile uint8_t uart_tx_buffer_in;
__xdata volatile uint8_t uart_tx_buffer_out;
__xdata union uart_config_t uart_config;

void uart_init(void){
    uart_select_port();
    uart_set_baudrate();

    //set up config
    uart_config.bit.START = 0; //startbit level = low
    uart_config.bit.STOP  = 1; //stopbit level = high
    uart_config.bit.SPB   = 0; //1 stopbit
    uart_config.bit.PARITY = 0; //no parity
    uart_config.bit.D9     = 0; //8 Bits
    uart_config.bit.FLOW   = 0; //no hw flow control
    uart_config.bit.ORDER  = 0; //lsb first
    uart_set_mode();

    //init tx buffer
    uart_tx_buffer_in = 0;
    uart_tx_buffer_out = 0;
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

    cli();

    if (IEN2 & IEN2_UTX0IE){
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

        //clear flags
        UTX0IF = 0;
        U0CSR &= ~U0CSR_TX_BYTE;

        //enable TX int:
        IEN2 |= (IEN2_UTX0IE);

        //send this char
        U0DBUF = ch;
    }

    sei();
}

void uart_flush(void){
    //wait until uart buffer is empty
    //once TX INT is disabled our buffer is empty again
    while (IEN2 & IEN2_UTX0IE){}
}

void uart_tx_interrupt(void) __interrupt UTX0_VECTOR{
    //clear tx int flag
    UTX0IF = 0;

    //finished with sending?
    if(uart_tx_buffer_in == uart_tx_buffer_out ){
            //no data in fifo -> disable tx int:
            IEN2 &= ~(IEN2_UTX0IE);
            return;
    }

    //else: data to tx
    U0DBUF = uart_tx_buffer[uart_tx_buffer_out];

    //handle out pointer
    uart_tx_buffer_out = (uart_tx_buffer_out+1) & UART_TX_BUFFER_AND_OPERAND;
}


void uart_set_mode(void){ //struct uart_mode_config_t *cfg){
    //enable uart mode
    U0CSR |= 0x80;

    //store config to U0UCR register
    U0UCR = uart_config.byte & (0x7F);

    //store config to U0GCR: (msb/lsb)
    if (uart_config.bit.ORDER){
        U0GCR |= U0GCR_ORDER;
    }else{
        U0GCR &= ~U0GCR_ORDER;
    }

    //interrupt prio to 1 (0..3=highest)
    IP0 |= (1<<3);
    IP1 &= ~(1<<3);
}

void uart_set_baudrate(void){
    //this assumes cpu runs from XOSC (26mhz) !
    //set baudrate
    U0BAUD = UART_BAUD_M;
    U0GCR = (U0GCR & ~0x1F) | (UART_BAUD_E);
}

void uart_select_port(void){
    //we will use SERVO_5 as tx output:
    //therefore we configure
    //USART0 use ALT1 -> Clear flag -> Port P0 = TX
    PERCFG &= ~(PERCFG_U0CFG);
    //USART1 use ALT2
    PERCFG |= PERCFG_U1CFG;

    //configure pin P0_3 (TX) as output:
    P0SEL |= (1<<3);

    //make sure all P1 pins switch to normal GPIO
    P1SEL &= ~(0x3C);

    //make tx pin output:
    P0DIR |= (1<<3);
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
