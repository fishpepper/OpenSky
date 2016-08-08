#include "portmacros.h"

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
#define ADC_PORT P0
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

#define BIND_PORT P0
#define BIND_PIN  SERVO_1

//leds
#define LED_GREEN_PORT P2
#define LED_GREEN_PIN  4
#define LED_RED_PORT P2
#define LED_RED_PIN  3

//hub telemetry input NOTE: this has to be a timer io
#define SOFT_SERIAL_PORT          P0
#define SOFT_SERIAL_PIN           7


