#include "cc25xx.h"
#include "debug.h"

void cc25xx_init(void) {
	debug("cc25xx: init\n"); debug_flush();
	hal_cc25xx_init();
}