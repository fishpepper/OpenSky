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
