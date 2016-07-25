#ifndef __HAL_SPI__H_
#define __HAL_SPI__H_
#include <stdint.h>
#include "pin_config.h"
#include "delay.h"

void hal_spi_init(void);
void _hal_spi_gpio_init(void);
void _hal_spi_mode_init(void);
void _hal_spi_enable(void);
void _hal_spi_rcc_init(void);

#define hal_spi_csn_lo() { CC25XX_SPI_GPIO->BRR  = (CC25XX_SPI_CSN_PIN); delay_us(1); }
#define hal_spi_csn_hi() { delay_us(1); CC25XX_SPI_GPIO->BSRR = (CC25XX_SPI_CSN_PIN); }
uint8_t hal_spi_tx(uint8_t address);
uint8_t hal_spi_rx(void);

#endif // __HAL_SPI__H_

