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
#include "delay.h"
#include "stm32f10x_iwdg.h"
#include "stm32f10x_rcc.h"

void hal_wdt_init(void) {
    // detect resets from wdt
    if (RCC_GetFlagStatus(RCC_FLAG_IWDGRST) != RESET){
        debug("hal_wdt: watchdog reset detected\n"); debug_flush();
        RCC_ClearFlag();
    }

    // set iwdg timeout to roughly 1000ms (varies due to LSI freq dispersion)
    uint32_t timeout_ms = 1000;

    // enable write access to IWDG_PR and IWDG_RLR registers
    IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);

    // IWDG counter clock: LSI/32
    IWDG_SetPrescaler(IWDG_Prescaler_32);

    // set counter reload value
    // 250ms timeout -> reload value = 0.25 * (LSI/32) = 0.25 * 40000 / 32 = 312.5
    // --> 1ms = 312.5 / 250 = 1.25 = 5/4
    IWDG_SetReload(timeout_ms * 5 / 4);

    // reload IWDG counter
    IWDG_ReloadCounter();

    // enable IWDG (the LSI oscillator will be enabled by hardware)
    IWDG_Enable();
}

inline void hal_wdt_reset(void){
    //reset wdt
     IWDG_ReloadCounter();
}

