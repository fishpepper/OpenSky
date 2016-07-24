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
#include "debug.h"
#include "wdt.h"

static __IO uint32_t hal_timeout_ms;
static __IO int32_t hal_tick;

void hal_timeout_init(void) {
	//configure 1ms sys tick:
	if (SysTick_Config(SystemCoreClock / 1000)){
		debug("hal_timeout: failed to set systick timeout\n");
	}
	
	hal_timeout_ms = 0;
	hal_tick = 0;
}

void hal_timeout_set(__IO uint32_t ms){ 
	hal_timeout_ms = ms;
}

uint8_t hal_timeout_timed_out(void) {
	//debug_put_uint16(hal_timeout_ms); debug("\n"); debug_flush();
	return (hal_timeout_ms == 0);
}

// seperate ms delay function
void hal_timeout_delay_ms(uint32_t timeout){
	int32_t hal_tick_copy;
	hal_tick_copy = hal_tick;
	
	while((hal_tick - hal_tick_copy) <= timeout) {
		wdt_reset();
	}
}

void SysTick_Handler(void){
	if (hal_timeout_ms != 0){
		hal_timeout_ms--;
	}
	hal_tick++;
}
