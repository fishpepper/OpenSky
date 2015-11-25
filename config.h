#include "portmacros.h"

//send ADC data as hub telemetry as well:
#define FRSKY_SEND_HUB_TELEMETRY 0

//sbus or ppm out on P0_4:
//enabling SBUS will DISABLE ppm!
//******************** WORK IN PROGRESS, NOT READY YET *********************
//#define SBUS_ENABLED 1

//pin layout ISP header
#define ISP_DATA  P2_1
#define ISP_CLOCK P2_2

//pin layout servo connection
#define SERVO_1 7 //P0_7 = BIND, pull down on startup to enter bind mode
#define SERVO_2 6 //P0_6 = ADC1 = voltage sensor (max 3.3V on I/O ! -> voltage divider necessary!)
#define SERVO_3 5 //P0_5 = ADC0 = current sensor (max 3.3V on I/O !)
#define SERVO_4 4 //P0_4 = PPM or SBUS OUT
#define SERVO_5 3 //P0_3 = debug UART

#define PPM_OUT SERVO_1
//note: change of adc ch require change in adc.c!
#define ADC0 SERVO_3
#define ADC1 SERVO_2
//acs712 in inverted mode on adc1?
//when powered by 5V we can use a trick
//to get a good resolution: use inverted power inputs
// 0A = 2.5V
//30A = 0.0V
#define ADC1_USE_ACS712 1

//voltage divider on my board is 10 / 3.3 k, scale to 100 / 33 to avoid floating point calc
#define ADC0_DIVIDER_A 100
#define ADC0_DIVIDER_B  33

#if 0
//APA102 SPI CLOCK
#define SOFT_SPI_CLOCK_PORT P0
#define SOFT_SPI_CLOCK_PIN  7
//APA102 SPI DATA
#define SOFT_SPI_MOSI_PORT P0
#define SOFT_SPI_MOSI_PIN  4
#else
//APA102 SPI CLOCK
#define SOFT_SPI_CLOCK_PORT P2
#define SOFT_SPI_CLOCK_PIN  1
//APA102 SPI DATA
#define SOFT_SPI_MOSI_PORT P2
#define SOFT_SPI_MOSI_PIN  2
#endif


//leds
#define LED_GREEN_PORT P2
#define LED_GREEN_PIN  4
#define LED_RED_PORT P2
#define LED_RED_PIN  3

//use helper macros to do expansion to *DIR etc
//LEDS
#define LED_GREEN_DIR PORT2DIR(LED_GREEN_PORT)
#define LED_RED_DIR   PORT2DIR(LED_RED_PORT)
#define LED_RED_BIT PORT2BIT(LED_RED_PORT, LED_RED_PIN)
#define LED_GREEN_BIT PORT2BIT(LED_GREEN_PORT, LED_GREEN_PIN)

//SOFTSPI
#define SOFT_SPI_CLOCK_DIR PORT2DIR(SOFT_SPI_CLOCK_PORT)
#define SOFT_SPI_CLOCK_BIT PORT2BIT(SOFT_SPI_CLOCK_PORT, SOFT_SPI_CLOCK_PIN)
#define SOFT_SPI_MOSI_DIR PORT2DIR(SOFT_SPI_MOSI_PORT)
#define SOFT_SPI_MOSI_BIT PORT2BIT(SOFT_SPI_MOSI_PORT, SOFT_SPI_MOSI_PIN)



