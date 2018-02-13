#ifndef __HAL_IO_H__
#define __HAL_IO_H__
#include <stdint.h>

void hal_io_init(void);
uint8_t hal_io_bind_request(void);

void hal_io_wait_miso_low();
uint8_t hal_io_get_gdo();
void hal_io_enable_pa(uint8_t enable);
void hal_io_enable_lna(uint8_t enable);
void hal_io_debug(uint8_t enable);
void hal_io_debug2(uint8_t enable);
void hal_io_set_ppm(uint8_t enable);

#endif // __HAL_IO_H__
