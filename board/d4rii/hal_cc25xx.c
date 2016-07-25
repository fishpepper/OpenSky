#include "hal_cc25xx.h"
#include "hal_spi.h"
#include "cc25xx.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "debug.h"

void hal_cc25xx_init(void){
	hal_spi_init();
	_hal_cc25xx_gpio_init();
}

void _hal_cc25xx_gpio_init(void) {
	GPIO_InitTypeDef gpio_init;
	
	// antenna switch
	// periph clock enable for port
	RCC_APB2PeriphClockCmd(CC25XX_ANT_SW_CTX_GPIO_CLK, ENABLE);
	RCC_APB2PeriphClockCmd(CC25XX_ANT_SW_CRX_GPIO_CLK, ENABLE);
	
	//CTX:
	gpio_init.GPIO_Pin = CC25XX_ANT_SW_CTX_PIN;
	gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
	gpio_init.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(CC25XX_ANT_SW_CTX_GPIO, &gpio_init);
	//CRX:
	gpio_init.GPIO_Pin = CC25XX_ANT_SW_CRX_PIN;
	gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
	gpio_init.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(CC25XX_ANT_SW_CRX_GPIO, &gpio_init);
	
	//select first antenna
	hal_cc25xx_set_antenna(0);
	
	//PA/LNA:
	// periph clock enable for port
	RCC_APB2PeriphClockCmd(CC25XX_LNA_SW_CTX_GPIO_CLK, ENABLE);
	RCC_APB2PeriphClockCmd(CC25XX_LNA_SW_CRX_GPIO_CLK, ENABLE);
	
	//CTX:
	gpio_init.GPIO_Pin = CC25XX_LNA_SW_CTX_PIN;
	gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
	gpio_init.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(CC25XX_LNA_SW_CTX_GPIO, &gpio_init);
	//CRX:
	gpio_init.GPIO_Pin = CC25XX_LNA_SW_CRX_PIN;
	gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
	gpio_init.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(CC25XX_LNA_SW_CRX_GPIO, &gpio_init);
	hal_cc25xx_enter_rxmode();
	
	//GDO2
	// periph clock enable for port
	RCC_APB2PeriphClockCmd(CC25XX_GDO2_GPIO_CLK, ENABLE);
	// configure GDO2 pin as Input floating
	gpio_init.GPIO_Pin  = CC25XX_GDO2_PIN;
	gpio_init.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(CC25XX_GDO2_GPIO, &gpio_init);
}

void hal_cc25xx_set_antenna(uint8_t id){
	//select antenna 0 or 1:
	if (id) {
		CC25XX_ANT_SW_CTX_GPIO->BRR  = (CC25XX_ANT_SW_CTX_PIN); //0
		CC25XX_ANT_SW_CRX_GPIO->BSRR = (CC25XX_ANT_SW_CRX_PIN); //1
	} else {
		CC25XX_ANT_SW_CTX_GPIO->BSRR = (CC25XX_ANT_SW_CTX_PIN); //1
		CC25XX_ANT_SW_CRX_GPIO->BRR  = (CC25XX_ANT_SW_CRX_PIN); //0
	}
}

void hal_cc25xx_set_gdo_mode(void) {
	cc25xx_set_register(IOCFG0, 0x06);
	//cc25xx_set_register(IOCFG1, ???);
	cc25xx_set_register(IOCFG2, 0x06);
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
	CC25XX_LNA_SW_CRX_GPIO->BSRR = (CC25XX_LNA_SW_CRX_PIN); //1
	CC25XX_LNA_SW_CTX_GPIO->BRR  = (CC25XX_ANT_SW_CTX_PIN); //0
}

void hal_cc25xx_enter_txmode(void) {
	//add pa/lna config bit setting here
	CC25XX_LNA_SW_CRX_GPIO->BRR  = (CC25XX_LNA_SW_CRX_PIN); //0
	CC25XX_LNA_SW_CTX_GPIO->BSRR = (CC25XX_ANT_SW_CTX_PIN); //1
}


void hal_cc25xx_enable_receive(void){
	//this is called after freq tuning before activating SRX
}
