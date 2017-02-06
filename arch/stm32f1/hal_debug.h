/*
    Copyright 2017 fishpepper <AT> gmail.com

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http:// www.gnu.org/licenses/>.

   author: fishpepper <AT> gmail.com
*/

#ifndef HAL_DEBUG_H_
#define HAL_DEBUG_H_

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
#define HAL_DEBUG_ISR_DISABLE() {\
    USART_ITConfig(DEBUG_USART, USART_IT_TXE, DISABLE); hal_debug_txe_is_on  = 0; }
#define HAL_DEBUG_TX_DATA(data) {\
    USART_SendData(DEBUG_USART, data); }


#endif  // HAL_DEBUG_H_
