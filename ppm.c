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

#include "ppm.h"
#include "main.h"
#include "config.h"
#include "debug.h"
#include "wdt.h"
#include "failsafe.h"

//ppm signal:
// s  CH1  s  CH2  s ... s   FILL_UP_TO_20.0ms
//|`|_____|`|_____|`|_  |`|_____________________
//
// s   = sync pulse= 0.3ms
// CH* = channel pulse low for 0.7-1.7ms -> total channel (incl sync) = 1.0-2.0ms
// total channel duration: 8*2ms = 16ms -> frame filler >= 4ms

//invert ppm polarity?
#define PPM_INVERTED 1

__xdata volatile uint8_t ppm_output_index;
__xdata uint16_t ppm_data_ticks[9];

void ppm_init(void){
    debug("ppm: init\n"); debug_flush();

    //no int on overflow:
    OVFIM = 0;

    //set channels to compare interupt:
    //CH0: off
    T1CCTL0 = 0;
    //CH1: off
    T1CCTL1 = 0;
    //CH2: toggle pin on cmp match, will generate sync pulses
    #if PPM_INVERTED
    //inverted, set on match, clear on zero
    T1CCTL2 = T1CCTLx_MODE_COMPARE | T1CCTLx_CMP_SETCLR0;
    #else
    //non-inverted, clear on match, set on zero
    T1CCTL2 = T1CCTLx_MODE_COMPARE | T1CCTLx_CMP_CLRSET0;
    #endif

    //configure peripheral alternative1 for timer 1:
    //use alt config 1 -> clr flag -> P0_4 = output
    PERCFG &= ~(PERCFG_T1CFG);

    //USART1 use ALT2 in order to free up P0_4 for peripheral func
    PERCFG |= PERCFG_U1CFG;

    //select P0_4 for peripheral function
    //NOTE: make sure to set usart1 to alt2 config!
    P0SEL |= (1<<4);

    //select P0_4 as output
    P0DIR |= (1<<4);

    //prescaler = 128
    //tickspeed = 26MHz / 8 = 3,25MHz (TICKSPD is set in timeout.c!)
    //1us = 3.25 ticks -> 2ms = 6500 ticks, 4ms = 13000
    T1CTL = T1CTL_MODE_MODULO | T1CTL_DIV_1;


    //ch2 cmp: sync pulse length
    SET_WORD_LO_FIRST(T1CC2H, T1CC2L, PPM_SYNC_PULS_LEN_TICKS);

    //overflow:
    SET_WORD_LO_FIRST(T1CC0H, T1CC0L, PPM_US_TO_TICKCOUNT(1000));

    ppm_output_index = 0;

    //clear pending interrupt flags (IRCON is reset by hw)
    T1CTL &= ~(T1CTL_CH0_IF | T1CTL_CH1_IF | T1CTL_CH2_IF | T1CTL_OVFIF);

    //overflow causes an int -> reload next channel data
    OVFIM = 1;

    //enable T1 interrups
    T1IE = 1;

    debug("ppm: init done\n"); debug_flush();
}


void ppm_update(__xdata uint16_t *data){
    uint8_t i=0;
    uint16_t val;
    uint16_t eof_frame_duration = PPM_FRAME_LEN;

    //convert to ticks for timer
    //input is 0..4095, we should map this to 1000..2000us
    //-> 1000 + input/4 = 1000 + (input<<2)
    for(i = 0; i<8; i++){
        val = data[i];
        //convert us to ticks:
        val = PPM_FRSKY_TO_TICKCOUNT(val);

        //make sure we end up with valid values:
        val = max(PPM_US_TO_TICKCOUNT( 900), val);
        val = min(PPM_US_TO_TICKCOUNT(2100), val);

        //subtract from sum:
        eof_frame_duration -= val;

        //set ppm tick data, disable ints during this:
        cli();
        ppm_data_ticks[i] = val;
        sei();
    }
    ppm_data_ticks[8] = eof_frame_duration;

    //debug("ppm: in "); debug_flush();
    //debug_put_uint16(data[0]);
    //debug(" out ");
    //debug_put_uint16(ppm_data_ticks[0]);
    //debug_put_newline(); debug_flush();

    //exit failsafe mode
    failsafe_exit();
}

void ppm_exit_failsafe(void){
    //debug("ppm: exit FS\n");

    //start from beginning
    ppm_output_index = 0;

    //configure p0_4 as peripheral:
    P0SEL |= (1<<4);

    //reset counter:
    SET_WORD_LO_FIRST(T1CNTH, T1CNTL, 0);

    //re enable timer interrupts:
    OVFIM = 1;

    //disable T1 interrups
    T1IE = 1;
}

void ppm_enter_failsafe(void){
    //disable interrupts
    OVFIM = 0;

    //disable T1 interrups
    T1IE = 0;

    //configure p0_4 as normal i/o:
    P0SEL &= ~(1<<4);

    //set pins to failsafe level:
    #if PPM_INVERTED
    //clear on zero -> default is high
    P0 |= (1<<4);
    #else
    //set on zero -> default is low
    P0 &= ~(1<<4);
    #endif

    //debug("ppm: entered FS\n");
}

//timer1 interrupt, this handles the reloading of the
//channel data to the timer cmp register
void ppm_timer1_interrupt(void) __interrupt T1_VECTOR{
    uint16_t pulse_len = PPM_US_TO_TICKCOUNT(1000);

    //clear pending interrupt flags (IRCON is reset by hw)
    T1CTL &= ~(T1CTL_CH0_IF | T1CTL_CH1_IF | T1CTL_CH2_IF | T1CTL_OVFIF);


    //failsafe mode?
    if (failsafe_is_active()){
        //failsafe_enter() will set pin levels
        return;
    }

    //handle failsafe
    failsafe_tick();

    if (ppm_output_index < 9){
        //load data
        pulse_len = ppm_data_ticks[ppm_output_index];
    }

    //manage index:
    ppm_output_index++;
    if (ppm_output_index >= 9){
        ppm_output_index = 0;
    }

    //set overflow cmp value
    SET_WORD_LO_FIRST(T1CC0H, T1CC0L, pulse_len);
}
