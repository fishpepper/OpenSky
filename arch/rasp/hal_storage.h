#ifndef __HAL_STORAGE__H_
#define __HAL_STORAGE__H_
#include <stdint.h>
#include "config.h"

void hal_storage_init(void);
void hal_storage_write(uint8_t *buffer, uint16_t len);
void hal_storage_read(uint8_t *storage_ptr, uint16_t len);

#endif // __HAL_STORAGE__H_
