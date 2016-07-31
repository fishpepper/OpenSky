#ifndef __HAL_SBUS_H__
#define __HAL_SBUS_H__
#include <cc2510fx.h>


//this helper routine will invert the data
//stored in buffer in case the sbus is set
//to inverted
#if SBUS_INVERTED
#define HAL_SBUS_PREPARE_DATA(a) (0xFF ^ (a))
#else
#define HAL_SBUS_PREPARE_DATA(a) (a)
#endif
void sbus_uart_set_mode(EXTERNAL_MEMORY union uart_config_t *cfg);


#endif // __HAL_SBUS_H__
