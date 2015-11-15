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
#include "flash.h"
#include "frsky.h"

//persistant storage in flash
__code __at (STORAGE_LOCATION) uint8_t storage_on_flash[STORAGE_PAGE_SIZE] = {
    //this is a trick to fill the flash data with 0xFF on building (saves some time on flashing)
    //sdcc does not support noinit pragme (?)
    #define STATIC_INIT_VALUE 0xFF
    #define STATIC_INIT_COUNT STORAGE_PAGE_SIZE
    #include "static_init.h"
};

//place in mem from where to run the function (MUST BE BIG ENOUGH!)
//static uint16_t __xdata storage_run_loc[64];

//run time copy of persistant storage data:
__xdata STORAGE_DESC storage;

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
    uint16_t i;
    uint8_t *storage_ptr = (uint8_t*)storage;

    debug("storage: loading from flash: "); debug_flush();

    //copy from persistant flash to ram:
    for(i=0; i<sizeof(storage); i++){
        storage_ptr[i] = storage_on_flash[i];
        debug_put_hex8(storage_on_flash[i]); debug_putc(' '); debug_flush();
        wdt_reset();
    }
}

void storage_write_to_flash(void){
    debug("storage: writing to flash\n"); debug_flush();
    storage.version = STORAGE_VERSION_ID;

    //execute flash write:
    flash_write((uint16_t)storage_on_flash, (uint8_t*)&storage, sizeof(storage));
}

