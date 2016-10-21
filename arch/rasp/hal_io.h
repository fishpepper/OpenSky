#ifndef __HAL_IO_H__
#define __HAL_IO_H__
#include <stdint.h>

void hal_io_init(void);
uint8_t hal_io_bind_request(void);
int hal_spi_get_gdo0(void);

#define HAL_PA 1
#define HAL_LNA 0

void hal_set_amp(int pa);

void hal_set_ppm(int state);

#endif // __HAL_IO_H__
