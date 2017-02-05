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
    along with this program.  If not, see <http:// www.gnu.org/licenses/>.

   author: fishpepper <AT> gmail.com
*/

#include "ppm.h"
#include "main.h"
#include "debug.h"
#include "wdt.h"
#include "failsafe.h"

#ifndef SBUS_ENABLED

// ppm signal:
// s  CH1  s  CH2  s ... s   FILL_UP_TO_20.0ms
// |`|_____|`|_____|`|_  |`|_____________________
// 
// s   = sync pulse= 0.3ms
// CH* = channel pulse low for 0.7-1.7ms -> total channel (incl sync) = 1.0-2.0ms
// total channel duration: 8*2ms = 16ms -> frame filler >= 4ms
EXTERNAL_MEMORY volatile uint8_t ppm_output_index;
EXTERNAL_MEMORY uint16_t ppm_data_ticks[9];

void ppm_init(void) {
    uint8_t i;
    debug("ppm: init\n"); debug_flush();

    // initialise
    for(i = 0; i<8; i++) {
        ppm_data_ticks[i] = HAL_PPM_US_TO_TICKCOUNT(1000);
    }

    hal_ppm_init();

    debug("ppm: init done\n"); debug_flush();
}


void ppm_update(EXTERNAL_MEMORY uint16_t *data) {
    uint8_t i=0;
    uint16_t val;
    uint16_t eof_frame_duration = PPM_FRAME_LEN;

    // convert to ticks for timer
    // input is 0..4095, we should map this to 1000..2000us
    // frsky seems to send us*1.5 (~1480...3020) -> divide by 1.5 (=*2/3) to get us
    for(i = 0; i<8; i++) {
        val = data[i];
        // convert us to ticks:
        val = PPM_FRSKY_TO_TICKCOUNT(val);

        // make sure we end up with valid values:
        val = max(HAL_PPM_US_TO_TICKCOUNT( 900), val);
        val = min(HAL_PPM_US_TO_TICKCOUNT(2100), val);

        // subtract from sum:
        eof_frame_duration -= val;

        // set ppm tick data, disable ints during this:
        HAL_PPM_ISR_DISABLE();
        ppm_data_ticks[i] = val;
        HAL_PPM_ISR_ENABLE();
    }
    ppm_data_ticks[8] = eof_frame_duration;

    // debug("ppm: in "); debug_flush();
    // debug_put_uint16(data[0]);
    // debug(" out ");
    // debug_put_uint16(ppm_data_ticks[0]);
    // debug_put_newline(); debug_flush();
}

void ppm_exit_failsafe(void) {
    // debug("ppm: exit FS\n");

    // start from beginning
    ppm_output_index = 0;

    hal_ppm_failsafe_exit();
}

void ppm_enter_failsafe(void) {
    hal_ppm_failsafe_enter();
    // debug("ppm: entered FS\n");
}

// this handles the reloading of the
// channel data to the timer cmp register
void PPM_TIMER_ISR(void) {
    if (HAL_PPM_ISR_FLAG_SET()) {
        uint16_t pulse_len = HAL_PPM_US_TO_TICKCOUNT(1000);

        // clear flag
        HAL_PPM_ISR_CLEAR_FLAG(); // THIS SHOULD NEVER BE THE LAST LINE IN AN ISR!

        // failsafe mode?
        if (failsafe_active) {
            // failsafe_enter() will set pin levels
            return;
        }

        // handle failsafe
        failsafe_tick();

        if (ppm_output_index < 9) {
            // load data
            pulse_len = ppm_data_ticks[ppm_output_index];
        }

        // manage index:
        ppm_output_index++;
        if (ppm_output_index >= 9) {
            ppm_output_index = 0;
        }

        // set overflow cmp value
        HAL_PPM_UPDATE_CCVALUE(pulse_len);
    }
}

#endif

