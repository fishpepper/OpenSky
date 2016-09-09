#ifndef __HAL_UART_H__
#define __HAL_UART_H__

#include <stdint.h>

void hal_uart_init(void);
void hal_uart_start_transmission(uint8_t ch);
uint8_t hal_uart_int_enabled(void);
void hal_uart_int_enable(void);
#define hal_uart_int_disable() {}
#define DEBUG_ISR(void) hal_uart_tx_interrupt(void)
#define HAL_UART_ISR_FLAG_SET() (1)
#define HAL_UART_ISR_CLEAR_FLAG() { }
#define HAL_UART_ISR_DISABLE() { }
#define HAL_UART_TX_DATA(data) { }


#endif // __HAL_UART_H__
