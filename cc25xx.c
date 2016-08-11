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

#include "cc25xx.h"
#include "debug.h"

uint8_t cc25xx_current_antenna;

void cc25xx_init(void) {
    debug("cc25xx: init\n"); debug_flush();
    cc25xx_current_antenna = 0;
    hal_cc25xx_init();
    hal_cc25xx_set_antenna(cc25xx_current_antenna);
}

void cc25xx_switch_antenna(void) {
    // switch to next antenna
    if (cc25xx_current_antenna) {
        cc25xx_current_antenna = hal_cc25xx_set_antenna(0);
    }else{
        cc25xx_current_antenna = hal_cc25xx_set_antenna(1);
    }
}

