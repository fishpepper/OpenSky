#ifndef __HAL_SOFT_SERIAL_H__
#define __HAL_SOFT_SERIAL_H__
#include "soft_serial.h"
#include "config.h"

void hal_soft_serial_init(void);

#define HAL_SOFT_SERIAL_PIN_LO() (0)
#define HAL_SOFT_SERIAL_PIN_HI() (! HAL_SOFT_SERIAL_PIN_LO())

#endif // __HAL_SOFT_SERIAL_H__
