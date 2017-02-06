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

#include "hal_io.h"
#include "portmacros.h"
#include "config.h"
#include "hal_cc25xx.h"

void hal_io_init(void) {
    // set bind pin as input
    PORT2DIR(BIND_PORT) &= ~(1 << BIND_PIN);
    // set pullup/down
    PORT2INP(BIND_PORT) &= ~(1 << BIND_PIN);

#ifdef BIND2_PORT
    // this board allows two bind buttons, both will work
    // set bind2 pin as input
    PORT2DIR(BIND2_PORT) &= ~(1 << BIND2_PIN);
    // set pullup/down
    PORT2INP(BIND2_PORT) &= ~(1 << BIND2_PIN);
#endif  // BIND2_PORT
}

uint8_t hal_io_bind_request(void) {
    // test bind button
    if (!(BIND_PORT & (1 << BIND_PIN))) {
        // LOW -> button pressed
        return 1;
    }

    #ifdef BIND2_PORT
    if (!(BIND2_PORT & (1 << BIND2_PIN))) {
        // LOW -> button2 pressed
        return 1;
    }
    #endif  // BIND2_PORT

    // no button pressed...
    return 0;
}
