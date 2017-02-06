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

#ifndef HAL_ADC_H_
#define HAL_ADC_H_

#include <stdint.h>

#ifdef ADC_PORT
  #define ADC_ENABLED
#endif  // ADC_PORT

// adc results
extern __xdata uint16_t hal_adc_data[2];

void hal_adc_init(void);
uint8_t hal_adc_get_scaled(uint8_t ch);
void hal_adc_dma_arm(void);
void hal_adc_dma_init(uint8_t dma_id, uint16_t __xdata *dest_adr, uint8_t trig);
uint8_t hal_adc_dma_done(void);

void hal_adc_test(void);
void hal_adc_process(void);

#endif  // HAL_ADC_H_
