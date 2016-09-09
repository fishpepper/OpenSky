#ifndef __HAL_LED__H_
#define __HAL_LED__H_

#include <stdint.h>

void hal_led_init(uint16_t pin);

#define hal_led_green_init() {}
#define hal_led_green_on()   {}
#define hal_led_green_off()  {}
#define hal_led_green_toggle() {}

#define hal_led_red_init() {}
#define hal_led_red_on()   {}
#define hal_led_red_off()  {}
#define hal_led_red_toggle() {}

#endif //__HAL_LED__H_

