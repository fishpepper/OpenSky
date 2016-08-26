#ifndef __SOFT_SERIAL_H__
#define __SOFT_SERIAL_H__
#include <stdint.h>
#include "hal_soft_serial.h"
#include "main.h"

typedef void (*soft_serial_rx_callback_t)(uint8_t);

void soft_serial_init(void);
void soft_serial_set_rx_callback(soft_serial_rx_callback_t cb);

void soft_serial_process_startbit(void);
uint8_t soft_serial_process_databit(void);

#endif // __SOFT_SERIAL_H__
