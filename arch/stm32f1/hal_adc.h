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

#include "main.h"
#include <stdint.h>

extern volatile uint16_t hal_adc_data[2];

void hal_adc_init(void);

static void hal_adc_init_rcc(void);
static void hal_adc_init_gpio(void);
static void hal_adc_init_mode(void);
static void hal_adc_init_dma(void);
static void hal_adc_dma_arm(void);

uint8_t hal_adc_get_scaled(uint8_t ch);
void hal_adc_process(void);

#endif  // HAL_ADC_H_
