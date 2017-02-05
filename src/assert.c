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
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

   author: fishpepper <AT> gmail.com
*/

#include "assert.h"
#include "debug.h"
#include "led.h"
#include "delay.h"

void assert_failed(uint8_t* filename, uint32_t line){
    //todo: add own implementation to report the file name and line number,
    //eg  : printf("Wrong parameters value: file %s on line %d\r\n", file, line)
    if (debug_is_initialized()) {
        debug("assert failed: ");
        debug(filename);
        debug(" line ");
        debug_put_uint16(line);
        debug_put_newline();

        debug_flush();
    }

    //infinite loop
    while (1){
        led_red_on();
        delay_ms(100);
        led_green_on();
        delay_ms(100);
        led_green_off();
        delay_ms(100);
        led_red_off();
        delay_ms(100);
    }
}

