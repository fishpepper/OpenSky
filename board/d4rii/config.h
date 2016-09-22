#ifndef __PIN_CONFIG_H__
#define __PIN_CONFIG_H__
#include "main.h"

#define LED_GPIO                  GPIOA
#define LED_GPIO_CLK              RCC_APB2Periph_GPIOA
#define LED_GPIO_CLK_RCC          2
#define LED_RED_PIN               GPIO_Pin_11
#define LED_GREEN_PIN             GPIO_Pin_12


#if SBUS_INVERTED
    //DEBUG is on SERVO4 output:
    #define DEBUG_USART               USART3
    #define DEBUG_USART_CLK           RCC_APB1Periph_USART3
    #define DEBUG_USART_CLK_RCC       1
    #define DEBUG_USART_GPIO          GPIOB
    #define DEBUG_USART_GPIO_CLK      RCC_APB2Periph_GPIOB
    #define DEBUG_USART_GPIO_CLK_RCC  2
    #define DEBUG_USART_TX_PIN        GPIO_Pin_10
    #define DEBUG_USART_IRQHANDLER    USART3_IRQHandler
    #define DEBUG_USART_IRQn          USART3_IRQn
    //SBUS is INVERTED on 4 pin header TX pin
    #define SBUS_USART                 USART1
    #define SBUS_USART_CLK             RCC_APB2Periph_USART1
    #define SBUS_USART_RCC             2
    #define SBUS_USART_GPIO            GPIOA
    #define SBUS_USART_GPIO_CLK        RCC_APB2Periph_GPIOA
    #define SBUS_USART_GPIO_CLK_RCC    2
    #define SBUS_USART_TX_PIN          GPIO_Pin_9
    #define SBUS_USART_IRQHANDLER      USART1_IRQHandler
    #define SBUS_USART_IRQn            USART1_IRQn
#else
    //DEBUG is INVERTED(!) on 4pin header TX pin
    #define DEBUG_USART               USART1
    #define DEBUG_USART_CLK           RCC_APB2Periph_USART1
    #define DEBUG_USART_CLK_RCC       2
    #define DEBUG_USART_GPIO          GPIOA
    #define DEBUG_USART_GPIO_CLK      RCC_APB2Periph_GPIOA
    #define DEBUG_USART_GPIO_CLK_RCC  2
    #define DEBUG_USART_TX_PIN        GPIO_Pin_9
    #define DEBUG_USART_IRQHANDLER    USART1_IRQHandler
    #define DEBUG_USART_IRQn          USART1_IRQn
    //SBUS is non-inverted on SERVO4 output:
    #define SBUS_USART                 USART3
    #define SBUS_USART_CLK             RCC_APB1Periph_USART3
    #define SBUS_USART_RCC             1
    #define SBUS_USART_GPIO            GPIOB
    #define SBUS_USART_GPIO_CLK        RCC_APB2Periph_GPIOB
    #define SBUS_USART_GPIO_CLK_RCC    2
    #define SBUS_USART_TX_PIN          GPIO_Pin_10
    #define SBUS_USART_IRQHANDLER      USART3_IRQHandler
    #define SBUS_USART_IRQn            USART3_IRQn
#endif

#define CC25XX_SPI_GPIO             GPIOA
#define CC25XX_SPI_SCK_PIN          GPIO_Pin_5
#define CC25XX_SPI_MOSI_PIN         GPIO_Pin_7
#define CC25XX_SPI_MISO_PIN         GPIO_Pin_6
#define CC25XX_SPI_CSN_PIN          GPIO_Pin_4
#define CC25XX_SPI_GDO2_PIN         GPIO_Pin_3
#define CC25XX_SPI                  SPI1
#define CC25XX_SPI_CLK              RCC_APB2Periph_SPI1
#define CC25XX_SPI_CLK_RCC          2
#define CC25XX_SPI_GPIO_CLK         RCC_APB2Periph_GPIOA
#define CC25XX_SPI_DMA_CLOCK        RCC_AHBPeriph_DMA1
#define CC25XX_SPI_TX_DMA_CHANNEL   DMA1_Channel3
#define CC25XX_SPI_TX_DMA_TC_FLAG   DMA1_FLAG_TC3
#define CC25XX_SPI_RX_DMA_CHANNEL   DMA1_Channel2
#define CC25XX_SPI_RX_DMA_TC_FLAG   DMA1_FLAG_TC2

#define CC25XX_ANT_SW_CTX_GPIO     GPIOC
#define CC25XX_ANT_SW_CTX_GPIO_CLK RCC_APB2Periph_GPIOC
#define CC25XX_ANT_SW_CTX_GPIO_CLK_RCC 2
#define CC25XX_ANT_SW_CTX_PIN      GPIO_Pin_15


#define CC25XX_ANT_SW_CRX_GPIO     GPIOC
#define CC25XX_ANT_SW_CRX_GPIO_CLK RCC_APB2Periph_GPIOC
#define CC25XX_ANT_SW_CRX_GPIO_CLK_RCC 2
#define CC25XX_ANT_SW_CRX_PIN      GPIO_Pin_14


#define CC25XX_LNA_SW_CTX_GPIO     GPIOA
#define CC25XX_LNA_SW_CTX_GPIO_CLK RCC_APB2Periph_GPIOA
#define CC25XX_LNA_SW_CTX_GPIO_CLK_RCC 2
#define CC25XX_LNA_SW_CTX_PIN      GPIO_Pin_15


