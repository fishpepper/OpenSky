#ifndef __HAL_SPI__H_
#define __HAL_SPI__H_
#include <stdint.h>
#include "config.h"

void hal_spi_init(void);

uint8_t hal_spi_tx(uint8_t address);
uint8_t hal_spi_rx(void);
uint8_t hal_spi_read_address(uint8_t address);
void hal_spi_csn_lo();
void hal_spi_csn_hi();

#endif // __HAL_SPI__H_

