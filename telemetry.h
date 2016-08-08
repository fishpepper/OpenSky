#ifndef __TELEMETRY_H__
#define __TELEMETRY_H__

#include "soft_serial.h"
#include <stdint.h>

void telemetry_init(void);
void telemetry_rx_callback(uint8_t data);

#endif // __TELEMETRY_H__
