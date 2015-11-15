#ifndef __FLASH_H__
#define __FLASH_H__
#include <stdint.h>
#include <cc2510fx.h>
#include "dma.h"

extern __xdata DMA_DESC flash_dma_config;
void flash_write(uint16_t address, uint8_t *data, uint16_t len);


void flash_enable_write(void);
void flash_erase_page(void);

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


#endif
