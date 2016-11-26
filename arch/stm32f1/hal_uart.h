#ifndef __HAL_UART_H_
#define __HAL_UART_H_

#include <stdint.h>
#include "hal_defines.h"

void hal_uart_init();
static void hal_uart_init_gpio(void);
static void hal_uart_init_rcc(void);
static void hal_uart_init_mode(void);
static void hal_uart_init_nvic(void);
void hal_uart_start_transmission(uint8_t *buffer, uint8_t len);

void SBUS_USART_IRQHANDLER(void);

#endif  // __HAL_UART_H_
