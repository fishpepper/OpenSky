#ifndef __HAL_SOFT_SPI__
#define __HAL_SOFT_SPI__
#include <stdint.h>
#include "config.h"
#include "main.h"
#include "stm32f10x_gpio.h"

void hal_soft_spi_init(void);

#define SOFT_SPI_CLOCK_HI() { SOFT_SPI_GPIO->BSRR = SOFT_SPI_SCK; }
#define SOFT_SPI_CLOCK_LO() { SOFT_SPI_GPIO->BRR  = SOFT_SPI_SCK; }
#define SOFT_SPI_MOSI_HI()  { SOFT_SPI_GPIO->BSRR = SOFT_SPI_MOSI; }
#define SOFT_SPI_MOSI_LO()  { SOFT_SPI_GPIO->BRR  = SOFT_SPI_MOSI; }

#define SOFT_SPI_DATA_TO_MOSI() {if (soft_spi_tx_byte & 0x80){ SOFT_SPI_MOSI_HI(); }else{ SOFT_SPI_MOSI_LO(); };}


#endif
