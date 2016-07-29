#include "spi.h"
#include "debug.h"

void spi_init(void){
    debug("spi: init\n"); debug_flush();
    hal_spi_init();
}
