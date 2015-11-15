#ifndef __SOFT_SPI__
#define __SOFT_SPI__
#include <cc2510fx.h>
#include <stdint.h>
#include "config.h"
#include "main.h"

//SPI MODE:
#define SOFT_SPI_CPHA 1
#define SOFT_SPI_CPOL 1

//extern __data uint8_t __at 0x0020 soft_spi_tx_byte;
//#define SOFT_SPI_CLOCK_HI()  { SOFT_SPI_CLOCK_BIT = 1; NOP(); NOP(); NOP(); NOP(); NOP();}

extern __data uint8_t soft_spi_tx_byte;

void soft_spi_init(void);
void soft_spi_tx_do(void);
#define soft_spi_tx(data) {soft_spi_tx_byte=data;  soft_spi_tx_do(); }

#define SOFT_SPI_CLOCK_HI()  { SOFT_SPI_CLOCK_BIT = 1; }
#define SOFT_SPI_CLOCK_LO()  { SOFT_SPI_CLOCK_BIT = 0; }
#define SOFT_SPI_DATA_TO_MOSI() {if (soft_spi_tx_byte & 0x80){ SOFT_SPI_MOSI_BIT=1; }else{ SOFT_SPI_MOSI_BIT = 0; };}

#if SOFT_SPI_CPHA
   #define SOFT_SPI_SCK_POST
   #if SOFT_SPI_CPOL
      #define SOFT_SPI_SCK_INIT 1
      #define SOFT_SPI_SCK_PRE  SOFT_SPI_CLOCK_LO()
      #define SOFT_SPI_SCK_MID  SOFT_SPI_CLOCK_HI()
   #else
      #define SOFT_SPI_SCK_INIT 0
      #define SOFT_SPI_SCK_PRE  SOFT_SPI_CLOCK_HI()
      #define SOFT_SPI_SCK_MID  SOFT_SPI_CLOCK_LO()
   #endif
#else
   #define SOFT_SPI_SCK_PRE
   #if SOFT_SPI_CPOL
      #define SOFT_SPI_SCK_INIT 1
      #define SOFT_SPI_SCK_MID  SOFT_SPI_CLOCK_LO()
      #define SOFT_SPI_SCK_POST SOFT_SPI_CLOCK_HI()
   #else
      #define SOFT_SPI_SCK_INIT 0
      #define SOFT_SPI_SCK_MID  SOFT_SPI_CLOCK_HI()
      #define SOFT_SPI_SCK_POST SOFT_SPI_CLOCK_LO()
   #endif
#endif



#endif
