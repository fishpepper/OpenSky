#ifndef __HAL_UART_H__
#define __HAL_UART_H__
#include <stdint.h>
#include "hal_defines.h"

void hal_uart_init(EXTERNAL_MEMORY uint8_t *data_ptr);
void hal_uart_start_transmission(uint8_t *buffer, uint8_t len);

#endif // __HAL_UART_H__
