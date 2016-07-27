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
#include "hal_wdt.h"
#include "debug.h"
#include "stm32f10x_wwdg.h"
#include "stm32f10x_rcc.h"

void hal_wdt_init(void) {
    // enable WWDG clock
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG, ENABLE);

    //on Value line devices, WWDG clock counter = (PCLK1 (24MHz)/4096)/8 = 732 Hz (~1366 us)
    //on other devices, WWDG clock counter = (PCLK1(36MHz)/4096)/8 = 1099 Hz (~910 us)
    WWDG_SetPrescaler(WWDG_Prescaler_8);

    //set Window value to 80; WWDG counter should be refreshed only when the counter
    //is below 80 (and greater than 64) otherwise a reset will be generated
    WWDG_SetWindowValue(127);

    //- On Value line devices,
    // Enable WWDG and set counter value to 127, WWDG timeout = ~1366 us * 64 = 87.42 ms
    // In this case the refresh window is: ~1366us * (127-80) = 64.20 ms < refresh window < ~1366us * 64 = 87.42ms
    // - On other devices
    // Enable WWDG and set counter value to 127, WWDG timeout = ~910 us * 64 = 58.25 ms
    // In this case the refresh window is: ~910 us * (127-80) = 42.77 ms < refresh window < ~910 us * 64 = 58.25ms
    WWDG_Enable(127);
}

void hal_wdt_reset(void){
    //reset wdt (special sequence)
    WWDG_Enable(127);
}
