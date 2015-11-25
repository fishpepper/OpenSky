#ifndef __FAILSAFE_H__
#define __FAILSAFE_H__
#include "main.h"
#include <stdint.h>

void failsafe_init(void);
void failsafe_enter(void);
void failsafe_exit(void);
void failsafe_tick(void);
uint8_t failsafe_is_active(void);

#endif
