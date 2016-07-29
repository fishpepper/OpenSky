#ifndef __ADC_H__
#define __ADC_H__
#include "main.h"
#include "hal_adc.h"

void adc_init(void);
#define adc_get_scaled(ch) hal_adc_get_scaled(ch)
#define adc_process() hal_adc_process()

#endif
