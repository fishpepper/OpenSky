#ifndef __HAL_ADC_H__
#define __HAL_ADC_H__
#include <stdint.h>

#ifdef ADC_PORT
  #define ADC_ENABLED
#endif

//adc results
extern __xdata uint16_t hal_adc_data[2];

void hal_adc_init(void);
uint8_t hal_adc_get_scaled(uint8_t ch);
void hal_adc_dma_arm(void);
void hal_adc_dma_init(uint8_t dma_id, uint16_t __xdata *dest_adr, uint8_t trig);
uint8_t hal_adc_dma_done(void);

void hal_adc_test(void);
void hal_adc_process(void);

#endif

