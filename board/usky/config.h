#include "portmacros.h"

//pin layout ISP header
#define ISP_DATA  P2_1
#define ISP_CLOCK P2_2

#define PPM_OUT_PIN 4

//note: change of adc ch require change in adc.c!
#define ADC_PORT P0
#define ADC0 5
#define ADC1 6
//acs712 in inverted mode on adc1?
//when powered by 5V we can use a trick
//to get a good resolution: use inverted power inputs
// 0A = 2.5V
//30A = 0.0V
#define ADC1_USE_ACS712 1

//voltage divider on my board is 10 / 5.6 k, scale to 100 / 56 to avoid floating point calc
#define ADC0_DIVIDER_A 100
#define ADC0_DIVIDER_B  56

//APA102 SPI CLOCK
#define SOFT_SPI_CLOCK_PORT P2
#define SOFT_SPI_CLOCK_PIN  1
//APA102 SPI DATA
#define SOFT_SPI_MOSI_PORT P2
#define SOFT_SPI_MOSI_PIN  2

//P0.2 is NC! change this to P0.3 (debug tx)
#define BIND_PORT P0
#define BIND_PIN  2

//leds
#define LED_GREEN_PORT P1
#define LED_GREEN_PIN  1
//re led is NC!
#define LED_RED_PORT P1
#define LED_RED_PIN  2


//hub telemetry input (will run a soft serial port)
#define SOFT_SERIAL_PORT          P0
#define SOFT_SERIAL_PIN           7


#define DEFAULT_FSCAL_VALUE -20

