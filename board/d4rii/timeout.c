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

#include "timeout.h"
#include "debug.h"
#include "delay.h"

void timeout_init(void) {
    debug("timeout: init\n"); debug_flush();
    hal_timeout_init();

#if 0
    //measure delay_us(x) accuracy
    timeout_set(2000);
    uint32_t c;
    for(c=0; c<1000; c++){
        delay_us(1000); //1ms
    }
    uint32_t remaining = timeout_time_remaining();
    uint32_t duration  = 2000-remaining;
    debug("delay: delay_us() for 1000ms took ");
    debug_put_uint16(duration);
    debug("ms...\n");
#endif
}
