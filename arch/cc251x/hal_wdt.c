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

#include "hal_wdt.h"
#include "debug.h"
#include "led.h"
#include "delay.h"
#include "hal_cc25xx.h"

void hal_wdt_init(void) {
    //check if 32khz clock source is rcosc:
    if (!(CLKCON & CLKCON_OSC32K)){
        debug("wdt: error! low speed clock not based on int rc");
        led_green_on();
        while(1){
            led_red_on();
            delay_ms(200);
            led_red_off();
            delay_ms(200);
        }
    }

    //set wdt interval to approx 1 second
    WDCTL = (WDCTL & ~WDCTL_INT) | WDCTL_INT_1S;

    //enable wdt. NOTE: this can not be disabled in software!
    WDCTL = (WDCTL & ~WDCTL_MODE) | WDCTL_EN;
}

void hal_wdt_reset(void){
    //reset wdt (special sequence)
    WDCTL = (WDCTL & 0x0F) | 0b10100000;
    WDCTL = (WDCTL & 0x0F) | 0b01010000;
}
