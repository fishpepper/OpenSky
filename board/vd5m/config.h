#define LED_GREEN_PORT P2
#define LED_GREEN_PIN  4
#define LED_RED_PORT P2
#define LED_RED_PIN  3

//DEBUG
#define DEBUG_UART USART0_P0

// SBUS
// enable SBUS output -> this will DISABLE ppm!
#define SBUS_ENABLED
#define SBUS_UART USART1_P0
// Note: default/futaba is INVERTED=1! (use this for a cc3d etc)
//       inverted     => idle = LOW
//       non inverted => idle = high
#define SBUS_INVERTED

// PPM (only used if sbus is disabled)
// invert SBUS output (normal is non inverted)
//#define PPM_INVERTED

//hub telemetry input (soft serial)
#define HUB_TELEMETRY_ON_SBUS_UART
#define HUB_TELEMETRY_INVERTED

#ifndef HUB_TELEMETRY_ON_SBUS_UART
  // if not shared with sbus, use any pin in softserial mode at 9600baud
  #define HUB_TELEMETRY_PORT        P0
  #define HUB_TELEMETRY_PIN         7
#endif

//note: change of adc ch require change in adc.c!
#define ADC_PORT P0
#ifdef HUB_TELEMETRY_ON_SBUS_UART
  //free up SERVO_3 for hub telemetry input!
  #define ADC0 SERVO_2
  #define ADC1 SERVO_2
#else
  #define ADC0 SERVO_2
  #define ADC1 SERVO_3
#endif
//acs712 in inverted mode on adc1?
//when powered by 5V we can use a trick
//to get a good resolution: use inverted power inputs
// 0A = 2.5V
//30A = 0.0V
#define ADC1_USE_ACS712

//voltage divider on my board is 10 / 3.3 k, scale to 100 / 33 to avoid floating point calc
#define ADC0_DIVIDER_A 100
#define ADC0_DIVIDER_B  33

//bootloader config
#define BOOTLOADER_LED_GREEN_PORT LED_GREEN_PORT
#define BOOTLOADER_LED_GREEN_PIN  LED_GREEN_PIN
#define BOOTLOADER_LED_RED_PORT   LED_RED_PORT
#define BOOTLOADER_LED_RED_PIN    LED_RED_PIN
//bootloader runs on UART1 P0
#define BOOTLOADER_UART_NUM           1
#define BOOTLOADER_UART_PORT          0
#define BOOTLOADER_UART_INVERTED      1
#define BOOTLOADER_UART_BAUDRATE      100000
#define BOOTLOADER_UART_USE_PARITY    1
#define BOOTLOADER_UART_USE_2STOPBITS 1
// use ISP interface clock pin as bootloader enable pin
// pull this low during startup to enter bootloader mode
#define BOOTLOADER_ENABLE_PORT P2
#define BOOTLOADER_ENABLE_PIN  2


#define DEFAULT_FSCAL_VALUE -20
