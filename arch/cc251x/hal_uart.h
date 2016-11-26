#ifndef __HAL_UART_H_
#define __HAL_UART_H_

#include <stdint.h>
#include "hal_defines.h"

//for a 26MHz Crystal:
#define CC2510_BAUD_E_115200  12
#define CC2510_BAUD_M_115200  34
//best match for 100kbit/s = 99975.5859375 bit/s
//baudrate = (((256.0 + baud_m)*2.0**baud_e) / (2**28)) * 26000000.0
#define CC2510_BAUD_E_100000  11
#define CC2510_BAUD_M_100000 248
#define CC2510_BAUD_E_57600   11
#define CC2510_BAUD_M_57600   34

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

void hal_uart_init(void);
static void hal_uart_set_mode(EXTERNAL_MEMORY union hal_uart_config_t *cfg);
void hal_uart_start_transmission(uint8_t *data, uint8_t len);

#ifdef HUB_TELEMETRY_ON_SBUS_UART
  #define HAL_UART_RX_ISR(void) hal_uart_rx_interrupt(void) __interrupt URX1_VECTOR
  #define HAL_UART_RX_ISR_CLEAR_FLAG() { URX1IF = 0; }
  #define HAL_UART_RX_GETCH() (U1DBUF)
  
  void HAL_UART_RX_ISR(void);
#endif

#endif  // __HAL_UART_H_
