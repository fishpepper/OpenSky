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

#ifndef HAL_LED_H_
#define HAL_LED_H_

#include "stm32f10x_gpio.h"
#include "config.h"

void hal_led_init(uint16_t pin);

#define hal_led_green_init() { hal_led_init(LED_GREEN_PIN); }
#define hal_led_green_on()   { LED_GPIO->BSRR = (LED_GREEN_PIN); }
#define hal_led_green_off()  { LED_GPIO->BRR = (LED_GREEN_PIN); }
#define hal_led_green_toggle() { \
    LED_GPIO->BSRR = (LED_GPIO->ODR ^ LED_GREEN_PIN) | (LED_GREEN_PIN << 16);}

#define hal_led_red_init() { hal_led_init(LED_RED_PIN); }
#define hal_led_red_on()   { LED_GPIO->BSRR = (LED_RED_PIN); }
#define hal_led_red_off()  { LED_GPIO->BRR = (LED_RED_PIN); }
#define hal_led_red_toggle() { \
    LED_GPIO->BSRR = (LED_GPIO->ODR ^ LED_RED_PIN) | (LED_RED_PIN << 16);}

#endif  // HAL_LED_H_

