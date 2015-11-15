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
//
// CC251x datasheet: http://www.ti.com/lit/ds/symlink/cc2510.pdf
//
#include <cc2510fx.h>
#include "main.h"
#include "led.h"
#include "delay.h"
#include "uart.h"
#include "clocksource.h"
#include "debug.h"
#include "frsky.h"
#include "timeout.h"
#include "adc.h"
#include "wdt.h"
#include "storage.h"
#include "ppm.h"
#include "apa102.h"

void main(void) {
    //leds:
    LED_INIT();

    //init clock source XOSC:
    clocksource_init();

    //init uart
    uart_init();

    //enable interrupts:
    sei();

    //init wdt timer
    wdt_init();

    apa102_init();

    //init storage
    storage_init();


    //enable timeout routines
    timeout_init();

    //apa102_init();

    //init frsky core
    frsky_init();

    //init adc
    adc_init();

    //init ppm output
    ppm_init();

    debug("main: init done\n");

    //run main
    frsky_main();

	LED_RED_ON();
	while (1) {
        LED_RED_ON();
        delay_ms(200);
        LED_RED_OFF();
        delay_ms(200);
    }
}


