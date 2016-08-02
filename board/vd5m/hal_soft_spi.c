#include "hal_soft_spi.h"
#include "soft_spi.h"

void hal_soft_spi_init(void) {
    //set to outputs:
    SOFT_SPI_CLOCK_DIR |= (1<<SOFT_SPI_CLOCK_PIN);

    SOFT_SPI_MOSI_DIR  |= (1<<SOFT_SPI_MOSI_PIN);
}
