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
#include "ppm.h"
#include "uart.h"
#include "failsafe.h"

#if SBUS_ENABLED

__xdata uint8_t sbus_data[SBUS_DATA_LEN];

//SBUS is:
//100000bps inverted serial stream, 8 bits, even parity, 2 stop bits
//frame period is 7ms (HS) or 14ms (FS)

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
    //see sbus.h for calc and defines
    U1BAUD = SBUS_BAUD_M;
    U1GCR = (U1GCR & ~0x1F) | (SBUS_BAUD_E);

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
    sbus_uart_set_mode(&sbus_uart_config);

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

    //transfer SBUS_DATA_LEN-1 bytes (first byte is transmitted on start of transmission)
    SET_WORD(dma_config[3].LENH, dma_config[3].LENL, SBUS_DATA_LEN-1);
    dma_config[3].SRCINC         = DMA_SRCINC_1;
    dma_config[3].DESTINC        = DMA_DESTINC_0;

    //set pointer to the DMA configuration struct into DMA-channel 1-4
    //configuration, should have happened in adc.c already...
    SET_WORD(DMA1CFGH, DMA1CFGL, &dma_config[1]);

    //arm the relevant DMA channel for UART TX, and apply 45 NOP's
    //to allow the DMA configuration to load
    //-> do a sleep instead of those nops...
    DMAARM |= DMA_ARM_CH3;
    delay_us(100);

    //start in failsafe mode:
    failsafe_enter();

    debug("sbus: init done\n"); debug_flush();
}

void sbus_uart_set_mode(__xdata union uart_config_t *cfg){
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

void sbus_start_transmission(uint8_t frame_lost){
    uint8_t tmp;
    //debug("sbus: TX\n");

    //set up flags:
    //bit 7 = discrete channel 17
    //bit 6 = discrete channel 18
    //bit 5 = frame lost
    //bit 4 = Failsafe
    //failsafe + frame lost will be set below,
    //discrete channels are zero
    tmp = 0x00;

    //failsafe active?
    if (failsafe_active){
        //clear failsafe flag:
        tmp |= SBUS_FLAG_FAILSAFE_ACTIVE;
    }

    //check if this frame was lost
    if (frame_lost == SBUS_FRAME_LOST){
        tmp |= SBUS_FLAG_FRAME_LOST;
    }

    //copy flags to buffer
    sbus_data[23] = SBUS_PREPARE_DATA( tmp );

    //time to send this frame!
    //re-arm dma:
    DMAARM |= DMA_ARM_CH3;

    //send the very first UART byte to trigger a UART TX session:
    U1DBUF = sbus_data[0];
}


void sbus_update(__xdata uint16_t *data){
    uint8_t i;
    __xdata uint16_t rescaled_data[8];
    int16_t tmp;

    //rescale input data:
    //frsky input is us*1.5
    //under normal conditions this is ~1480...3020
    //when tx is set to 125% this is  ~1290...3210
    //remap this to 0...2047 -> first substract 1290
    //center was at 2250us, is now at 960, remap this to ~1023:
    //1023/960 = 1,065625.. = approx by 17/16 = 1 1/16
    //sbus data = (input-1290)*(1 1/16) = (input-1290) + (input-1290)/16
    for(i=0; i<8; i++){
        tmp = data[i] - 1290;
        tmp = tmp + (tmp>>4);
        if (tmp < 0){
            rescaled_data[i] = 0;
        }else if(tmp > 2047){
            rescaled_data[i] = 2047;
        }else{
            rescaled_data[i] = tmp;
        }
    }

    //sbus transmits up to 16 channels with 11bit each.
    //build up channel data frame:
    sbus_data[ 0] = SBUS_PREPARE_DATA( SBUS_SYNCBYTE );

    //bits ch 0000 0000
    sbus_data[ 1] = SBUS_PREPARE_DATA( LO(rescaled_data[0]) );
    //bits ch 1111 1000
    sbus_data[ 2] = SBUS_PREPARE_DATA( (LO(rescaled_data[1])<<3) | HI(rescaled_data[0]) );
    //bits ch 2211 1111
    sbus_data[ 3] = SBUS_PREPARE_DATA( (rescaled_data[1]>>5) | (rescaled_data[2]<<6) );
    //bits ch 2222 2222
    sbus_data[ 4] = SBUS_PREPARE_DATA( (rescaled_data[2]>>2) & 0xFF );
    //bits ch 3333 3332
    sbus_data[ 5] = SBUS_PREPARE_DATA( (rescaled_data[2]>>10) | (LO(rescaled_data[3])<<1) );
    //bits ch 4444 3333
    sbus_data[ 6] = SBUS_PREPARE_DATA( (rescaled_data[3]>>7) | (LO(rescaled_data[4])<<4) );
    //bits ch 5444 4444
    sbus_data[ 7] = SBUS_PREPARE_DATA( (rescaled_data[4]>>4) | (LO(rescaled_data[5])<<7) );
    //bits ch 5555 5555
    sbus_data[ 8] = SBUS_PREPARE_DATA( (rescaled_data[5]>>1) & 0xFF );
    //bits ch 6666 6655
    sbus_data[ 9] = SBUS_PREPARE_DATA( (rescaled_data[5]>>9) | (LO(rescaled_data[6])<<2) );
    //bits ch 7776 6666
    sbus_data[10] = SBUS_PREPARE_DATA( (rescaled_data[6]>>6) | (LO(rescaled_data[7])<<5) );
    //bits ch 7777 7777
    sbus_data[11] = SBUS_PREPARE_DATA( (rescaled_data[7]>>3) & 0xFF );
    //ch8-ch15 = zero
    for(i=12; i<23; i++){
        sbus_data[i] = SBUS_PREPARE_DATA( 0x00 );
    }
    //sbus flags, will be set by start transmission...
    sbus_data[23] = SBUS_PREPARE_DATA( 0x00 );

    //EOF frame:
    sbus_data[24] = SBUS_PREPARE_DATA( SBUS_ENDBYTE );
}

void sbus_exit_failsafe(void){
    debug("sbus: exit FS\n");

}

void sbus_enter_failsafe(void){
    //failsafe is active
    debug("sbus: entered FS\n");
}

#endif



