#ifndef __HAL_SOFT_SERIAL_H__
#define __HAL_SOFT_SERIAL_H__
#include "soft_serial.h"
#include "config.h"

void hal_soft_serial_init(void);

#define HUB_TELEMETRY_PIN_LO() (0)
#define HUB_TELEMETRY_PIN_HI() (! HUB_TELEMETRY_PIN_LO())

#endif // __HAL_SOFT_SERIAL_H__
