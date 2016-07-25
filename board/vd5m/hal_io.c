#include "hal_io.h"

void hal_io_init(void) {
	set port as input
}

uint8_t hal_io_bind_request(void){
    if (P0 & (1<<SERVO_1)){
	//HIGH -> button pressed
        return 0;
    }else{
	//LOW -> button pressed
        return 1;
    }
}