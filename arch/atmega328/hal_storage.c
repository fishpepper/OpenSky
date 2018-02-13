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

   author: fishpepper <AT> gmail.com, jimmyw <AT> github
*/

#include "hal_storage.h"
#include "storage.h"
#include <avr/eeprom.h>

uint8_t storage_memory[sizeof(STORAGE_DESC)] EEMEM;

void hal_storage_write(uint8_t *buffer, uint16_t len) {
    eeprom_write_block(buffer, storage_memory, len);
}

void hal_storage_read(uint8_t *storage_ptr, uint16_t len) {
    eeprom_read_block(storage_ptr, storage_memory, len);
}

