#ifndef __HAL_SBUS_H__
#define __HAL_SBUS_H__
#include <stdint.h>
#include "hal_defines.h"
#include "hal_uart.h"

//this helper routine will invert the data
//stored in buffer in case the sbus is set
//to inverted
#if SBUS_INVERTED
#define HAL_SBUS_PREPARE_DATA(a) (0xFF ^ (a))
#else
#define HAL_SBUS_PREPARE_DATA(a) (a)
#endif

void hal_sbus_init(EXTERNAL_MEMORY uint8_t *data_ptr);
void hal_sbus_uart_set_mode(EXTERNAL_MEMORY union hal_uart_config_t *cfg);
void hal_sbus_start_transmission(uint8_t *data, uint8_t len);

//best match for 100kbit/s = 99975.5859375 bit/s
//baudrate = (((256.0 + baud_m)*2.0**baud_e) / (2**28)) * 26000000.0
#define HAL_SBUS_BAUD_E 11
#define HAL_SBUS_BAUD_M 248


#endif // __HAL_SBUS_H__
