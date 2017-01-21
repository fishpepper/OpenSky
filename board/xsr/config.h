#ifndef __PIN_CONFIG_H__
#define __PIN_CONFIG_H__
#include "main.h"

/* TODO find correct pins for LEDs */
#define LED_GPIO                  GPIOA
#define LED_GPIO_CLK              RCC_APB2Periph_GPIOA
#define LED_GPIO_CLK_RCC          2
#define LED_RED_PIN               GPIO_Pin_11
#define LED_GREEN_PIN             GPIO_Pin_12


// enable SBUS output -> this will DISABLE ppm!
#define SBUS_ENABLED
// Note: default/futaba is INVERTED=1! (use this for a cc3d etc)
//       inverted     => idle = LOW
//       non inverted => idle = high
#define SBUS_INVERTED

// PPM (only used if sbus is disabled)
// invert SBUS output (normal is non inverted)
//#define PPM_INVERTED

//hub telemetry input (soft serial)
//#define HUB_TELEMETRY_ON_SBUS_UART
//#define HUB_TELEMETRY_INVERTED 
//#define PPM_INVERTED

#define SBUS_USART                 USART1
#define SBUS_USART_CLK             RCC_APB2Periph_USART1
#define SBUS_USART_RCC             2
#define SBUS_USART_GPIO            GPIOA
#define SBUS_USART_GPIO_CLK        RCC_APB2Periph_GPIOA
#define SBUS_USART_GPIO_CLK_RCC    2
#define SBUS_USART_TX_PIN          GPIO_Pin_9
#define SBUS_USART_IRQHANDLER      USART1_IRQHandler
#define SBUS_USART_IRQn            USART1_IRQn

#define CC25XX_SPI_GPIO             GPIOB
#define CC25XX_SPI_SCK_PIN          GPIO_Pin_13
#define CC25XX_SPI_MOSI_PIN         GPIO_Pin_15
#define CC25XX_SPI_MISO_PIN         GPIO_Pin_14
/* 
    NOTE: CS pin is on GPIOA, PA8
    NOTE: GDO2 pin is on GPIOA, PA11 
*/
#define CC25XX_SPI_CSN_PIN          GPIO_Pin_8
#define CC25XX_SPI_GDO2_PIN         GPIO_Pin_11

#define CC25XX_SPI                  SPI2
#define CC25XX_SPI_CLK              RCC_APB2Periph_SPI2
#define CC25XX_SPI_CLK_RCC          2
/* TODO ?? chose correct DMA for SPI2 */
#define CC25XX_SPI_GPIO_CLK         RCC_APB2Periph_GPIOB
#define CC25XX_SPI_DMA_CLOCK        RCC_AHBPeriph_DMA1
#define CC25XX_SPI_TX_DMA_CHANNEL   DMA1_Channel3
#define CC25XX_SPI_TX_DMA_TC_FLAG   DMA1_FLAG_TC3
#define CC25XX_SPI_RX_DMA_CHANNEL   DMA1_Channel2
#define CC25XX_SPI_RX_DMA_TC_FLAG   DMA1_FLAG_TC2

/* 
    XSR uses the Skyworks SE2431 range extender
    See: http://www.skyworksinc.com/uploads/documents/SE2431L_202410J.pdf

    Looks to be a similar design to this: http://www.ti.com/tool/CC2530-SE2431L

    Which means the RX pin selection is not needed (it is RX if TX is off)

    Either ANT can be used for both RX and TX.

    High is logic 1, Low is logic 0

    ANT (PB5) selection is 0 (left) or 1 (right)
    CTX (PA15) is 0 then RX LNA mode active, or 1 then tx mode
    CPS (PB3) is 0 then bypass mode is active, 1 is bypass off.
    CSD (PB4) with 1 to enable chip (0 = sleep)
*/ 

/* TODO revisit base on the above */
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

/* GDO2 is on PA11 */
#define CC25XX_GDO2_GPIO           GPIOA
#define CC25XX_GDO2_PIN            GPIO_Pin_11
#define CC25XX_GDO2_GPIO_CLK       RCC_APB2Periph_GPIOA
#define CC25XX_GDO2_GPIO_CLK_RCC   2

/* bind pin is attached to PC13, and pushing button pulls to ground. */
#define BIND_JUMPER_GPIO           GPIOC
#define BIND_JUMPER_PIN            GPIO_Pin_13
#define BIND_JUMPER_GPIO_CLK       RCC_APB2Periph_GPIOC
#define BIND_JUMPER_GPIO_CLK_RCC   2

/* 
    EEPROM is connected to PA0 (SCL) and PA1 (SDA) 

    Note that this is **NOT** an I2C port on the F1 so must use SOFT I2C
*/
#define EEPROM_GPIO                GPIOA
#define EEPROM_GPIO_CLK            RCC_APB2Periph_GPIOA
#define EEPROM_GPIO_CLK_RCC        2
#define EEPROM_I2C_SDA_PIN         GPIO_Pin_1
#define EEPROM_I2C_SCL_PIN         GPIO_Pin_0
/* TODO: ??? */
#define EEPROM_WP_PIN              GPIO_Pin_8
//A2=LO, A1=LO, A0=HI on pcb
#define EEPROM_I2C_ADDRESS         0xA2

/* NOTE no analogue input for XSR, and no servo pins */

//cppm output is on PB12
#define PPM_GPIO                  GPIOB
#define PPM_GPIO_CLK              RCC_APB2Periph_GPIOB
#define PPM_GPIO_CLK_RCC          2
#define PPM_PIN                   GPIO_Pin_12
/* TODO: ??? chose timer for pin */
#define PPM_TIMER                 TIM3
#define PPM_TIMER_CLK             RCC_APB1Periph_TIM3
#define PPM_TIMER_CLK_RCC         1
#define PPM_TIMER_CH              TIM_Channel_3
#define PPM_TIMER_IRQHANDLER      TIM3_IRQHandler
#define PPM_TIMER_IRQn            TIM3_IRQn

/*
    hub telemetry input is RX USART3 (SPORT)

    PB11 for RX and PB10 for TX both inverted once leaving board.
    Output Enable (OE) is PA2
*/    


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
