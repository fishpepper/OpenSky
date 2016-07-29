#ifndef __HAL_SPI__H_
#define __HAL_SPI__H_
#include <stdint.h>
#include "pin_config.h"
#include "delay.h"

void hal_spi_init(void);
static void hal_spi_gpio_init(void);
static void hal_spi_mode_init(void);
static void hal_spi_enable(void);
static void hal_spi_rcc_init(void);
void hal_spi_tx_buffer_dma(uint8_t *buffer, uint8_t len);
void hal_spi_rx_buffer_dma(uint8_t *buffer, uint8_t len);
#define hal_spi_csn_lo() { CC25XX_SPI_GPIO->BRR  = (CC25XX_SPI_CSN_PIN); delay_us(1); }
#define hal_spi_csn_hi() { delay_us(1); CC25XX_SPI_GPIO->BSRR = (CC25XX_SPI_CSN_PIN); }
uint8_t hal_spi_tx(uint8_t address);
uint8_t hal_spi_rx(void);
uint8_t hal_spi_read_address(uint8_t address);

#endif // __HAL_SPI__H_

