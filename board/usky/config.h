//pin layout ISP header
#define ISP_DATA  P2_1
#define ISP_CLOCK P2_2

#define PPM_OUT_PIN 4

//note: change of adc ch require change in adc.c!
#define ADC_PORT P0
#define ADC0 5 //ADC0 is actually Pin 6, but we use this for telemetry on uSKY -> hack to disable it
#define ADC1 5 
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

//DBG DATA
#define BIND_PORT P2
#define BIND_PIN  1

//leds
#define LED_GREEN_PORT P1
#define LED_GREEN_PIN  1
#define LED_RED_PORT P2
#define LED_RED_PIN  3

//DEBUG
#define DEBUG_UART USART0_P0

//SBUS
#define SBUS_UART USART1_P0

//hub telemetry input (will run a soft serial port)
#define SOFT_SERIAL_PORT          P0
#define SOFT_SERIAL_PIN           6
#define SOFT_SERIAL_INVERTED 0

//device supports LNA/PA chip
#define RF_LNA_PA_AVAILABLE         0
//LNA control pin
//#define RF_LNA_PORT                 P0
//#define RF_LNA_PIN                  0 
//#define RF_LNA_ON_LEVEL             1
//PA control pin
//#define RF_PA_PORT                  P0
//#define RF_PA_PIN                   1
//#define RF_PA_ON_LEVEL              1
//device supports high and low gain mode?
#define RF_HIGH_GAIN_MODE_AVAILABLE 0
//enable high gain?
//#define RF_HIGH_GAIN_MODE_ENABLED   1
//gain control pin
//#define RF_HIGH_GAIN_MODE_PORT      P1
//#define RF_HIGH_GAIN_MODE_PIN       0
//#define RF_HIGH_GAIN_MODE_ON_LEVEL  1

//bootloader config
#define BOOTLOADER_LED_GREEN_PORT LED_GREEN_PORT
#define BOOTLOADER_LED_GREEN_PIN  LED_GREEN_PIN
#define BOOTLOADER_LED_RED_PORT   LED_RED_PORT
#define BOOTLOADER_LED_RED_PIN    LED_RED_PIN
//bootloader runs on UART1 P0
#define BOOTLOADER_UART_NUM       1
#define BOOTLOADER_UART_PORT      0
#define BOOTLOADER_UART_INVERTED 0
#define BOOTLOADER_UART_BAUDRATE 57600
#define BOOTLOADER_UART_USE_PARITY 0
// use ISP interface clock pin as bootloader enable pin
// pull this low during startup to enter bootloader mode
#define BOOTLOADER_ENABLE_PORT P2
#define BOOTLOADER_ENABLE_PIN  2


#define DEFAULT_FSCAL_VALUE -69

