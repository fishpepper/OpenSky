#ifndef __PIN_CONFIG_H__
#define __PIN_CONFIG_H__

#define LED_GPIO      GPIOA
#define LED_GPIO_CLK  RCC_APB2Periph_GPIOA
#define LED_RED_PIN   GPIO_Pin_11
#define LED_GREEN_PIN GPIO_Pin_12



#define DEBUG_USART              USART1
#define DEBUG_USART_GPIO         GPIOA
#define DEBUG_USART_CLK          RCC_APB2Periph_USART1
#define DEBUG_USART_GPIO_CLK     RCC_APB2Periph_GPIOA
#define DEBUG_USART_RX_PIN       GPIO_Pin_10
#define DEBUG_USART_TX_PIN       GPIO_Pin_9
#define DEBUG_USART_IRQHANDLER   USART1_IRQHandler
#define DEBUG_USART_IRQn         USART1_IRQn

#endif //__PIN_CONFIG_H__
