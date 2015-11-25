#ifndef __SBUS_H__
#define __SBUS_H__
#include <stdio.h>
#include <stdint.h>
#include <cc2510fx.h>
#include "main.h"

void sbus_init(void);
void sbus_update(__xdata uint16_t *data);
void sbus_exit_failsafe(void);
void sbus_enter_failsafe(void);


extern __xdata volatile uint8_t sbus_failsafe_active;
extern __xdata volatile uint16_t sbus_nodata_counter;
extern __xdata uint16_t sbus_data[24];

#endif
