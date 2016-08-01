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
#include "sbus.h"
#include "failsafe.h"

int main(void){
    //leds:
    led_init();

    //init clock sources:
    clocksource_init();

    //init uart
    uart_init();

    //init wdt timer
    wdt_init();

    //apa102_init();

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

    debug("main: frsky main ended?! THIS SHOULD NOT HAPPEN!");
    while(1);
}
