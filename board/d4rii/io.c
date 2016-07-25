#include "io.h"
#include "debug.h"

void io_init(void) {
	debug("io: init\n"); debug_flush();
	hal_io_init();
}