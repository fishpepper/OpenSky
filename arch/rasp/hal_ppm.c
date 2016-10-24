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

#include "hal_ppm.h"
#include "ppm.h"
#include "wdt.h"
#include "led.h"

#if SBUS_ENABLED == 1
void hal_ppm_init(void){
}
void hal_ppm_tick() {
}
#else

static int state = 1;
static int failsafe = 0;

void hal_ppm_init(void) {
    hal_set_ppm(state);
}

void hal_ppm_failsafe_enter(void){
    failsafe = 1;
}

void hal_ppm_failsafe_exit(void) {
    failsafe = 0;
}

void hal_ppm_update_cvalue(int us) {
    // Keep singal up, until the last 200ms of the count.
    hal_timeout_add_ppm(us - HAL_PPM_US_TO_TICKCOUNT(200));
}

void hal_ppm_tick() {
    if (state == 1) {
        state = 0;
        if (!failsafe)
            hal_set_ppm(state);
        // Then lower the signal for another 200ms..
        hal_timeout_add_ppm(HAL_PPM_US_TO_TICKCOUNT(200));
    } else { 
        state = 1;
        if (!failsafe)
            hal_set_ppm(state);

        // Call the code, that will call hal_ppm_update_cvalue back to us.
        hal_ppm_irq_callback();
    }
}

#endif
