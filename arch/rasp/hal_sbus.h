#ifndef __HAL_SBUS_H__
#define __HAL_SBUS_H__
#include <stdint.h>
#include "hal_defines.h"

void hal_sbus_init(EXTERNAL_MEMORY uint8_t *data_ptr);
void hal_sbus_start_transmission(uint8_t *buffer, uint8_t len);

//this helper routine will invert the data
//stored in buffer in case the sbus is set
//to inverted
#if SBUS_INVERTED
#define HAL_SBUS_PREPARE_DATA(a) (0xFF ^ (a))
#else
#define HAL_SBUS_PREPARE_DATA(a) (a)
#endif

#endif // __HAL_SBUS_H__
