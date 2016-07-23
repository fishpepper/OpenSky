#ifndef __HAL_UART__H_
#define __HAL_UART__H_
#include "stm32f10x_gpio.h"
#include "pin_config.h"

void hal_uart_init(void);
uint8_t hal_uart_start_transmission(uint8_t ch);
void hal_uart_int_enable(uint8_t enable) ;
uint8_t hal_uart_int_enabled(void);

void _hal_uart_nvic_init(uint8_t enable);
void _hal_uart_init_rcc(void);
void _hal_uart_init_gpio(void);
void _hal_uart_enable(void);
void _hal_uart_init_mode(void);
void DEBUG_USART_IRQHANDLER(void);

#endif // __HAL_UART__H_