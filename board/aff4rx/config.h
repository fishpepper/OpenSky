// pin layout ISP header
#define ISP_DATA  P2_1
#define ISP_CLOCK P2_2

// note: change of adc ch require change in adc.c!
#define ADC_PORT P0
#define ADC0 6
#define ADC1 5 
// acs712 in inverted mode on adc1?
// when powered by 5V we can use a trick
// to get a good resolution: use inverted power inputs
// 0A = 2.5V
// 30A = 0.0V
#define ADC1_USE_ACS712

// voltage divider on my board is 10 / 1 k, scale to 100 / 10
#define ADC0_DIVIDER_A 100
#define ADC0_DIVIDER_B  10

#define BIND_PORT P1
#define BIND_PIN  1

// LEDs
#define LED_GREEN_PORT P2
#define LED_GREEN_PIN  4
#define LED_RED_PORT P2
#define LED_RED_PIN  3

// DEBUG
#define DEBUG_UART USART0_P0

// SBUS
// enable SBUS output -> this will DISABLE ppm!
#define SBUS_ENABLED
#define SBUS_UART USART1_P0
// Note: default/futaba is INVERTED=1! (use this for a cc3d etc)
//       inverted     => idle = LOW
//       non inverted => idle = high
// #define SBUS_INVERTED

// PPM (only used if sbus is disabled)
// invert SBUS output (normal is non inverted)
// #define PPM_INVERTED

// hub telemetry input (soft serial)
// #define HUB_TELEMETRY_ON_SBUS_UART
// #define HUB_TELEMETRY_INVERTED

#ifndef HUB_TELEMETRY_ON_SBUS_UART
  // if not shared with sbus, use any pin in softserial mode at 9600baud
  #define HUB_TELEMETRY_PORT        P0
  #define HUB_TELEMETRY_PIN         2
#endif

// bootloader config
#define BOOTLOADER_LED_GREEN_PORT LED_GREEN_PORT
#define BOOTLOADER_LED_GREEN_PIN  LED_GREEN_PIN
#define BOOTLOADER_LED_RED_PORT   LED_RED_PORT
#define BOOTLOADER_LED_RED_PIN    LED_RED_PIN
// bootloader runs on UART1 P0
#define BOOTLOADER_UART_NUM           1
#define BOOTLOADER_UART_PORT          0
#define BOOTLOADER_UART_INVERTED      0
#define BOOTLOADER_UART_BAUDRATE      57600
#define BOOTLOADER_UART_USE_PARITY    0
#define BOOTLOADER_UART_USE_2STOPBITS 0
// use ISP interface clock pin as bootloader enable pin
// pull this low during startup to enter bootloader mode
#define BOOTLOADER_ENABLE_PORT P2
#define BOOTLOADER_ENABLE_PIN  2


#define DEFAULT_FSCAL_VALUE -69

