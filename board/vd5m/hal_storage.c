//persistant storage in flash
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

#include "flash.h"
#include <cc2510fx.h>
#include "debug.h"
#include "main.h"
#include "delay.h"
#include "wdt.h"


__xdata DMA_DESC flash_dma_config;
__code __at (STORAGE_LOCATION) uint8_t storage_on_flash[STORAGE_PAGE_SIZE]; //no ini value -> sdcc does not init this!

void hal_storage_init(void) {
    debug("hal_storage: init\n"); debug_flush();
}

void hal_storage_write(uint8_t *buffer, uint16_t len){
    hal_storage_flash_write((uint16_t)storage_on_flash, (uint8_t*)&storage, sizeof(storage));
}

void hal_storage_read(uint8_t *storage_ptr, uint16_t len){
    uint16_t i;

    debug("hal_storage: loading from flash: "); debug_flush();

    //copy from persistant flash to ram:
    for(i=0; i<len; i++){
        storage_ptr[i] = storage_on_flash[i];
        debug_put_hex8(storage_on_flash[i]); debug_putc(' '); debug_flush();
        wdt_reset();
    }
}

static void hal_storage_flash_write(uint16_t address, uint8_t *data, uint16_t len){
    uint16_t i=0;
    uint8_t *flash_ptr=0;

    debug("hal_storage: will write page at 0x"); debug_flush();
    debug_put_hex8(address>>8);
    debug_put_hex8(address&0xff);
    debug_put_newline();

    //this is VERY important:
    //make sure to write an even number of bytes!
    //simply write one extra byte
    if(len & 0x0001){
        debug("flash: corrected len to even\n");
        len++;
    }

    //disable interrupts
    cli();

    //cancel _ALL_ ongoing DMA transfers:
    DMAARM = DMA_ARM_ABORT | 0x1F;

    //high prio
    flash_dma_config.PRIORITY       = DMA_PRI_HIGH;
    //irrelevant since we use LEN for transfer count
    flash_dma_config.M8             = DMA_M8_USE_8_BITS;
    //disable ints from this ch
    flash_dma_config.IRQMASK        = DMA_IRQMASK_DISABLE;
    //use dma flash data write complete trigger
    flash_dma_config.TRIG           = DMA_TRIG_FLASH;
    //single mode, see datasheet
    flash_dma_config.TMODE          = DMA_TMODE_SINGLE;
    //one byte
    flash_dma_config.WORDSIZE       = DMA_WORDSIZE_BYTE;

    //set src: address of data to be written
    SET_WORD(flash_dma_config.SRCADDRH,  flash_dma_config.SRCADDRL,  data);
    //destination is flash controller data reg
    SET_WORD(flash_dma_config.DESTADDRH, flash_dma_config.DESTADDRL, &X_FWDATA);
    //use LEN
    flash_dma_config.VLEN           = DMA_VLEN_USE_LEN;
    //set length
    SET_WORD(flash_dma_config.LENH, flash_dma_config.LENL, len);
    //set srcinc to 1 byte
    flash_dma_config.SRCINC         = DMA_SRCINC_1;
    //fixed, always write to FWDATA
    flash_dma_config.DESTINC        = DMA_DESTINC_0;

    // Save pointer to the DMA configuration struct into DMA-channel 0
    // configuration registers
    SET_WORD(DMA0CFGH, DMA0CFGL, flash_dma_config);

    //waiting for the flash controller to be ready
    while (FCTL & FCTL_BUSY);

    //configure flash controller for 26mhz clock
    FWT = 0x2A; //(21 * 26) / (16);

    //set up address:
    SET_WORD(FADDRH, FADDRL, ((uint16_t)address)>>1);

    //re enable ints
    sei();

    debug("hal_storage: erasing page\n");debug_flush();

    //disable interrupts
    cli();

    //clear any pending flags
    DMAIRQ = 0;

    //erase that page
    //has to be 2byte aligned. use a hack to place it at a given adress:
    hal_storage_flash_erase_page();

    // Wait for the erase operation to complete. //
    while (FCTL & FCTL_BUSY);

    //FCTL = 0;

    //re enable ints
    sei();

    debug("hal_storage: erase done\n"); debug_flush();

    debug("hal_storage: will write ["); debug_flush();
    i=0;
    while (i < len){
        debug_put_hex8(((uint8_t *)data)[i++]);
        debug_put_hex8(((uint8_t *)data)[i++]);
        debug_putc(' ');
        debug_flush();
    }
    debug("]\n");

    debug("hal_storage: will write flash now\n"); debug_flush();

    //disable interrupts
    cli();

    //arm the DMA channel, so that a DMA trigger will initiate DMA writing
    DMAARM = DMA_ARM_CH0;
    NOP();

    //enable flash write. this generates a DMA trigger.
    //must be aligned on a 2-byte boundary and is therefor implemented in assembly!
    hal_storage_flash_enable_write();


    //wait for dma finish
    while (!(DMAIRQ & DMAIRQ_DMAIF0)){
       wdt_reset();
    }

    //wait until flash controller not busy
    while (FCTL & (FCTL_BUSY | FCTL_SWBUSY));

    sei();

    //by now, the transfer is completed, so the transfer count is reached.
    //the DMA channel 0 interrupt flag is then set, so we clear it here.
    DMAIRQ &= ~DMAIRQ_DMAIF0;


    debug("hal_storage: read back [");
    //copy from persistant flash to ram:
    flash_ptr = address;
    for(i=0; i<len; i++){
        debug_put_hex8(*flash_ptr++); debug_putc(' '); debug_flush();
        wdt_reset();
    }
    debug("]\n");

    debug("hal_storage: write done");
}


//this has to be placed at a 2byte boundary, as sdcc does not support .align
//pragmas this is done in this hacky way...
BEGIN_CODE_ABS_LOCATION(FLASH_ENABLE_WRITE, 0x1000)  // NOTE:  No semicolon!
void hal_storage_flash_enable_write(void){
    __asm
    ORL _FCTL, #0x02; //FCTL |=  FCTL_WRITE
    __endasm;
}
END_CODE_ABS_LOCATION(FLASH_ENABLE_WRITE)            // NOTE:  No semicolon!

//this has to be placed at a 2byte boundary, as sdcc does not support .align
//pragmas this is done in this hacky way...
BEGIN_CODE_ABS_LOCATION(FLASH_ERASE_PAGE, 0x1010)  // NOTE:  No semicolon!
void hal_storage_flash_erase_page(void){
    __asm
    ORL _FCTL, #0x01; //FCTL |= FCTL_ERASE
    NOP;            //required sequence!
    __endasm;
}
END_CODE_ABS_LOCATION(FLASH_ERASE_PAGE) // NOTE:  No semicolon!
