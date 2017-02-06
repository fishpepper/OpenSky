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
#include "hal_cc25xx.h"
#include "hal_dma.h"

// place data on end of flash
// FIXME: this is for a cc2510f16 with flash size 0x4000, needs to be adjusted for bigger mcus
#define STORAGE_PAGE_SIZE 1024
#define STORAGE_LOCATION (0x4000-STORAGE_PAGE_SIZE)

// place persistant storage:
extern __code __at(STORAGE_LOCATION) uint8_t storage_on_flash[STORAGE_PAGE_SIZE];
extern __xdata HAL_DMA_DESC flash_dma_config;


void hal_storage_init(void);
void hal_storage_write(uint8_t *buffer, uint16_t len);
void hal_storage_read(uint8_t *storage_ptr, uint16_t len);

static void hal_storage_flash_write(uint16_t address, uint8_t *data, uint16_t len);
void hal_storage_flash_enable_write(void);
void hal_storage_flash_erase_page(void);

#endif  // HAL_STORAGE_H_
