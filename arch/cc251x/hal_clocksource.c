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
    along with this program.  If not, see <http:// www.gnu.org/licenses/>.

   author: fishpepper <AT> gmail.com
*/

#include "hal_clocksource.h"
#include "hal_cc25xx.h"
#include "led.h"

void hal_clocksource_init(void) {
    // for debugging clocksource problems
    led_red_on();
    led_green_on();

    // power up osc (?)
    SLEEP &= ~CLOCKSOURCE_OSC_PD_BIT;
    // wait for XOSC stable
    while(!CLOCKSOURCE_XOSC_STABLE()) {}
    NOP();

    // start crystal osc as HS clocksource, OSC32 is int rc osc
    CLKCON = 0x80;

    // wait for selection to be active
    while(!CLOCKSOURCE_XOSC_STABLE()) {}
    NOP();

    // power down the unused oscillator
    SLEEP |= CLOCKSOURCE_OSC_PD_BIT;


    // for debugging clocksource problems
    led_red_off();
    led_green_off();
}

