#ifndef __HAL_LED__H_
#define __HAL_LED__H_
#include "stm32f10x_gpio.h"
#include "config.h"

void hal_led_init(uint16_t pin);

#define hal_led_green_init() { hal_led_init(LED_GREEN_PIN); }
#define hal_led_green_on()   { LED_GPIO->BSRR = (LED_GREEN_PIN); }
#define hal_led_green_off()  { LED_GPIO->BRR = (LED_GREEN_PIN); }
#define hal_led_green_toggle() { LED_GPIO->BSRR = (LED_GPIO->ODR ^ LED_GREEN_PIN) | (LED_GREEN_PIN << 16);}

#define hal_led_red_init() { hal_led_init(LED_RED_PIN); }
#define hal_led_red_on()   { LED_GPIO->BSRR = (LED_RED_PIN); }
#define hal_led_red_off()  { LED_GPIO->BRR = (LED_RED_PIN); }
#define hal_led_red_toggle() { LED_GPIO->BSRR = (LED_GPIO->ODR ^ LED_RED_PIN) | (LED_RED_PIN << 16);}

#endif // __HAL_LED__H_

