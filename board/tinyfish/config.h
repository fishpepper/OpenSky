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

#define DEFAULT_FSCAL_VALUE -69

