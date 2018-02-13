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

   author: fishpepper <AT> gmail.com, jimmyw <AT> github
*/

#include "hal_timeout.h"
#include "hal_delay.h"

#include "../../src/debug.h"
#include <avr/io.h>
#include <avr/interrupt.h>

// Took me about 2 days to figure out that it was important that
// these variables was volatile. doh.
static volatile uint32_t timer_1 = 0;
static volatile uint32_t timer_2 = 0;

/*  TIMER0 TCCR0B
 *  CS02     CS01    CS00    DESCRIPTION
 *  0        0       0       Timer/Counter2 Disabled
 *  0        0       1       No Prescaling
 *  0        1       0       Clock / 8
 *  0        1       1       Clock / 64
 *  1        0       0       Clock / 256
 *  1        0       1       Clock / 1024
 */

/*  TIMER2 TCCR2B
 *  CS22     CS21    CS20    DESCRIPTION
 *  0        0       0       Timer/Counter2 Disabled
 *  0        0       1       No Prescaling
 *  0        1       0       Clock / 8
 *  0        1       1       Clock / 32
 *  1        0       0       Clock / 64
 *  1        0       1       Clock / 128
 *  1        1       0       Clock / 256
 *  1        1       1       Clock / 1024
 */

void hal_timeout_init(void) {
    cli(); // Disable interrupts

    // setup timer1 to use timer 0
    TCCR0A = (1<<WGM01); // CTC mode for timer 0
    TCCR0B  = (0 << CS02) | (1 << CS01) | (1 << CS00); // Clock / 64

    //set up overflow every 1.0ms.
    OCR0A   = F_CPU/64L/1000L;
    hal_timeout_set(0);

    // setup timer2 to use timer 2
    TCCR2B  = (0 << CS22) | (1 << CS21) | (1 << CS20); // Clock / 32
    TCCR2A = (1<<WGM21); // CTC mode
    OCR2A   = F_CPU/32L/10000L;

    //set up overflow every 100us.
    hal_timeout2_set_100us(0);

    sei(); // Enable interrupts again

#ifdef TIMER_TEST1
    debug("timer test_1\n");
    DDRD = (1<<PD4) | (1<<PD5);
    while(1){
        hal_timeout_set(1); // 1ms
        PORTD &= ~ (1 << PD4);
        while(timer_1 > 0){
        }
        PORTD |= (1 << PD4);
        hal_timeout_set(10); // 10ms
        PORTD &= ~ (1 << PD4);
        while(timer_1 > 0){
        }
        PORTD |= (1 << PD4);
    }
#endif
#ifdef TIMER_TEST2
    debug("timer test_2\n");
    DDRD = (1<<PD4) | (1<<PD5);
    sei();
    while(1){
        hal_timeout2_set_100us(10); // 1ms
        PORTD &= ~ (1 << PD4);
        while(timer_2 > 0){
        }
        PORTD |= (1 << PD4);
        hal_timeout2_set_100us(100); // 10ms
        PORTD &= ~ (1 << PD4);
        while(timer_2 > 0){
        }
        PORTD |= (1 << PD4);
    }
#endif
}

ISR(TIMER0_COMPA_vect){
#ifdef TIMER_TEST1
  static volatile int c = 0;
  if (c) {
    PORTD &= ~ (1 << PD5);
    c = 0;
  } else {
    PORTD |= (1 << PD5);
    c = 1;
  }
#endif

    // Disable interrupt.
    if (timer_1 == 0) {
      //debug("timeout1\n");
      TIMSK0 &= ~(1 << OCIE0A);
      return;
    }
    timer_1--;
}

ISR(TIMER2_COMPA_vect){
#ifdef TIMER_TEST2
  static volatile int c = 0;
  if (c) {
    PORTD &= ~ (1 << PD5);
    c = 0;
  } else {
    PORTD |= (1 << PD5);
    c = 1;
  }
#endif

    // Disable interrupt.
    if (timer_2 == 0) {
      //debug("timeout2\n");
      TIMSK2 &= ~(1 << OCIE2A);
      return;
    }
    timer_2--;
}


void hal_timeout_set(__IO uint32_t ms) {
    //debug("timeout1 set "); debug_put_hex32(ms); debug_put_newline();

    //disable OVF interrupts:
    TIMSK0 &= ~(1 << OCIE0A);

    //clear counter
    TCNT0 = 0;

    //clear pending ints
    TIFR0 |= (1<<OCF0A);

    //prepare timeout val
    timer_1 = ms;

    if (timer_1 == 0){
        return;
    }

    //clear pending ints
    TIFR0 |= (1<<OCF0A);

    //re enable interrupts
    TIMSK0 |= (1 << OCIE0A);

    //hal_io_debug2(1);
}

void hal_timeout2_set_100us(__IO uint32_t ms) {
    //debug("timeout2 set "); debug_put_uint8(ms); debug_put_newline();

    //disable OVF interrupts:
    TIMSK2 &= ~(1 << OCIE2A);

    //clear counter
    TCNT2 = 0;

    //clear pending ints
    TIFR2 |= (1<<OCF2A);

    //prepare timeout val:
    timer_2 = ms;

    if (timer_2 == 0){
        return;
    }

    //clear pending ints
    TIFR2 |= (1<<OCF2A);

    //re enable interrupts
    TIMSK2 |= (1 << OCIE2A);
}

uint8_t hal_timeout_timed_out(void) {
  //debug("timer_1: "); debug_put_uint8(timer_1); debug("\n"); debug_flush();
  return timer_1 == 0;
}

uint8_t hal_timeout2_timed_out(void) {
  //debug("timer_2: "); debug_put_uint8(timer_1); debug("\n"); debug_flush();
  return timer_2 == 0;
}

