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

inline void hal_cc25xx_set_antenna(uint8_t id){
	//select antenna 0 or 1:
	if (id) {
		CC25XX_ANT_SW_CTX_GPIO->BRR  = (CC25XX_ANT_SW_CTX_PIN); //0
		CC25XX_ANT_SW_CRX_GPIO->BSRR = (CC25XX_ANT_SW_CRX_PIN); //1
	} else {
		CC25XX_ANT_SW_CTX_GPIO->BSRR = (CC25XX_ANT_SW_CTX_PIN); //1
		CC25XX_ANT_SW_CRX_GPIO->BRR  = (CC25XX_ANT_SW_CRX_PIN); //0
	}
}


void hal_cc25xx_disable_rf_interrupt(void) {
	//nothing to do here
}

void hal_cc25xx_setup_rf_dma(uint8_t mode) {
	//nothign to do
}

inline void hal_cc25xx_set_gdo_mode(void) {
	cc25xx_set_register(IOCFG0, 0x06);
	//cc25xx_set_register(IOCFG1, ???);
	cc25xx_set_register(IOCFG2, 0x06);
}

inline void hal_cc25xx_set_register(uint8_t address, uint8_t data){
	hal_spi_csn_lo();
	hal_spi_tx(address);
	hal_spi_tx(data);
	hal_spi_csn_hi();
}

inline uint8_t hal_cc25xx_get_register(uint8_t address){
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

inline void hal_cc25xx_enter_rxmode(void) {
	//add pa/lna config bit setting here
	CC25XX_LNA_SW_CRX_GPIO->BSRR = (CC25XX_LNA_SW_CRX_PIN); //1
	CC25XX_LNA_SW_CTX_GPIO->BRR  = (CC25XX_ANT_SW_CTX_PIN); //0
}

inline void hal_cc25xx_enter_txmode(void) {
	//add pa/lna config bit setting here
	CC25XX_LNA_SW_CRX_GPIO->BRR  = (CC25XX_LNA_SW_CRX_PIN); //0
	CC25XX_LNA_SW_CTX_GPIO->BSRR = (CC25XX_ANT_SW_CTX_PIN); //1
}


inline void hal_cc25xx_enable_receive(void){
	//this is called after freq tuning before activating SRX
}

void hal_cc25xx_enable_transmit(void) {
	//FIXME
}

inline uint8_t hal_cc25xx_get_gdo_status(void) {
	if (GPIO_ReadInputDataBit(CC25XX_GDO2_GPIO, GPIO_Pin_3)){
		return 1;
	}else{
		return 0;
	}
}

inline void hal_cc25xx_read_fifo(uint8_t *buf, uint8_t len){
    hal_cc25xx_register_read_multi(CC25XX_FIFO | READ_FLAG | BURST_FLAG, buf, len);
}

inline void hal_cc25xx_register_read_multi(uint8_t address, uint8_t *buf, uint8_t len){
    //select device:
    hal_spi_csn_lo();
    
    //wait for RDY signal:
    //while(CC2500_SPI_MISO_PIN & (1<<CC2500_SPI_MISO_PN)){}
    
    //request address (read request)
    uint8_t result = hal_spi_tx(address);
    
    while(len--){
        *buf = hal_spi_rx();
        buf++;
    }
    
    //deselect device
    hal_spi_csn_hi();
}

inline void hal_cc25xx_register_write_multi(uint8_t address, uint8_t *buf, uint8_t len){
    //select device:
    hal_spi_csn_lo();

    //wait for RDY signal:
    //while(CC2500_SPI_MISO_PIN & (1<<CC2500_SPI_MISO_PN)){}
    
    //request address (write request)
    hal_spi_tx(address | READ_FLAG | BURST_FLAG);
    while(len--){
        hal_spi_tx(*buf);
        buf++;
    }
    //deselect device
    hal_spi_csn_lo();
}

inline void hal_cc25xx_process_packet(volatile uint8_t *packet_received, volatile uint8_t *buffer, uint8_t maxlen){
	 if(hal_cc25xx_get_gdo_status() == 0){
		//data received, fetch data
		uint8_t len = hal_cc25xx_get_register_burst(RXBYTES);
		
                //packet received, grab data
                uint8_t tmp_buffer[len];
                hal_cc25xx_read_fifo(tmp_buffer, len);
		
		if (len != 0) debug_put_hex8(len);
		
		//only accept valid packet lenbghts:
		if (len == maxlen){
			uint8_t i;
			for(i=0; i<maxlen; i++){
				buffer[i] = tmp_buffer[i];
			}
			*packet_received = 1;
		}
	 }
}

void hal_cc25xx_transmit_packet(volatile uint8_t *buffer, uint8_t len) {
	//flush rx fifo
	hal_cc25xx_strobe(RFST_SFRX);
	//why that delay?!
	delay_us(25);
	//flush tx fifo
	hal_cc25xx_strobe(RFST_SFTX);
	//copy to fifo
	hal_cc25xx_register_write_multi(CC25XX_FIFO, (uint8_t *)buffer, buffer[0]+1);
	//and send!
	hal_cc25xx_strobe(RFST_STX);
}