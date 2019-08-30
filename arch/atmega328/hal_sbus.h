#ifndef __HAL_SBUS_H__
#define __HAL_SBUS_H__
#include <stdint.h>
#include "hal_defines.h"

void SBUS_USART_IRQHANDLER(void);

//not used here
#define HAL_SBUS_PREPARE_DATA(a) (a)

#endif // __HAL_SBUS_H__
