/*
    Copyright 2017 fishpepper <AT> gmail.com

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

#include "failsafe.h"
#include "debug.h"
#include "sbus.h"
#include "ppm.h"
#include "led.h"


EXTERNAL_MEMORY volatile uint8_t failsafe_active;
EXTERNAL_MEMORY volatile uint16_t failsafe_tick_counter;

void failsafe_init(void) {
    debug("failsafe: init\n"); debug_flush();
    failsafe_tick_counter = 0;

    // start in failsafe mode
    failsafe_enter();
}

void failsafe_enter(void) {
    // debug("failsafe: enter\n");

    led_red_on();
    led_green_off();

    sbus_enter_failsafe();
    ppm_enter_failsafe();

    failsafe_active = 1;
}

void failsafe_exit(void) {
    // reset tick counter
    failsafe_tick_counter = 0;

    if (failsafe_active) {
        // reset failsafe counter:
        failsafe_active = 0;

        led_red_off();

        sbus_exit_failsafe();
        ppm_exit_failsafe();

        // debug("failsafe: left\n");
    }
}

void failsafe_tick(void) {
    // this could be running during an interrupt!
    // count missing data packets
    failsafe_tick_counter++;

    // if >1.5s no packets -> enter failsafe!
    // actually failsafe is also entered from within frsky.c
    // this is meant as a second failsafe guard
    if (failsafe_tick_counter >= 50*9*1.5) {
        // go to failsafe mode!
        failsafe_enter();
    }
}


