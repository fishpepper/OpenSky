#include "hal_io.h"
#include "portmacros.h"
#include "config.h"
#include "hal_cc25xx.h"

void hal_io_init(void) {
    //set bind pin as input
    PORT2DIR(BIND_PORT) &= ~(1<<BIND_PIN);
}

uint8_t hal_io_bind_request(void){
    if (BIND_PORT & (1<<BIND_PIN)){
    //HIGH -> button pressed
        return 0;
    }else{
    //LOW -> button pressed
        return 1;
    }
}
