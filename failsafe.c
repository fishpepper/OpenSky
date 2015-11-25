#include "failsafe.h"
#include "debug.h"
#include "sbus.h"
#include "ppm.h"

__xdata volatile uint8_t failsafe_active;
__xdata volatile uint16_t failsafe_tick_counter;


void failsafe_init(void){
    debug("failsafe: init\n"); debug_flush();
    failsafe_tick_counter = 0;

    //start in failsafe mode
    failsafe_enter();
}

void failsafe_enter(void){
    debug("failsafe: enter\n");

    #if SBUS_ENABLED
    sbus_enter_failsafe();
    #else
    ppm_enter_failsafe();
    #endif

    //failsafe is active
    failsafe_active = 1;
}

void failsafe_exit(void){
    //reset tick counter
    failsafe_tick_counter = 0;

    if (failsafe_active){
        //reset failsafe counter:
        failsafe_active = 0;

        #if SBUS_ENABLED
        sbus_exit_failsafe();
        #else
        ppm_exit_failsafe();
        #endif

        debug("failsafe: left\n");
    }
}

void failsafe_tick(void){
    //this could be running during an interrupt!
    //count missing data packets
    failsafe_tick_counter++;

    //if >1.5s no packets -> enter failsafe!
    //actually failsafe is also entered from within frsky.c
    //this is meant as a second failsafe guard
    if (failsafe_tick_counter >= 50*9*1.5){
        //go to failsafe mode!
        failsafe_enter();
    }
}

uint8_t failsafe_is_active(void){
    return failsafe_active;
}


