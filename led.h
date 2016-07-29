#ifndef __LED_H__
#define __LED_H__

#include "hal_led.h"

//set/clear/toggle
#define led_green_off() hal_led_green_off()
#define led_green_on()  hal_led_green_on()

//set/clear/toggle
#define led_red_off() hal_led_red_off()
#define led_red_on()  hal_led_red_on()

//led init routines
#define led_init() {  hal_led_red_init(); hal_led_green_init(); led_red_off(); led_green_off(); }

#endif


