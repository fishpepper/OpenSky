/*
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

   author: fishpepper <AT> gmail.com
*/
#include "hal_spi.h"
#include "debug.h"
#include "led.h"
#include "pin_config.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_spi.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_dma.h"


void hal_spi_init(void) {
    hal_spi_rcc_init();
    hal_spi_gpio_init();
    hal_spi_mode_init();
    hal_spi_dma_init();
    hal_spi_enable();
}

static void hal_spi_rcc_init(void) {
    // enable clocks
    RCC_APB2PeriphClockCmd(CC25XX_SPI_GPIO_CLK | RCC_APB2Periph_AFIO, ENABLE);
    RCC_APBxPeriphClockCmd(CC25XX_SPI_CLK_RCC, CC25XX_SPI_CLK, ENABLE);
}

static void hal_spi_enable(void) {
    SPI_Cmd(CC25XX_SPI, ENABLE);
}

static void hal_spi_mode_init(void) {
    SPI_InitTypeDef spi_init;

    // mode config
    spi_init.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    spi_init.SPI_Mode      = SPI_Mode_Master;
    spi_init.SPI_DataSize  = SPI_DataSize_8b;
    spi_init.SPI_CPOL      = SPI_CPOL_Low;
    spi_init.SPI_CPHA      = SPI_CPHA_1Edge;
    spi_init.SPI_NSS       = SPI_NSS_Soft;
    spi_init.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8; //3mhz
    spi_init.SPI_FirstBit  = SPI_FirstBit_MSB;
    spi_init.SPI_CRCPolynomial = 7;
    SPI_Init(CC25XX_SPI, &spi_init);
}



static void hal_spi_dma_init(void) {
    DMA_InitTypeDef dma_init;

    // Enable DMA1 Peripheral Clock
    RCC_AHBPeriphClockCmd(CC25XX_SPI_DMA_CLOCK, ENABLE);

    // Configure SPI RX Channel
    dma_init.DMA_DIR                = DMA_DIR_PeripheralSRC;
    dma_init.DMA_PeripheralBaseAddr = (uint32_t)&CC25XX_SPI->DR;
    dma_init.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    dma_init.DMA_PeripheralInc      = DMA_PeripheralInc_Disable;
    dma_init.DMA_MemoryBaseAddr     = 0; // will be set later
    dma_init.DMA_MemoryDataSize     = DMA_MemoryDataSize_Byte;
    dma_init.DMA_MemoryInc          = DMA_MemoryInc_Enable;
    dma_init.DMA_BufferSize         = 1; // will be set later
    dma_init.DMA_Mode               = DMA_Mode_Normal;
    dma_init.DMA_Priority           = DMA_Priority_VeryHigh;
    dma_init.DMA_M2M                = DMA_M2M_Disable;
    DMA_Init(CC25XX_SPI_RX_DMA_CHANNEL, &dma_init);

    // configure SPI TX Channel
    dma_init.DMA_DIR                = DMA_DIR_PeripheralDST;
    dma_init.DMA_PeripheralBaseAddr = (uint32_t)&CC25XX_SPI->DR;
    dma_init.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    dma_init.DMA_PeripheralInc      = DMA_PeripheralInc_Disable;
    dma_init.DMA_MemoryBaseAddr     = 0; // will be set later
    dma_init.DMA_MemoryDataSize     = DMA_MemoryDataSize_Byte;
    dma_init.DMA_MemoryInc          = DMA_MemoryInc_Enable;
    dma_init.DMA_BufferSize         = 1; // will be set later
    dma_init.DMA_Mode               = DMA_Mode_Normal;
    dma_init.DMA_Priority           = DMA_Priority_VeryHigh;
    dma_init.DMA_M2M                = DMA_M2M_Disable;
    DMA_Init(CC25XX_SPI_TX_DMA_CHANNEL, &dma_init);

    // start disabled
    DMA_Cmd(CC25XX_SPI_TX_DMA_CHANNEL, DISABLE);
    DMA_Cmd(CC25XX_SPI_RX_DMA_CHANNEL, DISABLE);

}

// data in buffer will be sent and will be overwritten with
// the data read back from the spi slave
void hal_spi_dma_xfer(uint8_t *buffer, uint8_t len) {
    //debug("xfer "); debug_put_uint8(len); debug(")\n");

    // TX: transfer buffer to slave
    CC25XX_SPI_TX_DMA_CHANNEL->CMAR  = (uint32_t)buffer;
    CC25XX_SPI_TX_DMA_CHANNEL->CNDTR = len;

    // RX: read back data from slave
    CC25XX_SPI_RX_DMA_CHANNEL->CMAR  = (uint32_t)buffer;
    CC25XX_SPI_RX_DMA_CHANNEL->CNDTR = len;

    // enable both dma
    DMA_Cmd(CC25XX_SPI_RX_DMA_CHANNEL, ENABLE);
    DMA_Cmd(CC25XX_SPI_TX_DMA_CHANNEL, ENABLE);

    //debug("DMA EN\n"); debug_flush();

    // trigger the SPI TX + RX dma
    SPI_I2S_DMACmd(CC25XX_SPI, SPI_I2S_DMAReq_Rx | SPI_I2S_DMAReq_Tx, ENABLE);

    //debug("TRIG\n"); debug_flush();
#if 0
    // Wait until the command is sent to the DR
    while (!DMA_GetFlagStatus(CC25XX_SPI_TX_DMA_TC_FLAG)) {};

    //debug("ACTIVE\n"); debug_flush();

    // wait for tx to be finished:
    while (DMA_GetFlagStatus(CC25XX_SPI_TX_DMA_TC_FLAG)) {};
    while (DMA_GetFlagStatus(CC25XX_SPI_RX_DMA_TC_FLAG)) {};

    //wait for SPI to be no longer busy
    while (SPI_I2S_GetFlagStatus(CC25XX_SPI, SPI_I2S_FLAG_BSY) != RESET){}
    //debug("!BUSY\n"); debug_flush();
#endif

    while (SPI_I2S_GetFlagStatus(CC25XX_SPI, SPI_I2S_FLAG_TXE) == RESET);
    while (SPI_I2S_GetFlagStatus(CC25XX_SPI, SPI_I2S_FLAG_BSY) != RESET){}

    //while((SPI1->SR & 2) == 0);  // wait while TXE flag is 0 (TX is not empty)
    //while((SPI1->SR & (1 << 7)) != 0);  // wait while BSY flag is 1 (SPI is busy)

    //disable DMA
    DMA_Cmd(CC25XX_SPI_RX_DMA_CHANNEL, DISABLE);
    DMA_Cmd(CC25XX_SPI_TX_DMA_CHANNEL, DISABLE);

    // clear DMA flags
    SPI_I2S_DMACmd(CC25XX_SPI,SPI_I2S_DMAReq_Rx | SPI_I2S_DMAReq_Tx, DISABLE);
}


static void hal_spi_gpio_init(void) {
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
}

uint8_t hal_spi_tx(uint8_t address){
    // wait for SPI Tx buffer empty
    while (SPI_I2S_GetFlagStatus(CC25XX_SPI, SPI_I2S_FLAG_TXE) == RESET);
    // send SPI data
    SPI_I2S_SendData(CC25XX_SPI, address);

    // read response
    while (SPI_I2S_GetFlagStatus(CC25XX_SPI, SPI_I2S_FLAG_RXNE) == RESET);
    uint8_t result = SPI_I2S_ReceiveData(CC25XX_SPI);
    return result;
}


uint8_t hal_spi_rx(void) {
    return hal_spi_tx(0xff);
}
