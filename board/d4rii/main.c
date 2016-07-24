#include "stm32f10x.h"
#include "led.h"
#include "uart.h"
#include "debug.h"
#include "clocksource.h"
#include "timeout.h"
#include "delay.h"
#include "wdt.h"
#include "frsky.h"

int i = 0;
int off = 5;

void inc(void){
  i += off;
}



int main(void){
	//leds:
	led_init();

	//init clock sources:
	clocksource_init();

	//init uart
	uart_init();

	//init wdt timer
	//FIXME FIXME FIXME
	//wdt_init();

//	apa102_init();

	//init storage
//	storage_init();

	//enable timeout routines
	timeout_init();

	/*while(1){
		led_red_on();
		timeout_set(1000);
		while(!timeout_timed_out()){}
		led_red_off();
		timeout_set(1000);
		while(!timeout_timed_out()){}
	}*/
	
	
	//apa102_init();

	//init frsky core
	frsky_init();
/*
	//init adc
	adc_init();

	//init output
	#if SBUS_ENABLED
	sbus_init();
	#else
	ppm_init();
	#endif

	//init failsafe
	failsafe_init();

	debug("main: init done\n");

	//run main
	//frsky_frame_sniffer();
	frsky_main();
*/
	/*LED_RED_ON();
	while (1) {
		LED_RED_ON();
		delay_ms(200);
		LED_RED_OFF();
		delay_ms(200);
	}
	*/
	
	uint32_t i=0;
	uint8_t t=0;
	while (1) {
		uint16_t y;
		led_green_on();
		for(y=0; y<100; y++){
			uint16_t x;
			for(x=0; x<100; x++){
				delay_us(100);
			}
			//10ms
			wdt_reset();
		}
		led_green_off();
		debug_put_uint8(t++); debug_put_newline();
		//delay_ms(200);
	}
	while (1) {
		for(i=0; i<10; i++) { delay_ms(10); wdt_reset();}
		led_green_on();
		led_red_on();
		for(i=0; i<10; i++) { delay_ms(10); wdt_reset();}
		led_green_off();
		led_red_off();
		
		for(i=0; i<20; i++) { delay_ms(10); wdt_reset();}
		debug_putc('#');
		debug("MAIN");
	}
}
