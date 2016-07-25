#include "hal_spi.h"
#include "debug.h"
#include "pin_config.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_spi.h"
#include "stm32f10x_rcc.h"

void hal_spi_init(void) {
	_hal_spi_rcc_init();
	_hal_spi_gpio_init();
	_hal_spi_mode_init();
	_hal_spi_enable();
}

void _hal_spi_rcc_init(void) {
	// enable clocks
	RCC_APB2PeriphClockCmd(CC25XX_SPI_GPIO_CLK | RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
}

void _hal_spi_enable(void) {
	SPI_Cmd(CC25XX_SPI, ENABLE);
}

void _hal_spi_mode_init(void) {
	SPI_InitTypeDef spi_init;
	
	// mode config
	spi_init.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	spi_init.SPI_Mode      = SPI_Mode_Master;
	spi_init.SPI_DataSize  = SPI_DataSize_8b;
	spi_init.SPI_CPOL      = SPI_CPOL_Low;
	spi_init.SPI_CPHA      = SPI_CPHA_2Edge;
	spi_init.SPI_NSS       = SPI_NSS_Soft;
	spi_init.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;
	spi_init.SPI_FirstBit  = SPI_FirstBit_MSB;
	spi_init.SPI_CRCPolynomial = 7;
	SPI_Init(CC25XX_SPI, &spi_init);
}

void _hal_spi_gpio_init(void) {
	GPIO_InitTypeDef gpio_init;

	// configure SCK and MOSI pins as Alternate Function Push-Pull
	gpio_init.GPIO_Pin   = CC25XX_SPI_SCK_PIN | CC25XX_SPI_MOSI_PIN;
	gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
	gpio_init.GPIO_Mode  = GPIO_Mode_AF_PP;
	GPIO_Init(CC25XX_SPI_GPIO, &gpio_init);
  
	// configure CSN as Push-Pull
	gpio_init.GPIO_Pin   = CC25XX_SPI_CSN_PIN;
	gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
	gpio_init.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_Init(CC25XX_SPI_GPIO, &gpio_init);
	
	// configure MISO pin as Input floating
	gpio_init.GPIO_Pin  = CC25XX_SPI_MISO_PIN;
	gpio_init.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(CC25XX_SPI_GPIO, &gpio_init);
	
	// configure GDO2 pin as Input floating
	gpio_init.GPIO_Pin  = CC25XX_SPI_GDO2_PIN;
	gpio_init.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(CC25XX_SPI_GPIO, &gpio_init);
}

void hal_spi_tx(uint8_t address){
	// wait for SPI Tx buffer empty
	while (SPI_I2S_GetFlagStatus(CC25XX_SPI, SPI_I2S_FLAG_TXE) == RESET);
	// send SPI data
	SPI_I2S_SendData(CC25XX_SPI, address);
}

uint8_t hal_spi_rx(void) {
	hal_spi_tx(0xFF);
	
	// read data
	uint8_t result = SPI_I2S_ReceiveData(CC25XX_SPI);
	return result;
}
