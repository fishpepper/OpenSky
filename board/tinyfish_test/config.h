// pin layout ISP header
#define ISP_DATA  P2_1
#define ISP_CLOCK P2_2

// pin layout servo connection
#define SERVO_1 7 // P0_7 = BIND, pull down on startup to enter bind mode
#define SERVO_2 6 // P0_6 = ADC1 = voltage sensor (max 3.3V on I/O ! -> voltage divider necessary!)
#define SERVO_3 5 // P0_5 = ADC0 = current sensor (max 3.3V on I/O !)
#define SERVO_4 4 // P0_4 = PPM or SBUS OUT
#define SERVO_5 3 // P0_3 = debug UART

#define PPM_OUT_PIN SERVO_4

// note: change of adc ch require change in adc.c!
#define ADC_PORT P0
// make sure not to use SERVO_3
#define ADC0 SERVO_2
#define ADC1 SERVO_2

// voltage divider on my board is 10 / 3.3 k, scale to 100 / 33 to avoid floating point calc
#define ADC0_DIVIDER_A 100
#define ADC0_DIVIDER_B  33

#define BIND_PORT P0
#define BIND_PIN  SERVO_1

// leds
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
#define HUB_TELEMETRY_ON_SBUS_UART
// #define HUB_TELEMETRY_INVERTED

#ifndef HUB_TELEMETRY_ON_SBUS_UART
  // if not shared with sbus, use any pin in softserial mode at 9600baud
  #define HUB_TELEMETRY_PORT        P0
  #define HUB_TELEMETRY_PIN         7
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
#define BOOTLOADER_UART_BAUDRATE      100000
#define BOOTLOADER_UART_USE_PARITY    1
#define BOOTLOADER_UART_USE_2STOPBITS 1
// use ISP interface clock pin as bootloader enable pin
// pull this low during startup to enter bootloader mode
#define BOOTLOADER_ENABLE_PORT P2
#define BOOTLOADER_ENABLE_PIN  2


#define DEFAULT_FSCAL_VALUE -20
