#ifndef __FAILSAFE_H__
#define __FAILSAFE_H__
#include "main.h"
#include <stdint.h>
#include "config.h"

void failsafe_init(void);
void failsafe_enter(void);
void failsafe_exit(void);
void failsafe_tick(void);

extern __xdata volatile uint8_t failsafe_active;
extern __xdata volatile uint16_t failsafe_tick_counter;

#if SBUS_ENABLED
#define failsafe_enter(){ sbus_enter_failsafe(); failsafe_active = 1; }
#else
#define failsafe_enter(){ ppm_enter_failsafe(); failsafe_active = 1; }
#endif


#endif
