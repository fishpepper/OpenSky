#ifndef __HAL_IO_H__
#define __HAL_IO_H_
#include "portmacros.h"
#include "config.h"
#include <stdint.h>

void hal_io_init(void);
uint8_t hal_io_bind_request(void);

#endif // __HAL_IO_H__
