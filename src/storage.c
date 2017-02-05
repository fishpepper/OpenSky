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

#include "storage.h"
#include "hal_storage.h"
#include "debug.h"
#include "wdt.h"
#include "delay.h"
#include "led.h"
#include "frsky.h"
#include "hal_defines.h"


// run time copy of persistant storage data:
EXTERNAL_MEMORY STORAGE_DESC storage;

void storage_init(void) {
    uint8_t i;

    debug("storage: init\n"); debug_flush();

    // init hal storage
    hal_storage_init();

    // reload data from flash
    storage_read_from_flash();

    debug("storage: loaded hoptable[] = ");
    for (i = 0; i < FRSKY_HOPTABLE_SIZE; i++) {
        debug_put_hex8(storage.frsky_hop_table[i]);
        debug_putc(' ');
        debug_flush();
    }
    debug_put_newline();
}

static const uint8_t storage_default_hoptable[] =
      { 0x01, 0x42, 0x83, 0xC4, 0x1A, 0x5B, 0x9C, 0xDD, 0x33, 0x74, 0xB5, 0x0B,
        0x4C, 0x8D, 0xCE, 0x24, 0x65, 0xA6, 0xE7, 0x3D, 0x7E, 0xBF, 0x15, 0x56,
        0x97, 0xD8, 0x2E, 0x6F, 0xB0, 0x06, 0x47, 0x88, 0xC9, 0x1F, 0x60, 0xA1,
        0xE2, 0x38, 0x79, 0xBA, 0x10, 0x51, 0x92, 0xD3, 0x29, 0x6A, 0xAB };

void storage_read_from_flash(void) {
    uint8_t *storage_ptr;
    uint16_t len;
    uint8_t i;

    debug("storage: reading\n"); debug_flush();
    storage_ptr = (uint8_t*)&storage;
    len = sizeof(storage);

    hal_storage_read(storage_ptr, len);

#ifdef FRSKY_USE_FIXED_ID
    // allow override for testing
    if (1) {
#else  // FRSKY_USE_FIXED_ID
    // only init with defaults when no valid storage id was found
    if (storage.version != STORAGE_VERSION_ID) {
#endif  // FRSKY_USE_FIXED_ID
        debug("storage: init with defaults (txid 0x1668)\n");

        storage.version = STORAGE_VERSION_ID;

        // hard coded config for debugging:
        storage.frsky_txid[0] = 0x16;
        storage.frsky_txid[1] = 0x68;
        storage.frsky_freq_offset = DEFAULT_FSCAL_VALUE;

        for (i = 0; i < FRSKY_HOPTABLE_SIZE; i++) {
            storage.frsky_hop_table[i] = storage_default_hoptable[i];
        }

        // store settings
        storage_write_to_flash();
    }
}

void storage_write_to_flash(void) {
    uint8_t *storage_ptr;
    uint16_t len;

    debug("storage: writing\n"); debug_flush();
    storage.version = STORAGE_VERSION_ID;

    storage_ptr = (uint8_t*)&storage;
    len = sizeof(storage);

    // execute flash write:
    hal_storage_write(storage_ptr, len);
}

