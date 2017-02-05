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

#include "hal_debug.h"
#include  "debug.h"
#include  "led.h"

#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

void hal_debug_init(void) {
}

void hal_debug_init_nvic(uint8_t enable) {
}


static void hal_debug_init_mode(void) {
}

static void hal_debug_enable(void) {
}

void hal_debug_start_transmission(uint8_t ch) {
    fwrite(&ch, 1, 1, stdout);
}

void hal_debug_int_enable(void) {
}

uint8_t hal_debug_int_enabled(void) {
    return 0;
}

static void hal_debug_init_gpio(void) {
}

static void hal_debug_init_rcc(void) {
}
