#ifndef __HAL_SBUS_H__
#define __HAL_SBUS_H__
#include <stdint.h>
#include "hal_defines.h"
#include "hal_uart.h"

//this helper routine will invert the data
//stored in buffer in case the sbus is set
//to inverted
#ifdef SBUS_INVERTED
  #define HAL_SBUS_PREPARE_DATA(a) (0xFF ^ (a))
#else
  #define HAL_SBUS_PREPARE_DATA(a) (a)
#endif

#endif // __HAL_SBUS_H__
