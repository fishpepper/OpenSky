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

#include "hal_uart.h"
#include "debug.h"
#include "main.h"
#include "sbus.h"
#include "delay.h"
#include "config.h"
#include "hal_defines.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "misc.h"  // this defines stm32 nvic stuff
#include "uart.h" 

#ifdef SBUS_ENABLED

volatile uint8_t hal_uart_tx_pos;
volatile uint8_t *hal_uart_tx_buffer;

void hal_uart_init(EXTERNAL_MEMORY uint8_t *data_ptr) {
    hal_uart_init_rcc();
    hal_uart_init_gpio();
    hal_uart_init_nvic();
    hal_uart_init_mode();
}

void SBUS_USART_IRQHANDLER(void) {
    if (USART_GetITStatus(SBUS_USART, USART_IT_TXE) != RESET) {
        // TXE interrupt
        // finished with sending?
        if (hal_uart_tx_pos >= SBUS_DATA_LEN) {
            // no data in fifo -> disable tx int:
            USART_ITConfig(SBUS_USART, USART_IT_TXE, DISABLE);
            // debug_put_newline();
        } else {
            // else: data to tx
            // debug_put_hex8(hal_uart_tx_buffer[hal_uart_tx_pos]);
            USART_SendData(SBUS_USART, hal_uart_tx_buffer[hal_uart_tx_pos++]);
        }
    }

#ifdef HUB_TELEMETRY_ON_SBUS_UART
    if (USART_GetITStatus(SBUS_USART, USART_IT_RXNE) != RESET) {
        // RXE interrupt
        uint8_t rx = USART_ReceiveData(SBUS_USART);

        if (uart_rx_callback != 0) {
            // execute callback
            uart_rx_callback(rx);
        }
    }
#endif  // HUB_TELEMETRY_ON_SBUS_UART
}


static void hal_uart_init_nvic(void) {
    // enable interrupts
    NVIC_InitTypeDef nvic_init;

    // configure the NVIC Preemption Priority Bits
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);

    // enable the USART interrupt
    nvic_init.NVIC_IRQChannel = SBUS_USART_IRQn;
    nvic_init.NVIC_IRQChannelPreemptionPriority = NVIC_PRIO_SBUS;
    nvic_init.NVIC_IRQChannelSubPriority = 0;
    nvic_init.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic_init);
}

static void hal_uart_init_gpio(void) {
    GPIO_InitTypeDef gpio_init;

    // Configure USART TX as alternate function push-pull
    gpio_init.GPIO_Pin   = SBUS_USART_TX_PIN;
    gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
    gpio_init.GPIO_Mode  = GPIO_Mode_AF_PP;
    GPIO_Init(SBUS_USART_GPIO, &gpio_init);
}

static void hal_uart_init_rcc(void) {
    // configure clocks for uart:
    // enable GPIO clock
    RCC_APBxPeriphClockCmd(SBUS_USART_GPIO_CLK_RCC, SBUS_USART_GPIO_CLK, ENABLE);
    RCC_APBxPeriphClockCmd(2, RCC_APB2Periph_AFIO, ENABLE);

    // enable USART clock
    RCC_APBxPeriphClockCmd(SBUS_USART_RCC, SBUS_USART_CLK, ENABLE);
}

static void hal_uart_init_mode(void) {
    USART_InitTypeDef uart_init;

    // USART configuration:
    // 100000bps inverted serial stream, 8 bits, even parity, 2 stop bits
    // no hw flow control
    uart_init.USART_BaudRate            = 100000;
    // THIS IS TRICKY! for parity bit we need to set uart to 9 bit mode !
    uart_init.USART_WordLength          = USART_WordLength_9b;
    uart_init.USART_StopBits            = USART_StopBits_2;
    uart_init.USART_Parity              = USART_Parity_Even;
    uart_init.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    uart_init.USART_Mode                = USART_Mode_Tx;

#ifdef HUB_TELEMETRY_ON_SBUS_UART
    uart_init.USART_Mode                |= USART_Mode_Rx;
#endif  // HUB_TELEMETRY_ON_SBUS_UART

    USART_Init(SBUS_USART, &uart_init);

    USART_Cmd(SBUS_USART, ENABLE);

#ifdef HUB_TELEMETRY_ON_SBUS_UART
    USART_ITConfig(SBUS_USART, USART_IT_RXNE, ENABLE);
#endif  // HUB_TELEMETRY_ON_SBUS_UART
}

void hal_uart_start_transmission(uint8_t *buffer, uint8_t len) {
    // copy data ptr:
    hal_uart_tx_buffer = buffer;

    // set up counter:
    hal_uart_tx_pos = 0;

    // enable TXE int
    USART_ITConfig(SBUS_USART, USART_IT_TXE, ENABLE);
}

#endif  // SBUS_ENABLED
