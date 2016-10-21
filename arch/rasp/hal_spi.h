#ifndef __HAL_SPI__H_
#define __HAL_SPI__H_
#include <stdint.h>
#include "config.h"
#include "delay.h"

void hal_spi_init(void);
void hal_spi_dma_xfer(uint8_t *buffer, uint8_t len);

#endif // __HAL_SPI__H_

