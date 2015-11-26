#ifndef __UART_H__
#define __UART_H__
#include "cc2510fx.h"
#include <stdint.h>

union uart_config_t{
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

void uart_init(void);
void uart_test(void);
void uart_set_mode(__xdata union uart_config_t *cfg);
void uart_putc(uint8_t ch);
void uart_flush(void);
void uart_puts(uint8_t *data);
void uart_put_hex8(uint8_t val);
void uart_put_uint8(uint8_t c);
void uart_put_int8(int8_t c);
void uart_put_uint16(uint16_t c);
void uart_put_newline(void);

#if DEBUG
void putchar(char c);
#endif

void uart_tx_interrupt(void) __interrupt UTX0_VECTOR;

#define UART_TX_BUFFER_SIZE 128
#if ((UART_TX_BUFFER_SIZE==128) || (UART_TX_BUFFER_SIZE==64) || (UART_TX_BUFFER_SIZE==32))
    //ALWAYS use 2^n for buffer size! -> faster code and no int16 in interrupts (see Readme.md)
    #define UART_TX_BUFFER_AND_OPERAND (UART_TX_BUFFER_SIZE-1)
#else
    //FAIL!
    #warn "ALWAYS USE POWER OF TWO FOR BUFFER SIZE!"
#endif
extern __xdata uint8_t uart_tx_buffer[UART_TX_BUFFER_SIZE];
extern volatile __xdata uint8_t uart_tx_buffer_in;
extern volatile __xdata uint8_t uart_tx_buffer_out;

//for a 26MHz Crystal:
#define CC2510_BAUD_E_115200 12
#define CC2510_BAUD_E_57600  11
#define CC2510_BAUD_M_115200 34
#define CC2510_BAUD_M_57600  34


#endif
