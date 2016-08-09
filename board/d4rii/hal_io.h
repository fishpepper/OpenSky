#ifndef __HAL_IO_H__
#define __HAL_IO_H__
#include <stdint.h>

void hal_io_init(void);
uint8_t hal_io_bind_request(void);
static uint8_t hal_io_bind_request_pressed(void);

#endif // __HAL_IO_H__
