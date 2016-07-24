#include "hal_cc25xx.h"
#include "hal_spi.h"
#include "stm32f10x_gpio.h"

void hal_cc25xx_init(void){
	hal_spi_init();
}

void hal_cc25xx_set_register(uint8_t address, uint8_t data){
	hal_spi_csn_lo();
	hal_spi_tx(address);
	hal_spi_tx(data);
	hal_spi_csn_hi();
}

inline void hal_cc25xx_strobe(uint8_t address){
    hal_spi_csn_lo();
    hal_spi_tx(address);
    hal_spi_csn_hi();
}


