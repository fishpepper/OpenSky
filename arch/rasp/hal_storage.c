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

#include "hal_storage.h"
#include "debug.h"
#include "main.h"
#include "delay.h"
#include "wdt.h"

#include <stdio.h>
#include <string.h>

void hal_storage_init(void) {
    debug("hal_storage: init\n"); debug_flush();
}

void hal_storage_write(uint8_t *buffer, uint16_t len){
    FILE *f = fopen("flash_storage.bin", "w");
    fwrite(buffer, len, 1, f);
    fclose(f);
}

void hal_storage_read(uint8_t *storage_ptr, uint16_t len){
    FILE *f = fopen("flash_storage.bin", "r");
    if (!f) {
        memset(storage_ptr, 0, len);
        return;
    }
    fread(storage_ptr, len, 1, f);
    fclose(f);
}


