#ifndef __HAL_DEBUG_H__
#define __HAL_DEBUG_H__

#include "hal_cc25xx.h"
#include "hal_uart.h"
#include <stdint.h>

void hal_debug_init(void);
void hal_debug_start_transmission(uint8_t ch);

#define hal_debug_int_enabled() (IEN2 & IEN2_UTX0IE)
#define hal_debug_int_enable() { sei(); }
#define hal_debug_int_disable() { cli(); }

static void hal_debug_set_mode(__xdata union hal_uart_config_t *cfg);
#if (DEBUG_UART == USART0_P0) || (DEBUG_UART == USART0_P1)
  #define DEBUG_ISR(void) hal_uart_tx_interrupt(void) __interrupt UTX0_VECTOR
  #define HAL_DEBUG_ISR_FLAG_SET() (1)
  #define HAL_DEBUG_ISR_CLEAR_FLAG() { UTX0IF = 0; }
  #define HAL_DEBUG_ISR_DISABLE() { IEN2 &= ~(IEN2_UTX0IE); }
  #define HAL_DEBUG_TX_DATA(data) { U0DBUF = data; }
#else
  #define DEBUG_ISR(void) hal_uart_tx_interrupt(void) __interrupt UTX1_VECTOR
  #define HAL_DEBUG_ISR_FLAG_SET() (1)
  #define HAL_DEBUG_ISR_CLEAR_FLAG() { UTX1IF = 0; }
  #define HAL_DEBUG_ISR_DISABLE() { IEN2 &= ~(IEN2_UTX1IE); }
  #define HAL_DEBUG_TX_DATA(data) { U1DBUF = data; }
#endif

#endif // __HAL_DEBUG_H__
