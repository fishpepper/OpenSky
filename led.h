#ifndef __LED_H__
#define __LED_H__

#include "hal_led.h"

//set/clear/toggle
#define led_green_off()    hal_led_green_off()
#define led_green_on()     hal_led_green_on()
#define led_green_toggle() hal_led_green_toggle()

//set/clear/toggle
#define led_red_off()    hal_led_red_off()
#define led_red_on()     hal_led_red_on()
#define led_red_toggle() hal_led_red_toggle()

//led init routines
#define led_init() {  hal_led_red_init(); hal_led_green_init(); led_red_off(); led_green_off();}

#endif


