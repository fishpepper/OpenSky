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
#include "delay.h"

//busy wait delay loop. not 100% accurate
static void delay_ms(uint16_t ms) {
    #define DELAY_MS_LOOP_A 86
    #define DELAY_MS_LOOP_B 30

    while(ms--){
        //this asm snippet gives us roughly 1ms delay:
        __asm
                mov     r1, #DELAY_MS_LOOP_A
            00000$: //delay_ms_loop_outer
                dec     r1
                mov     a, r1
                jz      00002$
                mov     r2, #DELAY_MS_LOOP_B
            00001$: //delay_ms_loop_inner
                dec     r2
                mov     a, r2
                jz      00000$
                sjmp    00001$
            00002$: //delay_ms_done
        __endasm;
    }
}

//busy wait delay loop
//this is more or less accurate
static void delay_us(uint16_t us) {
    #define DELAY_US_LOOP 1

    while(us--){
        __asm
            nop
            nop
            nop
        __endasm;
    }
}
