#ifndef __DEBUG__H_
#define __DEBUG__H_
#include "main.h"
#include "hal_defines.h"
#include "hal_debug.h"
#include <stdint.h>

#define DEBUG_TX_BUFFER_SIZE 128

#if !DEBUG
    #define debug_init() {}
    #define debug_is_initialized() (0)
    #define debug(__ax)           {}
    #define debug_put_hex8(__a)   {}
    #define debug_put_hex16(__a)  {}
    #define debug_put_hex32(__a)  {}
    #define debug_put_uint8(__a)  {}
    #define debug_put_uint16(__a) {}
    #define debug_put_int8(__a)   {}
    #define debug_put_newline()   {}
    #define debug_putc(__c)       {}
    #define debug_flush()         {}

#else

typedef struct {
    uint8_t data[DEBUG_TX_BUFFER_SIZE];
    uint8_t read;
    uint8_t write;
} debug_buffer_t;


extern EXTERNAL_MEMORY uint8_t debug_init_done;
extern EXTERNAL_MEMORY debug_buffer_t debug_buffer;

#define debug_is_initialized() (debug_init_done)

void debug_init(void);
void debug_uart_init(void);
void debug_test(void);
void debug_putc(uint8_t ch);
void debug_flush(void);
void debug(uint8_t *data);
void debug_put_hex8(uint8_t val);
void debug_put_hex16(uint16_t val);
void debug_put_hex32(uint32_t val);
void debug_put_uint8(uint8_t c);
void debug_put_int8(int8_t c);
void debug_put_uint16(uint16_t c);
void debug_put_newline(void);

void DEBUG_ISR(void);

#if ((DEBUG_TX_BUFFER_SIZE==128) || (DEBUG_TX_BUFFER_SIZE==64) || (DEBUG_TX_BUFFER_SIZE==32))
    //ALWAYS use 2^n for buffer size! -> faster code and no int16 in interrupts (see Readme.md)
    #define DEBUG_TX_BUFFER_AND_OPERAND (DEBUG_TX_BUFFER_SIZE-1)
#else
    //FAIL!
    #warn "ALWAYS USE POWER OF TWO FOR BUFFER SIZE!"
#endif

#endif



#endif
