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

#include "debug.h"
#include "uart.h"

#define DEBUG_DEFINE_TO_STR(x) #x
#define DEBUG_DEFINE_TO_STR_VAL(x) DEBUG_DEFINE_TO_STR(x)

EXTERNAL_MEMORY uint8_t debug_init_done = 0;

void debug_init(void) {
    uart_init();
    debug_init_done = 1;

    debug_put_newline();
    debug("### OpenSky - ");
    debug(DEBUG_DEFINE_TO_STR_VAL(BUILD_TARGET));
    debug(" - (c) by github.com/fishpepper ###\n"); debug_flush();
    debug("uart: init done\n");
}
