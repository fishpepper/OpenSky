#include "hal_cc25xx.h"
#include "hal_spi.h"
#include "stm32f10x_gpio.h"
#include "debug.h"

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
	uint8_t status = hal_spi_tx(address | 0x80);
	//debug_put_hex8(status);
	
	//fetch result:
	result = hal_spi_rx();
	
	//deselect device
	hal_spi_csn_hi();
	
	//return result
	return(result); 
}

inline void hal_cc25xx_strobe(uint8_t address){
    hal_spi_csn_lo();
    uint8_t status = hal_spi_tx(address);
    //debug("stobe = 0x"); debug_put_hex8(status); debug_put_newline();
    hal_spi_csn_hi();
}

void hal_cc25xx_enter_rxmode(void) {
	//add pa/lna config bit setting here
}

void hal_cc25xx_enable_receive(void){
	//this is called after freq tuning before activating SRX
}
