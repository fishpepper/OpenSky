#ifndef __UART_H__
#define __UART_H__
#include <stdint.h>

typedef void (*uart_rx_callback_t)(uint8_t);
extern volatile uart_rx_callback_t uart_rx_callback;

void uart_init(void);
void uart_start_transmission(uint8_t *buf, uint8_t len);
void uart_set_rx_callback(uart_rx_callback_t cb);

#endif

