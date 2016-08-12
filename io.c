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

#include "io.h"
#include "hal_io.h"
#include "debug.h"
#include "delay.h"

void io_init(void) {
    debug("io: init\n"); debug_flush();
    hal_io_init();
}

uint8_t io_bind_request(void){
    uint8_t i;

    //wait for pin status to settle
    delay_ms(100);

    //check if pressed at least 200ms:
    if (hal_io_bind_request()){
        // double check to see if this was really a press:
        for(i=0; i<200; i++){
            delay_ms(1);
            if (! hal_io_bind_request()){
                // not pressed any more -> return
                return 0;
            }
        }
        //still pressed, return true
        return 1;
    }

    //not pressed
    return 0;
}
