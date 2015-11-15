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

#include "wdt.h"
#include "debug.h"
#include "main.h"
#include "led.h"
#include "delay.h"

void wdt_init(void){
    debug("wdt: init\n"); debug_flush();

    //check if 32khz clock source is rcosc:
    if (!(CLKCON & CLKCON_OSC32K)){
        debug("wdt: error! low speed clock not based on int rc");
        LED_GREEN_OFF();
        while(1){
            LED_RED_ON();
            delay_ms(200);
            LED_RED_OFF();
            delay_ms(200);
        }
    }

    //set wdt interval to approx 1 second
    WDCTL = (WDCTL & ~WDCTL_INT) | WDCTL_INT_1S;

    //enable wdt. NOTE: this can not be disabled in software!
    WDCTL = (WDCTL & ~WDCTL_MODE) | WDCTL_EN;
}

void wdt_reset(void){
    //reset wdt (special sequence)
    WDCTL = (WDCTL & 0x0F) | 0b10100000;
    WDCTL = (WDCTL & 0x0F) | 0b01010000;
}
