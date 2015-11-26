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

#define SBUS_BAUD_M 0
#define SBUS_BAUD_E 19

#define SBUS_DATA_LEN 24
extern __xdata uint16_t sbus_data[SBUS_DATA_LEN];

#endif
