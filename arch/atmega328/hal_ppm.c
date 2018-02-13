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

#include <stdint.h>

#include "hal_ppm.h"
#include "hal_io.h"
#include "ppm.h"
#include <avr/io.h>
#include <avr/interrupt.h>

/*
 * CS12 CS11 CS10 Description
 * 0    0    0    No clock source (Timer/Counter stopped).
 * 0         1    clkI/O/1
 * 0    1    0    clkI/O/8
 * 0    1    1    clkI/O/64
 * 1    0    0    clkI/O/256
 * 1    0    1    clkI/O/1024
 */

void hal_ppm_init(void) {
  cli(); // Disable interrupts
	//initialise timer 1:
	//we use COMPA and COMPB
	//COMPA triggers the io pin level change
	//COMPB triggers an interrupt and reloads the data whereas
  // Set timer to Mode 15, Fast PWM,  TOP: OCR1A, OCR1x at BOTTOM, TOV1 flag on TOP
	TCCR1A = (0<<COM1A1) | (0<<COM1A0) | (0<<COM1B1) | (0<<COM1B0) | (0<<FOC1A) | (0<<FOC1B) | (1<<WGM11) | (1<<WGM10);
	TCCR1B = (1<<WGM13) | (1<<WGM12) | (0<<CS12) | (1<<CS11) | (1<<CS10);

  // Configure TIMER1

	//default during reset is 1ms
	OCR1A = HAL_PPM_US_TO_TICKCOUNT(1000);

	//interrupt for timer value reload is triggered after sync pulse
	OCR1B = HAL_PPM_US_TO_TICKCOUNT(PPM_SYNC_DURATION_US);

	//enable interrupts:
  // MASK
	TIMSK1 = (1<<OCIE1B) | (1<<OCIE1A);

  // Counter
	TCNT1 = 0;
  PPM_PORT &= ~ (1 << PPM_PIN);
  sei(); // Enable interrupts again
}

void hal_ppm_update_ccvalue(uint16_t tickcount) {
	OCR1A = tickcount;
}

ISR(TIMER1_CAPT_vect){
}

ISR(TIMER1_COMPA_vect){
#if (PPM_OUT_INVERTED)
  PPM_PORT &= ~ (1 << PPM_PIN);
#else
  PPM_PORT |= 1 << PPM_PIN;
#endif
}

ISR(TIMER1_COMPB_vect){
#if (PPM_OUT_INVERTED)
  PPM_PORT |= 1 << PPM_PIN;
#else
  PPM_PORT &= ~ (1 << PPM_PIN);
#endif
  ppm_timer_isr();
}


ISR(TIMER1_OVF_vect){
}

void hal_ppm_failsafe_exit(void) {
}

void hal_ppm_failsafe_enter(void) {
}


