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

#include "hal_adc.h"
#include "hal_defines.h"
#include "hal_cc25xx.h"
#include "portmacros.h"
#include "config.h"
#include "hal_dma.h"
#include "debug.h"
#include "delay.h"
#include "wdt.h"


// adc results
__xdata uint16_t hal_adc_data[2];

void hal_adc_init(void) {
    hal_adc_data[0] = 0;
    hal_adc_data[1] = 0;

    // pin config -> dir = input
    PORT2DIR(ADC_PORT) &= ~((1 << ADC1) | (1 << ADC0));

    // set special function ADC for those pins:
    ADCCFG = (1 << ADC1) | (1 << ADC0);

    // set up adc:
    // - external vref (avcc)
    // - 10bit adc
    // - stop on AIN7
    ADCCON2 = ADCCON2_SREF_AVDD | ADCCON2_SDIV_10BIT | ADCCON2_SCH_AIN7;

    // full speed, start conversion (bit0+1 always write as 1...)
    ADCCON1 = ADCCON1_ST | ADCCON1_STSEL_FULL_SPEED | 0b11;

    // configure DMA1 + DMA2:
    hal_adc_dma_init(1, &hal_adc_data[0], DMA_TRIG_ADC_CH0 + ADC0);
    hal_adc_dma_init(2, &hal_adc_data[1], DMA_TRIG_ADC_CH0 + ADC1);

    // set pointer to the DMA configuration struct into DMA-channel 1-4
    // configuration
    SET_WORD(DMA1CFGH, DMA1CFGL, &hal_dma_config[1]);

    hal_adc_dma_arm();

    // for testing only, do not use under normal use
#if ADC_DO_TEST
    adc_test();
#endif  // ADC_DO_TEST
}

void hal_adc_dma_arm(void) {
    DMAARM = (DMA_ARM_CH1 | DMA_ARM_CH2);
}

void hal_adc_process(void) {
    if (hal_adc_dma_done()) {
        // THIS OUTPUT BREAKS CONNECTIVITY! USE FOR DEBUGGING ONLY.
        // fine, arm dma:
        hal_adc_dma_arm();
    } else {
        // oops this should not happen
        debug_putc('D');
        // cancel and re arm dma
        // DMAARM = DMA_ARM_ABORT | (DMA_ARM_CH1 | DMA_ARM_CH2);
    }
}


void hal_adc_dma_init(uint8_t dma_id, uint16_t __xdata *dest_adr, uint8_t trig) {
    hal_dma_config[dma_id].PRIORITY       = DMA_PRI_LOW;  // example used high...
    hal_dma_config[dma_id].M8             = DMA_M8_USE_7_BITS;
    hal_dma_config[dma_id].IRQMASK        = DMA_IRQMASK_DISABLE;
    hal_dma_config[dma_id].TRIG           = trig;
    hal_dma_config[dma_id].TMODE          = DMA_TMODE_BLOCK;
    hal_dma_config[dma_id].WORDSIZE       = DMA_WORDSIZE_BYTE;

    SET_WORD(hal_dma_config[dma_id].SRCADDRH,  hal_dma_config[dma_id].SRCADDRL,  &X_ADCL);
    SET_WORD(hal_dma_config[dma_id].DESTADDRH, hal_dma_config[dma_id].DESTADDRL, dest_adr);
    hal_dma_config[dma_id].VLEN           = DMA_VLEN_USE_LEN;

    SET_WORD(hal_dma_config[dma_id].LENH, hal_dma_config[dma_id].LENL, 2);
    hal_dma_config[dma_id].SRCINC         = DMA_SRCINC_1;
    hal_dma_config[dma_id].DESTINC        = DMA_DESTINC_1;
}

uint8_t hal_adc_dma_done(void) {
    return ((DMAIRQ & (DMA_ARM_CH1 | DMA_ARM_CH2)) == (DMA_ARM_CH1 | DMA_ARM_CH2));
}

uint8_t hal_adc_get_scaled(uint8_t ch) {
    uint16_t adc_data;
    // adc data is HHHHHHHHLLLL0000 -> shift >>6 to get 10bit
    // the cc2510 is _ALWAYS_ outputting data in two's complement format
    // thus we shift >>7 to get 9 bit two's complement
    // NOTE: the chip seems to have a bug, in contrast to the datasheet
    //      measuring a signal close to GND seems to deliver negative values (!)
    //      therefore we have to check for negative numbers and set them to zero
    if (ch == 0) {
        // convert to 8 bit (see above)
        adc_data = hal_adc_data[1] >> 7;
        if (adc_data & (1 << 8)) adc_data = 0;  // bugfix: handle negative numbers
        // return fixed value
        return adc_data;
    } else {
        adc_data = hal_adc_data[0] >> 7;
        if (adc_data & (1 << 8)) adc_data = 0;  // bugfix: handle negative numbers
        #ifdef ADC1_USE_ACS712
        // acs712 is connected to ADC1
        // when powered by 5V we can use a trick
        // to get a good resolution:
        // use inverted power inputs to get
        // 0A = 2.5V
        // 30A = 0.0V
        return 255-(adc_data);
        #else
        return adc_data;
        #endif  // ADC1_USE_ACS712
    }
}

#if ADC_DO_TEST
void hal_adc_test(void) {
    debug("adc: running test\n"); debug_flush();

    while (1) {
        debug("adc: re-arming adc\n"); debug_flush();
        hal_adc_dma_arm();

        debug("adc: waiting for adc completion\n"); debug_flush();
        while (!hal_adc_dma_done()) {
            debug_putc('.');
            delay_ms(1);
        }
        debug("\nadc: done. res[0] = "); debug_flush();

        debug_put_uint16(hal_adc_data[0]>>4);
        debug_putc('x'); debug_put_hex8(hal_adc_data[0]>>12);
        debug_put_hex8((hal_adc_data[0]>>4)&0xff);

        debug(", res[1] = "); debug_flush();

        debug_put_uint16(hal_adc_data[1]>>4);
        debug_putc('x'); debug_put_hex8(hal_adc_data[1]>>12);
        debug_put_hex8((hal_adc_data[1]>>4)&0xff);

        debug_put_newline();

        delay_ms(100);

        // reset wdt
        wdt_reset();
    }
}
#endif  // ADC_DO_TEST
