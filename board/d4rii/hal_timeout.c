#include "hal_timeout.h"
#include "debug.h"

static __IO uint32_t hal_timeout_ms;

void hal_timeout_init(void) {
	//configure 1ms sys tick:
	if (!SysTick_Config(SystemCoreClock / 1000)){
		debug("hal_timeout: failed to set systick timeout\n");
	}
	
	hal_timeout_ms = 0;
}

void hal_timeout_set(__IO uint32_t ms){ 
	hal_timeout_ms = ms;
}

uint8_t hal_timeout_timed_out(void) {
	//debug_put_uint16(hal_timeout_ms); debug("\n"); debug_flush();
	return (hal_timeout_ms == 0);
}

void SysTick_Handler(void){
	if (hal_timeout_ms != 0){
		hal_timeout_ms--;
	}
}
