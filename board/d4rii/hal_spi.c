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
    hal_spi_dma_setup();
    hal_spi_enable();
}

static void hal_spi_rcc_init(void) {
    // enable clocks
    RCC_APB2PeriphClockCmd(CC25XX_SPI_GPIO_CLK | RCC_APB2Periph_AFIO, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
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

static void hal_spi_dma_setup(void) {
    DMA_InitTypeDef dma_init;

    GNARR USE hal_spi_dma_setup()!

    test https://www.mikrocontroller.net/attachment/59501/spi_dma.c

    // Enable DMA1 Peripheral Clock (SPI_DECAWAVE and SPI_BUS)
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

    // Configure SPI_BUS RX Channel
    dma_init.DMA_DIR = DMA_DIR_PeripheralSRC; // From SPI to memory
    dma_init.DMA_PeripheralBaseAddr = (uint32_t)&CC25XX_SPI->DR;
    dma_init.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    dma_init.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    dma_init.DMA_MemoryBaseAddr = 0; // To be set later
    dma_init.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    dma_init.DMA_MemoryInc = DMA_MemoryInc_Enable;
    dma_init.DMA_BufferSize = 1; // To be set later
    dma_init.DMA_Mode = DMA_Mode_Normal;
    dma_init.DMA_Priority = DMA_Priority_VeryHigh;
    dma_init.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMA1_Channel2, &dma_init);

    // Configure SPI_BUS TX Channel
    dma_init.DMA_DIR = DMA_DIR_PeripheralDST; // From memory to SPI
    dma_init.DMA_PeripheralBaseAddr = (uint32_t)&CC25XX_SPI->DR;
    dma_init.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    dma_init.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    dma_init.DMA_MemoryBaseAddr = 0; // To be set later
    dma_init.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    dma_init.DMA_MemoryInc = DMA_MemoryInc_Enable;
    dma_init.DMA_BufferSize = 1; // To be set later
    dma_init.DMA_Mode = DMA_Mode_Normal;
    dma_init.DMA_Priority = DMA_Priority_VeryHigh;
    dma_init.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMA1_Channel3, &dma_init);

    DMA_Cmd(DMA1_Channel2, DISABLE); // RX
    DMA_Cmd(DMA1_Channel3, DISABLE); // TX

}

void hal_spi_tx_buffer_dma(uint8_t *buffer, uint8_t len) {
    DMA1_Channel2->CMAR  = (uint32_t)buffer;
    DMA1_Channel3->CMAR  = (uint32_t)buffer;
    DMA1_Channel2->CNDTR = len;
    DMA1_Channel3->CNDTR = len;

    DMA_Cmd(DMA1_Channel2, ENABLE); // RX
    DMA_Cmd(DMA1_Channel3, ENABLE); // TX

    while(1);
#if 0
    while (!(DMA1->ISR & DMA_ISR_TCIF4)) ;		// wait until RX DMA complete
    DMA1->IFCR = DMA_IFCR_CGIF4 | DMA_IFCR_CGIF5;	// clear DMA interrupt flags

    BBPeriphMask(DMA1_Channel4->CCR, DMA_CCR_EN) = 0;	// disable RX DMA
    BBPeriphMask(DMA1_Channel5->CCR, DMA_CCR_EN) = 0;	// disable TX DMA



    // Prepare the DMA
       DMA1_Channel3->CNDTR = len;
       DMA1_Channel3->CMAR = (uint32_t)buffer;

       // Enable the DMAs - They will await signals from the SPI hardware
       DMA_Cmd(DMA1_Channel3, ENABLE); // TX

       // Enable the SPI communication to the TX DMA, which will send the command
       SPI_I2S_DMACmd(CC25XX_SPI, SPI_I2S_DMAReq_Tx, ENABLE);
led_red_on();
       // Wait until the command is sent to the DR
       while (!DMA_GetFlagStatus(DMA1_FLAG_TC3));
led_green_on();
       // Wait until the transmission is completed
       while (SPI_I2S_GetFlagStatus(CC25XX_SPI, SPI_I2S_FLAG_TXE) == RESET);
       while (SPI_I2S_GetFlagStatus(CC25XX_SPI, SPI_I2S_FLAG_BSY) == RESET);

       // Disable the TX DMA and clear DMA flags
       SPI_I2S_DMACmd(CC25XX_SPI, SPI_I2S_DMAReq_Tx, DISABLE);
       DMA_Cmd(DMA1_Channel3, DISABLE);
       DMA_ClearFlag(DMA1_FLAG_GL3 | DMA1_FLAG_HT3 | DMA1_FLAG_TC3);
       //NOTE: I checked the SPI OVR flag here, and it wasn't set...

       DMA_Cmd(DMA1_Channel3, DISABLE); // TX
#endif
}

void hal_spi_rx_buffer_dma(uint8_t *buffer, uint8_t len) {
    uint8_t txbuf[len];
    uint8_t i;
    for(i=0; i<len; i++) txbuf[i] = 0xFF;

    // wait for SPI Tx buffer empty
    //while (SPI_I2S_GetFlagStatus(CC25XX_SPI, SPI_I2S_FLAG_TXE) == RESET);

    // prepare the DMA
    DMA1_Channel3->CNDTR = len;
    DMA1_Channel3->CMAR = (uint32_t)txbuf;

    //DMA1_Channel2->CNDTR = len;
    //DMA1_Channel2->CMAR = (uint32_t)buffer;

    // Enable the DMA - will await signals from the SPI hardware
    DMA_Cmd(DMA1_Channel3, ENABLE); // TX
  //  DMA_Cmd(DMA1_Channel2, ENABLE); // RX

    // Enable the SPI communication to the TX DMA, which will send the command
    SPI_I2S_DMACmd(CC25XX_SPI, SPI_I2S_DMAReq_Tx, ENABLE);
  //  SPI_I2S_DMACmd(CC25XX_SPI, SPI_I2S_DMAReq_Rx, ENABLE);

    // Wait until the command is sent to the DR
    while (!DMA_GetFlagStatus(DMA1_FLAG_TC3));

    // Wait until the transmission is completed
    while (SPI_I2S_GetFlagStatus(CC25XX_SPI, SPI_I2S_FLAG_TXE) == RESET);
    while (SPI_I2S_GetFlagStatus(CC25XX_SPI, SPI_I2S_FLAG_BSY) == RESET);

    // Disable the TX DMA and clear DMA flags
    SPI_I2S_DMACmd(CC25XX_SPI, SPI_I2S_DMAReq_Tx, DISABLE);
    DMA_Cmd(DMA1_Channel3, DISABLE);
    DMA_ClearFlag(DMA1_FLAG_GL5 | DMA1_FLAG_HT5 | DMA1_FLAG_TC3);






    // Enable the DMA - will await signals from the SPI hardware


    // Enable SPI communication to the RX DMA, which should receive the data

    debug("DMA RX\n");
    while(1);
    // Wait until the data is received
    while (!DMA_GetFlagStatus(DMA1_FLAG_TC2));

    // Disable the DMAs
    DMA_Cmd(DMA1_Channel2, DISABLE); // RX
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
