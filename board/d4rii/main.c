#include "stm32f10x.h"
#include "hal_led.h"


int i = 0;
int off = 5;

void inc(void){
  i += off;
}



int main(void){
	uint32_t i=0;
	hal_led_red_init();
	hal_led_green_init();
	
	
	
	while (1) {
		for(i=0; i<0x00FFFFF; i++) {}
		hal_led_green_on();
		hal_led_red_off();
		for(i=0; i<0x00FFFFF; i++) {}
		hal_led_green_off();
		hal_led_red_on();
	}
}
