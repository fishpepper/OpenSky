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

uint8_t hal_cc25xx_get_register(uint8_t address){
	uint8_t result;
	
	//select device:
	hal_spi_csn_lo();
	//wait for RDY signal:
	//while(CC2500_SPI_MISO_PIN & (1<<CC2500_SPI_MISO_PN)){}
	
	//request address (read request has bit7 set)
	hal_spi_tx(address | 0x80);
	
	//fetch result:
	result = hal_spi_rx();
	
	//deselect device
	hal_spi_csn_hi();
	
	//return result
	return(result); 
} 

inline void hal_cc25xx_strobe(uint8_t address){
    hal_spi_csn_lo();
    hal_spi_tx(address);
    hal_spi_csn_hi();
}


