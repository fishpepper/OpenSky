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

#include "hal_debug.h"
#include "hal_defines.h"

void hal_debug_init(void) {
    __xdata union hal_uart_config_t uart_config;

#ifndef DEBUG_UART
#error "ERROR: DEBUG_UART not defined"
#endif

#if DEBUG_UART == USART0_P0
    //USART0 use ALT1 -> Clear flag -> Port P0_3 = TX
    PERCFG &= ~(PERCFG_U0CFG);

    //configure pin P0_3 (TX) as output:
    P0SEL |= (1<<3);

    //make sure all P1 pins switch to normal GPIO
//    P1SEL &= ~(0x3C);

    //make tx pin output:
    P0DIR |= (1<<3);
#elif DEBUG_UART == USART0_P1
    //USART0 use ALT2 -> Set flag -> Port P1_5 = TX
    PERCFG |= (PERCFG_U0CFG);

    //configure pin P1_5 (TX) as output:
    P1SEL |= (1<<5);

    //make tx pin output:
    P1DIR |= (1<<5);
#elif DEBUG_UART == USART1_P0
    //USART1 use ALT1 -> Clear flag -> Port P0_4 = TX
    PERCFG &= ~(PERCFG_U1CFG);

    // USART1 has priority when USART0 is also enabled
    P2DIR = (P2DIR & 0x3F) | 0b01000000;

    //configure pin P0_4 (TX) and P0_5 (RX) as special function:
    P0SEL |= (1<<4) | (1<<5);

    // make sure all P1 pins switch to normal GPIO
    //P1SEL &= ~(0xF0);

    //make tx pin output:
    P0DIR |= (1<<4);
#else
  #error "ERROR: UNSUPPORTED DEBUG UART"
#endif

    //this assumes cpu runs from XOSC (26mhz) !
    //set baudrate
#if (DEBUG_UART == USART0_P0) || (DEBUG_UART == USART0_P1)
    U0BAUD = CC2510_BAUD_M_115200;
    U0GCR = (U0GCR & ~0x1F) | (CC2510_BAUD_E_115200);
#else
    U1BAUD = CC2510_BAUD_M_115200;
    U1GCR = (U1GCR & ~0x1F) | (CC2510_BAUD_E_115200);
#endif

    //set up config
    uart_config.bit.START = 0; //startbit level = low
    uart_config.bit.STOP  = 1; //stopbit level = high
    uart_config.bit.SPB   = 0; //1 stopbit
    uart_config.bit.PARITY = 0; //no parity
    uart_config.bit.BIT9   = 0; //8bit
    uart_config.bit.D9     = 0; //8 Bits
    uart_config.bit.FLOW   = 0; //no hw flow control
    uart_config.bit.ORDER  = 0; //lsb first
    hal_debug_set_mode(&uart_config);

    //enable interrupts:
    sei();
}

static void hal_debug_set_mode(EXTERNAL_MEMORY union hal_uart_config_t *cfg){
#if (DEBUG_UART == USART0_P0) || (DEBUG_UART == USART0_P1)
    //enable uart mode
    U0CSR |= 0x80;

    //store config to UxUCR register
    U0UCR = cfg->byte & (0x7F);

    //store config to U1GCR: (msb/lsb)
    if (cfg->bit.ORDER){
        U0GCR |= U0GCR_ORDER;
    }else{
        U0GCR &= ~U0GCR_ORDER;
    }

    //interrupt prio to 0 (0..3=highest)
    IP0 &= ~(1<<2);
    IP1 &= ~(1<<2);
#else
    //enable uart mode
    U1CSR |= 0x80;

    //store config to UxUCR register
    U1UCR = cfg->byte & (0x7F);

    //store config to U1GCR: (msb/lsb)
    if (cfg->bit.ORDER){
        U1GCR |= U1GCR_ORDER;
    }else{
        U1GCR &= ~U1GCR_ORDER;
    }
    //interrupt prio to 0 (0..3=highest)
    IP0 &= ~(1<<3);
    IP1 &= ~(1<<3);
#endif
}


void hal_debug_start_transmission(uint8_t ch){
#if (DEBUG_UART == USART0_P0) || (DEBUG_UART == USART0_P1)
    //clear flags
    UTX0IF = 0;
    U0CSR &= ~UxCSR_TX_BYTE;

    //enable TX int:
    IEN2 |= (IEN2_UTX0IE);

    //send this char
    U0DBUF = ch;
#else
    //clear flags
    UTX1IF = 0;
    U1CSR &= ~UxCSR_TX_BYTE;

    //enable TX int:
    IEN2 |= (IEN2_UTX1IE);

    //send this char
    U1DBUF = ch;
#endif
}


