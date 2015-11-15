#ifndef __ADC_H__
#define __ADC_H__
#include "main.h"

//adc results
extern __xdata uint16_t adc_data[2];

void adc_init(void);
uint8_t adc_get_scaled(uint8_t ch);
void adc_arm_dma(void);
void adc_dma_init(uint8_t dma_id, uint16_t __xdata *dest_adr, uint8_t trig);
uint8_t adc_dma_done(void);


void adc_test(void);
void adc_process(void);

#endif
