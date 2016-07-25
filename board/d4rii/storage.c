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

#include "storage.h"
#include "debug.h"
#include "wdt.h"
#include "delay.h"
#include "led.h"
//#include "flash.h"
#include "frsky.h"
#include "hal_defines.h"

//persistant storage in flash
//FIXME __code __at (STORAGE_LOCATION) uint8_t storage_on_flash[STORAGE_PAGE_SIZE]; //no ini value -> sdcc does not init this!

//run time copy of persistant storage data:
EXTERNAL_MEMORY STORAGE_DESC storage;

void storage_init(void){
    debug("storage: init\n"); debug_flush();

    //reload data from flash
    storage_read_from_flash();

    /*frsky_enter_rxmode(0);
    IEN2 &= ~(IEN2_RFIE);
    RFIM = 0;
    storage.version = 0xAB;
    storage.frsky_txid[0] = 0x12;
    storage.frsky_txid[1] = 0x34;
    storage_write_to_flash();*/

}

void storage_read_from_flash(void){
    /*uint16_t i;
    uint8_t *storage_ptr = (uint8_t*)storage;

    debug("storage: loading from flash: "); debug_flush();

    //copy from persistant flash to ram:
    for(i=0; i<sizeof(storage); i++){
        storage_ptr[i] = storage_on_flash[i];
        debug_put_hex8(storage_on_flash[i]); debug_putc(' '); debug_flush();
        wdt_reset();
    }*/
    storage.version = 0xAB;
    storage.frsky_txid[0] = 0x12;
    storage.frsky_txid[1] = 0x34;
}

void storage_write_to_flash(void){
    /*
    debug("storage: writing to flash\n"); debug_flush();
    storage.version = STORAGE_VERSION_ID;

    //execute flash write:
    flash_write((uint16_t)storage_on_flash, (uint8_t*)&storage, sizeof(storage));
    */
}

