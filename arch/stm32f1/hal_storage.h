/*
    Copyright 2017 fishpepper <AT> gmail.com

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http:// www.gnu.org/licenses/>.

   author: fishpepper <AT> gmail.com
*/

#ifndef HAL_STORAGE_H_
#define HAL_STORAGE_H_

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


#endif  // HAL_STORAGE_H_
