#ifndef __HAL_UART_H__
#define __HAL_UART_H__

#include "hal_cc25xx.h"
#include <stdint.h>

//for a 26MHz Crystal:
#define CC2510_BAUD_E_115200 12
#define CC2510_BAUD_E_57600  11
#define CC2510_BAUD_M_115200 34
#define CC2510_BAUD_M_57600  34

//use 155200 baud
#define UART_BAUD_M CC2510_BAUD_M_115200
#define UART_BAUD_E CC2510_BAUD_E_115200

void hal_uart_init(void);
void hal_uart_start_transmission(uint8_t ch);

#define hal_uart_int_enabled() (IEN2 & IEN2_UTX0IE)
#define hal_uart_int_enable() { sei(); }
#define hal_uart_int_disable() { cli(); }

union hal_uart_config_t{
  uint8_t byte;
  struct{
        uint8_t START : 1; //start bit level
        uint8_t STOP  : 1; //stop bit level
        uint8_t SPB   : 1; //stop bits (0=1, 1=2)
        uint8_t PARITY: 1; //parity (on/off)
        uint8_t BIT9  : 1; //9 bit mode
        uint8_t D9    : 1; //9th bit level or parity type
        uint8_t FLOW  : 1; //flow control
        uint8_t ORDER : 1; //data bit order (LSB or MSB first)
  } bit;
};

static void hal_uart_set_mode(__xdata union hal_uart_config_t *cfg);

void hal_uart_tx_interrupt(void) __interrupt UTX0_VECTOR;



#endif // __HAL_UART_H__
