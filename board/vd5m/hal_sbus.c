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

#include "hal_sbus.h"
#include "hal_dma.h"
#include "debug.h"
#include "delay.h"
#include "sbus.h"

#if SBUS_ENABLED

void hal_sbus_init(EXTERNAL_MEMORY uint8_t *sbus_data_ptr) {

    EXTERNAL_MEMORY union hal_uart_config_t sbus_uart_config;

    //we will use SERVO_4 as sbus output:
    //therefore we configure
    //USART1 use ALT1 -> Clear flag -> Port P0_4 = TX
    PERCFG &= ~(PERCFG_U1CFG);

    //configure pin P0_4 (TX) as output:
    P0SEL |= (1<<4);

    //make tx pin output:
    P0DIR |= (1<<4);

    //this assumes cpu runs from XOSC (26mhz) !
    //see sbus.h for calc and defines
    U1BAUD = HAL_SBUS_BAUD_M;
    U1GCR = (U1GCR & ~0x1F) | (HAL_SBUS_BAUD_E);

    //set up config for USART -> 8E2
    #if SBUS_INVERTED
        //this is a really nice feature of the cc2510:
        //we can invert the idle level of the usart
        //by setting STOP to zero. by inverting
        //the parity, the startbit, and the data
        //by using the SBUS_PREPARE_DATA() macro
        //we can effectively invert the usart in software :)
        sbus_uart_config.bit.START  = 1; //startbit level = low
        sbus_uart_config.bit.STOP   = 0; //stopbit level = high
        sbus_uart_config.bit.D9     = 1; //UNEven parity
    #else
        //standard usart, non-inverted mode
        //NOTE: most sbus implementations use inverted mode
        sbus_uart_config.bit.START  = 0; //startbit level = low
        sbus_uart_config.bit.STOP   = 1; //stopbit level = high
        sbus_uart_config.bit.D9     = 0; //Even parity
    #endif
    sbus_uart_config.bit.SPB    = 1; //1 = 2 stopbits
    sbus_uart_config.bit.PARITY = 1; //1 = parity enabled, D9=0 -> even parity
    sbus_uart_config.bit.BIT9   = 1; //8bit
    sbus_uart_config.bit.FLOW   = 0; //no hw flow control
    sbus_uart_config.bit.ORDER  = 0; //lsb first
    hal_sbus_uart_set_mode(&sbus_uart_config);

    //use dma channel 3 for transmission:
    hal_dma_config[3].PRIORITY       = DMA_PRI_LOW;
    hal_dma_config[3].M8             = DMA_M8_USE_7_BITS;
    hal_dma_config[3].IRQMASK        = DMA_IRQMASK_DISABLE;
    hal_dma_config[3].TRIG           = DMA_TRIG_UTX1;
    hal_dma_config[3].TMODE          = DMA_TMODE_SINGLE;
    hal_dma_config[3].WORDSIZE       = DMA_WORDSIZE_BYTE;

    //important: src addr start is sbus_data[1] as we
    //initiate the transfer by manually sending sbus_data[0]!
    SET_WORD(hal_dma_config[3].SRCADDRH,  hal_dma_config[3].SRCADDRL,  &sbus_data_ptr[1]);
    SET_WORD(hal_dma_config[3].DESTADDRH, hal_dma_config[3].DESTADDRL, &X_U1DBUF);
    hal_dma_config[3].VLEN           = DMA_VLEN_USE_LEN;

    //transfer SBUS_DATA_LEN-1 bytes (first byte is transmitted on start of transmission)
    SET_WORD(hal_dma_config[3].LENH, hal_dma_config[3].LENL, SBUS_DATA_LEN-1);
    hal_dma_config[3].SRCINC         = DMA_SRCINC_1;
    hal_dma_config[3].DESTINC        = DMA_DESTINC_0;

    //set pointer to the DMA configuration struct into DMA-channel 1-4
    //configuration, should have happened in adc.c already...
    SET_WORD(DMA1CFGH, DMA1CFGL, &hal_dma_config[1]);

    //arm the relevant DMA channel for UART TX, and apply 45 NOP's
    //to allow the DMA configuration to load
    //-> do a sleep instead of those nops...
    DMAARM |= DMA_ARM_CH3;
    delay_us(100);
}


void hal_sbus_uart_set_mode(EXTERNAL_MEMORY union hal_uart_config_t *cfg){
    //enable uart mode
    U1CSR |= 0x80;

    //store config to U1UCR register
    U1UCR = cfg->byte & (0x7F);

    //store config to U1GCR: (msb/lsb)
    if (cfg->bit.ORDER){
        U1GCR |= U1GCR_ORDER;
    }else{
        U1GCR &= ~U1GCR_ORDER;
    }

    //interrupt prio to 1 (0..3=highest)
    IP0 |= (1<<3);
    IP1 &= ~(1<<3);
}

void hal_sbus_start_transmission(uint8_t *data, uint8_t len){
    UNUSED(len);

    //time to send this frame!
    //re-arm dma:
    DMAARM |= DMA_ARM_CH3;

    //send the very first UART byte to trigger a UART TX session:
    U1DBUF = data[0];
}

#endif
