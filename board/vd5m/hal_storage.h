#ifndef __HAL_STORAGE_H__
#define __HAL_STORAGE_H__
#include <stdint.h>
#include "hal_cc25xx.h"
#include "hal_dma.h"

//place data on end of flash
//FIXME: this is for a cc2510f16 with flash size 0x4000, needs to be adjusted for bigger mcus
#define STORAGE_PAGE_SIZE 1024
#define STORAGE_LOCATION (0x4000-STORAGE_PAGE_SIZE)

//place persistant storage:
extern __code __at (STORAGE_LOCATION) uint8_t storage_on_flash[STORAGE_PAGE_SIZE];
extern __xdata HAL_DMA_DESC flash_dma_config;


void hal_storage_init(void);
void hal_storage_write(uint8_t *buffer, uint16_t len);
void hal_storage_read(uint8_t *storage_ptr, uint16_t len);

static void hal_storage_flash_write(uint16_t address, uint8_t *data, uint16_t len);
void hal_storage_flash_enable_write(void);
void hal_storage_flash_erase_page(void);

#define BEGIN_CODE_ABS_LOCATION(uniqueName, address)    \
void __##uniqueName##_Begin(void) __naked                 \
{                                                       \
        __asm                                            \
                .area ABSCODE (ABS,CODE)                \
        __endasm;                                        \
        __asm                                            \
                .org address                            \
        __endasm;                                        \
}

// return to normal mode by using this macro.
#define END_CODE_ABS_LOCATION(uniqueName)               \
void __##uniqueName##_End(void) __naked                   \
{                                                       \
        __asm                                            \
                .area CSEG (REL,CODE)                   \
        __endasm;                                        \
}


#endif // __HAL_STORAGE_H__
