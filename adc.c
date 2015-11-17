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
#include <cc2510fx.h>
#include "adc.h"
#include "debug.h"
#include "config.h"
#include "dma.h"
#include "delay.h"
#include "wdt.h"


//adc results
__xdata uint16_t adc_data[2];


void adc_init(void){
    debug("adc: init\n"); debug_flush();

    adc_data[0] = 0;
    adc_data[1] = 0;

    //pin config -> dir = input
    P0DIR &= ~((1<<ADC1) | (1<<ADC0));

    //set special function ADC for those pins:
    ADCCFG = (1<<ADC1) | (1<<ADC0);

    //set up adc:
    //- external vref (avcc)
    //- 10bit adc
    //- stop on AIN7
    ADCCON2 = ADCCON2_SREF_AVDD | ADCCON2_SDIV_10BIT | ADCCON2_SCH_AIN7;

    //full speed, start conversion (bit0+1 always write as 1...)
    ADCCON1 = ADCCON1_ST | ADCCON1_STSEL_FULL_SPEED | 0b11;

    //configure DMA1 + DMA2:
    adc_dma_init(1, &adc_data[0], DMA_TRIG_ADC_CH5);
    adc_dma_init(2, &adc_data[1], DMA_TRIG_ADC_CH6);

    //set pointer to the DMA configuration struct into DMA-channel 1-4
    //configuration
    SET_WORD(DMA1CFGH, DMA1CFGL, &dma_config[1]);

    adc_arm_dma();

    //for testing only, do not use under normal use
    //adc_test();
}

void adc_arm_dma(void){
    DMAARM = (DMA_ARM_CH1 | DMA_ARM_CH2);
}

void adc_process(void){
    if (adc_dma_done()){
        //THIS OUTPUT BREAKS CONNECTIVITY! USE FOR DEBUGGING ONLY.
        //fine, arm dma:
        adc_arm_dma();
    }else{
        //oops this should not happen
        debug_putc('D');
        //cancel and re arm dma
        //DMAARM = DMA_ARM_ABORT | (DMA_ARM_CH1 | DMA_ARM_CH2);
    }
}


void adc_dma_init(uint8_t dma_id, uint16_t __xdata *dest_adr, uint8_t trig){
    dma_config[dma_id].PRIORITY       = DMA_PRI_LOW; //example used high...
    dma_config[dma_id].M8             = DMA_M8_USE_7_BITS;
    dma_config[dma_id].IRQMASK        = DMA_IRQMASK_DISABLE;
    dma_config[dma_id].TRIG           = trig;
    dma_config[dma_id].TMODE          = DMA_TMODE_BLOCK;
    dma_config[dma_id].WORDSIZE       = DMA_WORDSIZE_BYTE;

    SET_WORD(dma_config[dma_id].SRCADDRH,  dma_config[dma_id].SRCADDRL,  &X_ADCL);
    SET_WORD(dma_config[dma_id].DESTADDRH, dma_config[dma_id].DESTADDRL, dest_adr);
    dma_config[dma_id].VLEN           = DMA_VLEN_USE_LEN;

    SET_WORD(dma_config[dma_id].LENH, dma_config[dma_id].LENL, 2);
    dma_config[dma_id].SRCINC         = DMA_SRCINC_1;
    dma_config[dma_id].DESTINC        = DMA_DESTINC_1;
}

uint8_t adc_dma_done(void){
    return ((DMAIRQ & (DMA_ARM_CH1 | DMA_ARM_CH2)) == (DMA_ARM_CH1 | DMA_ARM_CH2));
}

uint8_t adc_get_scaled(uint8_t ch){
    //adc data is HHHHHHHHLLLL0000 -> shift >>6 to get 10bit
    //shift by >>6 and shift by 2 to get 8 bit
    //now comes the strange part! this seems to be wrong
    //shifting by 6+1 does work and delivers the right values
    //i have no clue whats wrong here... or is 10bit res not 10bit values?!
    if (ch == 0){
        //convert to 8 bit (see above)
        return adc_data[1]>>7;
    }else{
        #if ADC1_USE_ACS712
        //acs712 is connected to ADC1
        //when powered by 5V we can use a trick
        //to get a good resolution:
        //use inverted power inputs to get
        // 0A = 2.5V
        //30A = 0.0V
        return 255-(adc_data[0]>>7);
        #else
        return adc_data[0]>>7;
        #endif
    }
}

void adc_test(void){
    debug("adc: running test\n"); debug_flush();

    while(1){
        debug("adc: re-arming adc\n"); debug_flush();
        adc_arm_dma();

        debug("adc: waiting for adc completion\n"); debug_flush();
        while(!adc_dma_done()){
            debug_putc('.');
            delay_ms(1);
        }
        debug("\nadc: done. res[0] = "); debug_flush();

        debug_put_uint16(adc_data[0]>>4);
        debug_putc('x'); debug_put_hex8(adc_data[0]>>12); debug_put_hex8((adc_data[0]>>4)&0xff);

        debug(", res[1] = "); debug_flush();

        debug_put_uint16(adc_data[1]>>4);
        debug_putc('x'); debug_put_hex8(adc_data[1]>>12); debug_put_hex8((adc_data[1]>>4)&0xff);

        debug_put_newline();

        delay_ms(100);

        //reset wdt
        wdt_reset();
    }


}
