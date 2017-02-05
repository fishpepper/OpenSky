/*
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
#include  "stm32f10x_rcc.h"
#include  "stm32f10x_usart.h"
#include  "misc.h" // this is actually a stm32 include (nvic stuff)
#include  "debug.h"
#include  "led.h"

volatile uint8_t hal_debug_txe_is_on;

void hal_debug_init(void) {
    hal_debug_txe_is_on = 0;

    hal_debug_init_nvic(0);
    hal_debug_init_rcc();
    hal_debug_init_gpio();
    hal_debug_init_mode();
    hal_debug_enable();
}

void hal_debug_init_nvic(uint8_t enable) {
    // enable interrupts
    NVIC_InitTypeDef nvic_init;


    // configure the NVIC Preemption Priority Bits
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);

    // enable the USART interrupt
    nvic_init.NVIC_IRQChannel = DEBUG_USART_IRQn;
    nvic_init.NVIC_IRQChannelPreemptionPriority = NVIC_PRIO_DEBUG_UART;
    nvic_init.NVIC_IRQChannelSubPriority = 0;
    nvic_init.NVIC_IRQChannelCmd = enable ? ENABLE : DISABLE;
    NVIC_Init(&nvic_init);
}


static void hal_debug_init_mode(void) {
    USART_InitTypeDef uart_init;

    // USART configuration:
    // 115200 baud, 8N1
    // no hw flow control
    uart_init.USART_BaudRate = 115200;
    uart_init.USART_WordLength = USART_WordLength_8b;
    uart_init.USART_StopBits = USART_StopBits_1;
    uart_init.USART_Parity = USART_Parity_No;
    uart_init.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    uart_init.USART_Mode = USART_Mode_Tx;
    USART_Init(DEBUG_USART, &uart_init);
}

static void hal_debug_enable(void) {
    // enable uart
    USART_Cmd(DEBUG_USART, ENABLE);
}

void hal_debug_start_transmission(uint8_t ch) {
    // enable TXE int
    USART_ITConfig(DEBUG_USART, USART_IT_TXE, ENABLE);
    hal_debug_txe_is_on = 1;

    // send first byte
    USART_SendData(DEBUG_USART, ch);
}

uint8_t hal_debug_int_enabled(void) {
    // is the txe int enabled?
    return hal_debug_txe_is_on;
}

static void hal_debug_init_gpio(void) {
    GPIO_InitTypeDef gpio_init;

    // Configure USART TX as alternate function push-pull
    gpio_init.GPIO_Pin   = DEBUG_USART_TX_PIN;
    gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
    gpio_init.GPIO_Mode  = GPIO_Mode_AF_PP;
    GPIO_Init(DEBUG_USART_GPIO, &gpio_init);
}

static void hal_debug_init_rcc(void) {
    // configure clocks for uart:
    // enable GPIO clock
    RCC_APBxPeriphClockCmd(DEBUG_USART_GPIO_CLK_RCC, DEBUG_USART_GPIO_CLK | RCC_APB2Periph_AFIO, ENABLE);

    // enable USART clock
    RCC_APBxPeriphClockCmd(DEBUG_USART_CLK_RCC, DEBUG_USART_CLK, ENABLE);
}
