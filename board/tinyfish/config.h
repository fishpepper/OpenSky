#include "portmacros.h"

//pin layout ISP header
#define ISP_DATA  P2_1
#define ISP_CLOCK P2_2

//NOT USED
#define PPM_OUT_PIN 4

//NOT USED
#define ADC_PORT P0
#define ADC0 5
#define ADC1 6
#define ADC1_USE_ACS712 0

//voltage divider on my board is 10 / 5.6 k, scale to 100 / 56 to avoid floating point calc
#define ADC0_DIVIDER_A 100
#define ADC0_DIVIDER_B  56

//APA102 SPI CLOCK
#define SOFT_SPI_CLOCK_PORT P2
#define SOFT_SPI_CLOCK_PIN  1
//APA102 SPI DATA
#define SOFT_SPI_MOSI_PORT P2
#define SOFT_SPI_MOSI_PIN  2

#define BIND_PORT P0
#define BIND_PIN  3

//LEDS
#define LED_GREEN_PORT P0
#define LED_GREEN_PIN  4
#define LED_RED_PORT   P0
#define LED_RED_PIN    1

//DEBUG
#define DEBUG_UART USART0_P1

//SBUS
#define SBUS_UART USART1_P1

//hub telemetry input (will run a soft serial port)
#define SOFT_SERIAL_PORT          P1
#define SOFT_SERIAL_PIN           4
#define SOFT_SERIAL_INVERTED      1

//device supports LNA/PA chip
#define RF_LNA_PA_AVAILABLE  0
#define RF_HIGH_GAIN_MODE_AVAILABLE 0

//bootloader config
#define BOOTLOADER_LED_GREEN_PORT LED_GREEN_PORT
#define BOOTLOADER_LED_GREEN_PIN  LED_GREEN_PIN
#define BOOTLOADER_LED_RED_PORT   LED_RED_PORT
#define BOOTLOADER_LED_RED_PIN    LED_RED_PIN
//bootloader runs on UART1 P1
#define BOOTLOADER_UART_NUM       1
#define BOOTLOADER_UART_PORT      1

#define DEFAULT_FSCAL_VALUE -69

