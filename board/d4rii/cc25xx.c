#include "cc25xx.h"
#include "debug.h"

uint8_t cc25xx_current_antenna;

void cc25xx_init(void) {
	debug("cc25xx: init\n"); debug_flush();
	cc25xx_current_antenna = 0;
	hal_cc25xx_init();
}

void cc25xx_switch_antenna(void) {
	// switch to next antenna
	if (cc25xx_current_antenna) {
		cc25xx_current_antenna = 0;
	}else{
		cc25xx_current_antenna = 1;
	}
	hal_cc25xx_set_antenna(cc25xx_current_antenna);
}