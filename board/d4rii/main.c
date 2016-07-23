#include "stm32f10x.h"
#include "led.h"
#include "uart.h"
#include "debug.h"

int i = 0;
int off = 5;

void inc(void){
  i += off;
}



int main(void){
	uint32_t i=0;
	led_init();
	uart_init();
	
	while (1) {
		for(i=0; i<0x00FFFFF; i++) {}
		led_green_on();
		led_red_on();
		for(i=0; i<0x00FFFFF; i++) {}
		led_green_off();
		led_red_off();
		
		for(i=0; i<0x00FFFFF; i++) {}
		for(i=0; i<0x00FFFFF; i++) {}
		debug_putc('#');
		debug("MAIN");
	}
}
