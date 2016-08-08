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

#include "hal_timeout.h"
#include "delay.h"
#include "debug.h"
#include "timeout.h"
#include "hal_cc25xx.h"

//do not place this in xdata (faster this way)
volatile uint16_t hal_timeout_countdown;

void hal_timeout_init(void){
    debug("hal_timeout: init\n"); debug_flush();

    //timer clock
    CLKCON = (CLKCON & ~CLKCON_TICKSPD_111) | CLKCON_TICKSPD_011;

    //prepare timer3 for 0.01ms steps:
    //TICKSPD 011 -> /8 = 3250 kHz timer clock input
    T3CTL = T3CTL_DIV_2 | // /2
            T3CTL_START |   //start
            T3CTL_OVFIM |   //OVInt enabled
            T3CTL_CLR  |   //clear
            T3CTL_MODE_MODULO; // 01 = count to CC and the overflow

    //3250/2/65 = 25khz
    T3CC0 = 65-1;

    //enable int
    IEN1 |= (IEN1_T3IE);

    timeout_set(0);

    /*LED_RED_OFF();
    while(1){
        timeout_set(990);
        LED_GREEN_OFF();
        while(!timeout_timed_out()){}
        timeout_set(10);
        LED_GREEN_ON();
        while(!timeout_timed_out()){}
    }*/

    /* //TEST timings
    P0DIR |= (1<<7);
    while(1){
        timeout_set(1);
        while(!timeout_timed_out()){}
        P0 |= (1<<7);
        LED_RED_ON();
        delay_ms(100);
        P0 &= ~(1<<7);
        LED_RED_OFF();
    }*/

}

//prepare a new timeout
void hal_timeout_set(uint16_t timeout_ms){
    //disable T3ints:
    IEN1 &= ~(IEN1_T3IE);

    //clear counter
    T3CTL |= (1<<2);

    //clear pending ints
    T3IF = 0;

    //prepare timeout val:
    hal_timeout_countdown = timeout_ms * 25;

    if (hal_timeout_countdown == 0){
        return;
    }

    //clear pending ints
    T3IF = 0;

    //re enable interrupts
    IEN1 |= IEN1_T3IE;
}

uint8_t hal_timeout_timed_out(void){
    if (hal_timeout_countdown == 0){
        return 1;
    }else{
        return 0;
    }
}

void hal_timeout_interrupt(void) __interrupt T3_VECTOR{
    //clear flag
    //T3IF = 0;

    if (hal_timeout_countdown == 0){
        //disable interrupts
        IEN1 &= ~(IEN1_T3IE);
        return;
    }

    hal_timeout_countdown--;
}
