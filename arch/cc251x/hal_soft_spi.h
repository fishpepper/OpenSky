#ifndef __HAL_SOFT_SPI__
#define __HAL_SOFT_SPI__
#include <stdint.h>
#include "config.h"
#include "main.h"
#include "hal_cc25xx.h"
#include "portmacros.h"

void hal_soft_spi_init(void);

//SOFTSPI
#define SOFT_SPI_CLOCK_DIR PORT2DIR(SOFT_SPI_CLOCK_PORT)
#define SOFT_SPI_CLOCK_BIT PORT2BIT(SOFT_SPI_CLOCK_PORT, SOFT_SPI_CLOCK_PIN)
#define SOFT_SPI_MOSI_DIR  PORT2DIR(SOFT_SPI_MOSI_PORT)
#define SOFT_SPI_MOSI_BIT  PORT2BIT(SOFT_SPI_MOSI_PORT, SOFT_SPI_MOSI_PIN)

#define SOFT_SPI_CLOCK_HI()  { SOFT_SPI_CLOCK_BIT = 1; }
#define SOFT_SPI_CLOCK_LO()  { SOFT_SPI_CLOCK_BIT = 0; }
#define SOFT_SPI_MOSI_HI()  { SOFT_SPI_MOSI_BIT = 1; }
#define SOFT_SPI_MOSI_LO()  { SOFT_SPI_MOSI_BIT = 0; }

#endif
