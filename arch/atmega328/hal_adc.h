#ifndef __HAL_ADC_H__
#define __HAL_ADC_H__
#include "main.h"
#include <stdint.h>

extern volatile uint16_t hal_adc_data[2];

void hal_adc_init(void);

uint8_t hal_adc_get_scaled(uint8_t ch);
void hal_adc_process(void);

#endif
