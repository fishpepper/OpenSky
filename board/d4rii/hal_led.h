#ifndef __HAL_LED__H_
#define __HAL_LED__H_
#include "stm32f10x_gpio.h"
#include "pin_config.h"

void _hal_led_init(uint8_t pin);

#define hal_led_green_init() { _hal_led_init(LED_GREEN_PIN); }
#define hal_led_green_on()   { GPIOA->BSRR = (1<<LED_GREEN_PIN); }
#define hal_led_green_off()  { GPIOA->BRR = (1<<LED_GREEN_PIN); }

#define hal_led_red_init() { _hal_led_init(LED_RED_PIN); }
#define hal_led_red_on()   { GPIOA->BSRR = (1<<LED_RED_PIN); }
#define hal_led_red_off()  { GPIOA->BRR = (1<<LED_RED_PIN); }


#endif //__HAL_LED__H_

