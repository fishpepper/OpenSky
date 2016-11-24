#ifndef __HAL_DEBUG__H_
#define __HAL_DEBUG__H_
#include "stm32f10x_gpio.h"
#include "stm32f10x_usart.h"
#include "config.h"
#include "hal_uart.h"

extern volatile uint8_t hal_debug_txe_is_on;

void hal_debug_init(void);
void hal_debug_start_transmission(uint8_t ch);

#define hal_debug_int_enable()  { hal_debug_init_nvic(1); }
#define hal_debug_int_disable()  { hal_debug_init_nvic(0); }

uint8_t hal_debug_int_enabled(void);

void hal_debug_init_nvic(uint8_t enable);
static void hal_debug_init_rcc(void);
static void hal_debug_init_gpio(void);
static void hal_debug_init_mode(void);
static void hal_debug_enable(void);

#define DEBUG_ISR(void) DEBUG_USART_IRQHANDLER(void)
#define HAL_DEBUG_ISR_FLAG_SET() (USART_GetITStatus(DEBUG_USART, USART_IT_TXE) != RESET)
#define HAL_DEBUG_ISR_CLEAR_FLAG() { }
#define HAL_DEBUG_ISR_DISABLE() { USART_ITConfig(DEBUG_USART, USART_IT_TXE, DISABLE); hal_debug_txe_is_on  = 0; }
#define HAL_DEBUG_TX_DATA(data) { USART_SendData(DEBUG_USART, data); }


#endif // __HAL_DEBUG__H_
