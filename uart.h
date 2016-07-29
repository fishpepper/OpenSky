#ifndef __UART_H__
#define __UART_H__
#include "main.h"
#include <stdint.h>

void uart_init(void);
void uart_test(void);
void uart_putc(uint8_t ch);
void uart_flush(void);
void uart_puts(uint8_t *data);
void uart_put_hex8(uint8_t val);
void uart_put_uint8(uint8_t c);
void uart_put_int8(int8_t c);
void uart_put_uint16(uint16_t c);
void uart_put_newline(void);

#define UART_TX_BUFFER_SIZE 128
#if ((UART_TX_BUFFER_SIZE==128) || (UART_TX_BUFFER_SIZE==64) || (UART_TX_BUFFER_SIZE==32))
    //ALWAYS use 2^n for buffer size! -> faster code and no int16 in interrupts (see Readme.md)
    #define UART_TX_BUFFER_AND_OPERAND (UART_TX_BUFFER_SIZE-1)
#else
    //FAIL!
    #warn "ALWAYS USE POWER OF TWO FOR BUFFER SIZE!"
#endif
extern EXTERNAL_MEMORY uint8_t uart_tx_buffer[UART_TX_BUFFER_SIZE];
extern volatile EXTERNAL_MEMORY uint8_t uart_tx_buffer_in;
extern volatile EXTERNAL_MEMORY uint8_t uart_tx_buffer_out;

#endif
