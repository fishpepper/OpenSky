#ifndef __HAL_STORAGE__H_
#define __HAL_STORAGE__H_
#include <stdint.h>
#include "config.h"
#include "stm32f10x_gpio.h"

void hal_storage_init(void);
void hal_storage_write(uint8_t *buffer, uint16_t len);
void hal_storage_read(uint8_t *storage_ptr, uint16_t len);

static void hal_storage_init_i2c(void);
static void hal_storage_init_i2c_mode(void);
static void hal_storage_init_i2c_gpio(void);
static void hal_storage_init_i2c_rcc(void);
static uint32_t hal_storage_check_len(uint16_t len);
static uint32_t hal_storage_i2c_read_buffer(uint16_t address, uint8_t *buffer, uint8_t len);
static uint32_t hal_storage_i2c_write_buffer(uint8_t address, uint8_t *buffer, uint8_t len);
static uint32_t hal_storage_i2c_write_byte(uint8_t address, uint8_t data);


#define hal_storage_wp_enable()  { EEPROM_GPIO->BSRR = EEPROM_WP_PIN; }
#define hal_storage_wp_disable() { EEPROM_GPIO->BRR  = EEPROM_WP_PIN; }


#endif // __HAL_STORAGE__H_
