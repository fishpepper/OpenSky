#ifndef __HAL_SOFT_SERIAL_H__
#define __HAL_SOFT_SERIAL_H__
#include "soft_serial.h"
#include "config.h"

void hal_soft_serial_init(void);


#define HAL_SOFT_SERIAL_PIN_LO() ( 0 )
#define HAL_SOFT_SERIAL_PIN_HI() ( 1)


#define HAL_SOFT_SERIAL_IC_ISR_DISABLE()    { }
#define HAL_SOFT_SERIAL_IC_ISR_ENABLE()     { }
#define HAL_SOFT_SERIAL_IC_ISR_FLAG_SET()   ( 1 )
#define HAL_SOFT_SERIAL_IC_ISR_FLAG_CLEAR() { }

#define HAL_SOFT_SERIAL_UP_ISR_DISABLE()    { }
#define HAL_SOFT_SERIAL_UP_ISR_ENABLE()     { }
#define HAL_SOFT_SERIAL_UP_ISR_FLAG_SET()   ( 0 )
#define HAL_SOFT_SERIAL_UP_ISR_FLAG_CLEAR() { }

#define HAL_SOFT_SERIAL_UPDATE_TOP_VALUE(x)   { }

#endif // __HAL_SOFT_SERIAL_H__
