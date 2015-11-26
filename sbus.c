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

#include "main.h"
#include "config.h"
#include "debug.h"
#include "wdt.h"
#include "dma.h"
#include "delay.h"
#include "sbus.h"
#include "failsafe.h"

__xdata uint16_t sbus_data[SBUS_DATA_LEN];


void sbus_init(void){
    __xdata union uart_config_t sbus_uart_config;

    debug("sbus: init\n"); debug_flush();

    //we will use SERVO_4 as sbus output:
    //therefore we configure
    //USART1 use ALT1 -> Clear flag -> Port P0_4 = TX
    PERCFG &= ~(PERCFG_U1CFG);

    //configure pin P0_4 (TX) as output:
    P0SEL |= (1<<4);

    //make tx pin output:
    P0DIR |= (1<<4);

    //this assumes cpu runs from XOSC (26mhz) !
    //set baudrate 1mbit -> BAUD_E = 19, BAUD_M = 0
    U1BAUD = SBUS_BAUD_M;
    U1GCR = (U0GCR & ~0x1F) | (SBUS_BAUD_E);

    //set up config
    //8N1, strange, some doc says 8E2??
    sbus_uart_config.bit.START = 0; //startbit level = low
    sbus_uart_config.bit.STOP  = 1; //stopbit level = high
    sbus_uart_config.bit.SPB   = 0; //1 stopbit
    sbus_uart_config.bit.PARITY = 0; //no parity
    sbus_uart_config.bit.D9     = 0; //8 Bits
    sbus_uart_config.bit.FLOW   = 0; //no hw flow control
    sbus_uart_config.bit.ORDER  = 0; //lsb first
    uart_set_mode(&sbus_uart_config);

    //use dma channel 3 for transmission:
    dma_config[3].PRIORITY       = DMA_PRI_LOW;
    dma_config[3].M8             = DMA_M8_USE_7_BITS;
    dma_config[3].IRQMASK        = DMA_IRQMASK_DISABLE;
    dma_config[3].TRIG           = DMA_TRIG_UTX1;
    dma_config[3].TMODE          = DMA_TMODE_SINGLE;
    dma_config[3].WORDSIZE       = DMA_WORDSIZE_BYTE;

    //important: src addr start is sbus_data[1] as we
    //initiate the transfer by manually sending sbus_data[0]!
    SET_WORD(dma_config[3].SRCADDRH,  dma_config[3].SRCADDRL,  &sbus_data[1]);
    SET_WORD(dma_config[3].DESTADDRH, dma_config[3].DESTADDRL, &X_U1DBUF);
    dma_config[3].VLEN           = DMA_VLEN_USE_LEN;

    SET_WORD(dma_config[3].LENH, dma_config[3].LENL, SBUS_DATA_LEN);
    dma_config[3].SRCINC         = DMA_SRCINC_1;
    dma_config[3].DESTINC        = DMA_DESTINC_0;

    //set pointer to the DMA configuration struct into DMA-channel 1-4
    //configuration, should have happened in adc.c already...
    SET_WORD(DMA1CFGH, DMA1CFGL, &dma_config[1]);

    //arm the relevant DMA channel for UART TX, and apply 45 NOP's
    //to allow the DMA configuration to load
    //-> do a sleep instead of those nops...
    DMAARM |= (1<<DMA_ARM_CH3);
    delay_us(100);

    //start in failsafe mode:
    failsafe_enter();

    debug("sbus: init done\n"); debug_flush();
}

void sbus_start_transmission(){
    //send the very first UART byte to trigger a UART TX session:
    U1DBUF = sbus_data[0];
}


void sbus_update(__xdata uint16_t *data){
/*

    sbus[0]=SBUS_SYNCBYTE;
    sbus[1]=lowByte(channel[0]);//
    sbus[2]=highByte(channel[0]) | lowByte(channel[1])<<3;
    sbus[3]=(channel[1]>>5)|(channel[2]<<6);//
    sbus[4]=(channel[2]>>2)& 0xff;
    sbus[5]=(channel[2]>>10)|lowByte(channel[3])<<1;
    sbus[6]=(channel[3]>>7)|lowByte(channel[4])<<4;
    sbus[7]=(channel[4]>>4)|lowByte(channel[5])<<7;
    sbus[8]=(channel[5]>>1)& 0xff;
    sbus[9]=(channel[5]>>9)|lowByte(channel[6])<<2;
    sbus[10]=(channel[6]>>6)|lowByte(channel[7])<<3;
    sbus[11]=(channel[7]>>3)& 0xff;
    //
    sbus[12]=lowByte(channel[8]);
    sbus[13]=highByte(channel[8]) | lowByte(channel[9])<<3;
    sbus[14]=(channel[9]>>5)|(channel[10]<<6);
    sbus[15]=(channel[10]>>2)& 0xff;
    sbus[16]=(channel[10]>>10)|lowByte(channel[11])<<1;
    sbus[17]=(channel[11]>>7)|lowByte(channel[12])<<4;
    sbus[18]=(channel[12]>>4)|lowByte(channel[13])<<7;
    sbus[19]=(channel[13]>>1)& 0xff;
    sbus[20]=(channel[13]>>9)|lowByte(channel[14])<<2;
    sbus[21]=(channel[14]>>6)|lowByte(channel[15])<<3;
    sbus[22]=(channel[15]>>3)& 0xff;//end
     //Flags sbus[23]
     //bit 7=ch17(0x80)
     //bit 6=ch18(0x40)
    //bit 5=frame lost(0x20)
    //bit4=failsafe activated(0x10)
     //bit3=birt2=bit1=bit0=n/a
    //sbus[23]=0xC0;// 11000000/FS 11010000
    sbus[23]=0x10;// 11000000/FS 11010000
    sbus[24]=SBUS_ENDBYTE;//endbyte


*/
    //exit failsafe mode
    failsafe_exit();
}

void sbus_exit_failsafe(void){
    debug("sbus: exit FS\n");

}

void sbus_enter_failsafe(void){
    //failsafe is active
    debug("sbus: entered FS\n");
}





