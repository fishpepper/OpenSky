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
#include "hal_defines.h"

void hal_uart_init(void) {
    __xdata union hal_uart_config_t uart_config;

    //we will use SERVO_5 as tx output:
    //therefore we configure
    //USART0 use ALT1 -> Clear flag -> Port P0 = TX
    PERCFG &= ~(PERCFG_U0CFG);

    //configure pin P0_3 (TX) as output:
    P0SEL |= (1<<3);

    //make sure all P1 pins switch to normal GPIO
    P1SEL &= ~(0x3C);

    //make tx pin output:
    P0DIR |= (1<<3);

    //this assumes cpu runs from XOSC (26mhz) !
    //set baudrate
    U0BAUD = UART_BAUD_M;
    U0GCR = (U0GCR & ~0x1F) | (UART_BAUD_E);

    //set up config
    uart_config.bit.START = 0; //startbit level = low
    uart_config.bit.STOP  = 1; //stopbit level = high
    uart_config.bit.SPB   = 0; //1 stopbit
    uart_config.bit.PARITY = 0; //no parity
    uart_config.bit.BIT9   = 0; //8bit
    uart_config.bit.D9     = 0; //8 Bits
    uart_config.bit.FLOW   = 0; //no hw flow control
    uart_config.bit.ORDER  = 0; //lsb first
    hal_uart_set_mode(&uart_config);


    //enable interrupts:
    sei();
}

void hal_uart_start_transmission(uint8_t ch){
    //clear flags
    UTX0IF = 0;
    U0CSR &= ~U0CSR_TX_BYTE;

    //enable TX int:
    IEN2 |= (IEN2_UTX0IE);

    //send this char
    U0DBUF = ch;
}

static void hal_uart_set_mode(__xdata union hal_uart_config_t *cfg){
    //enable uart mode
    U0CSR |= 0x80;

    //store config to U0UCR register
    U0UCR = cfg->byte & (0x7F);

    //store config to U0GCR: (msb/lsb)
    if (cfg->bit.ORDER){
        U0GCR |= U0GCR_ORDER;
    }else{
        U0GCR &= ~U0GCR_ORDER;
    }

    //interrupt prio to 01 (0..3=highest)
    IP0 |=  (1<<2);
    IP1 &= ~(1<<2);
}


