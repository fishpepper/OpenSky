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

#include "hal_io.h"
#include "portmacros.h"
#include "config.h"
#include "hal_cc25xx.h"

void hal_io_init(void) {
    //set bind pin as input
    PORT2DIR(BIND_PORT) &= ~(1<<BIND_PIN);
    //set pullup/down
    PORT2INP(BIND_PORT) &= ~(1<<BIND_PIN);
}

uint8_t hal_io_bind_request(void){
    if (BIND_PORT & (1<<BIND_PIN)){
    //HIGH -> button pressed
        return 0;
    }else{
    //LOW -> button pressed
        return 1;
    }
}
