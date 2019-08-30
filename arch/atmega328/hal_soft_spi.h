#ifndef __HAL_SOFT_SPI__
#define __HAL_SOFT_SPI__
#include <stdint.h>
#include "config.h"
#include "main.h"

void hal_soft_spi_init(void);

#define SOFT_SPI_CLOCK_HI() { }
#define SOFT_SPI_CLOCK_LO() { }
#define SOFT_SPI_MOSI_HI()  { }
#define SOFT_SPI_MOSI_LO()  { }


#endif
