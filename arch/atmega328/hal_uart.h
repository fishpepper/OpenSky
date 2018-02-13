#ifndef __HAL_UART__H_
#define __HAL_UART__H_
#include "config.h"
#include "hal_uart.h"
#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>

void hal_uart_init(void);
void hal_uart_start_transmission(uint8_t ch);
void hal_uart_tx_data(uint8_t ch);

uint8_t hal_uart_int_enabled(void);
void hal_uart_interrupt(void);


#define DEBUG_ISR(void) hal_uart_interrupt(void)
#define HAL_UART_ISR_FLAG_SET() (1)
#define HAL_UART_ISR_CLEAR_FLAG() {}
#define HAL_UART_ISR_DISABLE() {}
#define HAL_UART_TX_DATA(data) hal_uart_tx_data(data)


#endif // __HAL_UART__H_
