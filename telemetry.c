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

#include "telemetry.h"
#include "soft_serial.h"
#include "debug.h"

void telemetry_init(void) {
    debug("telemetry: init\n"); debug_flush();

    // init software serial port:
    soft_serial_init();

    // attach callback
    soft_serial_set_rx_callback(&telemetry_rx_callback);
}


void telemetry_rx_callback(uint8_t data) {
    debug("telemetry rx 0x");
    debug_put_hex8(data);
    debug_put_newline();
}
