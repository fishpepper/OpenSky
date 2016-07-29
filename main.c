/*
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

   author: fishpepper <AT> gmail.com
*/
#include "stm32f10x.h"
#include "led.h"
#include "uart.h"
#include "debug.h"
#include "clocksource.h"
#include "timeout.h"
#include "delay.h"
#include "wdt.h"
#include "frsky.h"
#include "adc.h"
#include "storage.h"

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

    //enable timeout routines
    timeout_init();

    //init storage
    storage_init();


    // 	uint16_t seconds=0;
    // 	timeout_set(1000);
    // 	while(1){
    // 		while(!timeout_timed_out()){}
    // 		timeout_set(1000);
    // 		seconds++;
    // 		debug_put_uint16(seconds);
    // 		debug_put_newline();
    // 	}
    //

    //FIXME//apa102_init();

    //init frsky core
    frsky_init();

    //init adc
    adc_init();

    //init output
#if SBUS_ENABLED
    //FIXME//sbus_init();
#else
    //FIXME//ppm_init();
#endif

    //init failsafe
    ///FIXME//failsafe_init();

    debug("main: init done\n");

    //run main
    //frsky_frame_sniffer();
    frsky_main();
    /*
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