#define CC25XX_LNA_SW_CRX_GPIO     GPIOB
#define CC25XX_LNA_SW_CRX_GPIO_CLK RCC_APB2Periph_GPIOB
#define CC25XX_LNA_SW_CRX_GPIO_CLK_RCC 2
#define CC25XX_LNA_SW_CRX_PIN      GPIO_Pin_4


#define CC25XX_GDO2_GPIO           GPIOA
#define CC25XX_GDO2_PIN            GPIO_Pin_3
#define CC25XX_GDO2_GPIO_CLK       RCC_APB2Periph_GPIOA
#define CC25XX_GDO2_GPIO_CLK_RCC   2


#define BIND_JUMPER_GPIO           GPIOA
#define BIND_JUMPER_PIN            GPIO_Pin_8
#define BIND_JUMPER_GPIO_CLK       RCC_APB2Periph_GPIOA
#define BIND_JUMPER_GPIO_CLK_RCC   2

#define EEPROM_GPIO                GPIOB
#define EEPROM_GPIO_CLK            RCC_APB2Periph_GPIOB
#define EEPROM_GPIO_CLK_RCC        2
#define EEPROM_I2C                 I2C1
#define EEPROM_I2C_CLK             RCC_APB1Periph_I2C1
#define EEPROM_I2C_CLK_RCC         1
#define EEPROM_I2C_SDA_PIN         GPIO_Pin_7
#define EEPROM_I2C_SCL_PIN         GPIO_Pin_6
#define EEPROM_WP_PIN              GPIO_Pin_8
//A2=LO, A1=LO, A0=HI on pcb
#define EEPROM_I2C_ADDRESS         0xA2


#define ADC_GPIO                  GPIOA
#define ADC_GPIO_CLK              RCC_APB2Periph_GPIOA
#define ADC_GPIO_CLK_RCC          2
#define ADC_CLK                   RCC_APB2Periph_ADC1
#define ADC_CLK_RCC               2
#define ADC_IN1_PIN               GPIO_Pin_1
#define ADC_IN2_PIN               GPIO_Pin_2
#define ADC_DMA_CHANNEL           DMA1_Channel1
#define ADC_DMA_TC_FLAG           DMA1_FLAG_TC1
#define ADC                       ADC1

//ppm output
#define PPM_GPIO                  GPIOB
#define PPM_GPIO_CLK              RCC_APB2Periph_GPIOB
#define PPM_GPIO_CLK_RCC          2
#define PPM_PIN                   GPIO_Pin_0
#define PPM_TIMER                 TIM3
#define PPM_TIMER_CLK             RCC_APB1Periph_TIM3
#define PPM_TIMER_CLK_RCC         1
#define PPM_TIMER_CH              TIM_Channel_3
#define PPM_TIMER_IRQHANDLER      TIM3_IRQHandler
#define PPM_TIMER_IRQn            TIM3_IRQn

//soft spi for apa102 led
#define SOFT_SPI_GPIO             GPIOB
#define SOFT_SPI_GPIO_CLK         RCC_APB2Periph_GPIOB
#define SOFT_SPI_GPIO_CLK_RCC     2
#define SOFT_SPI_MOSI             GPIO_Pin_1
#define SOFT_SPI_SCK              GPIO_Pin_2

//hub telemetry input NOTE: this has to be a timer io
#define SOFT_SERIAL_GPIO          GPIOA
#define SOFT_SERIAL_CLK           RCC_APB2Periph_GPIOA
#define SOFT_SERIAL_CLK_RCC       2
#define SOFT_SERIAL_PIN           GPIO_Pin_10
#define SOFT_SERIAL_TIMER         TIM1
#define SOFT_SERIAL_TIMER_CLK     RCC_APB2Periph_TIM1
#define SOFT_SERIAL_TIMER_CLK_RCC 2
#define SOFT_SERIAL_TIMER_IT_IC   TIM_IT_CC3
#define SOFT_SERIAL_TIMER_CH      TIM_Channel_3
#define SOFT_SERIAL_TIMER_GET_CAPTURE()  TIM_GetCapture3(SOFT_SERIAL_TIMER)
#define SOFT_SERIAL_TIMER_IT_UP   TIM_IT_Update
#define SOFT_SERIAL_TIMER_IC_IRQHandler TIM1_CC_IRQHandler
#define SOFT_SERIAL_TIMER_UP_IRQHandler TIM1_UP_TIM16_IRQHandler
#define SOFT_SERIAL_TIMER_IC_IRQn TIM1_CC_IRQn
#define SOFT_SERIAL_TIMER_UP_IRQn TIM1_UP_IRQn

// THIS CONFIGURES IRQ PRIORITIES - DO NOT MESS THIS UP!
// this is the most critical stuff:
#define NVIC_PRIO_PPM 1
// sbus is a tx interrupt, this can be delayed at no cost
#define NVIC_PRIO_SBUS 6
// this is very time critical, but as telemetry data is not that important
// we allow this to be interrupted (=corrupted) occasionally
#define NVIC_PRIO_SOFT_SERIAL 2
// debugging data is not critical
#define NVIC_PRIO_DEBUG_UART 7
// systick prio
#define NVIC_PRIO_SYSTICK 0

#define DEFAULT_FSCAL_VALUE -62

#endif //__PIN_CONFIG_H__
