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

//fast software spi routines
//based on maxim APPLICATION NOTE 3524
//ported to sdcc/cc2510
#include "soft_spi.h"



void soft_spi_init(void){
    //set to outputs:
    SOFT_SPI_CLOCK_DIR |= (1<<SOFT_SPI_CLOCK_PIN);

    SOFT_SPI_MOSI_DIR  |= (1<<SOFT_SPI_MOSI_PIN);

    //init to idle values:
    SOFT_SPI_CLOCK_BIT = SOFT_SPI_SCK_INIT;
}

#if 1
__data uint8_t soft_spi_tx_byte;

void soft_spi_tx_do(void){
      SOFT_SPI_SCK_PRE; SOFT_SPI_DATA_TO_MOSI(); SOFT_SPI_SCK_MID; /*soft_spi_tx_bit7=SOFT_SPI_MISO_BIT;*/ SOFT_SPI_SCK_POST; // bit 7
      soft_spi_tx_byte <<= 1;

      SOFT_SPI_SCK_PRE; SOFT_SPI_DATA_TO_MOSI(); SOFT_SPI_SCK_MID; /*soft_spi_tx_bit7=SOFT_SPI_MISO_BIT;*/ SOFT_SPI_SCK_POST; // bit 6
      soft_spi_tx_byte <<= 1;

      SOFT_SPI_SCK_PRE; SOFT_SPI_DATA_TO_MOSI(); SOFT_SPI_SCK_MID; /*soft_spi_tx_bit7=SOFT_SPI_MISO_BIT;*/ SOFT_SPI_SCK_POST; // bit 5
      soft_spi_tx_byte <<= 1;

      SOFT_SPI_SCK_PRE; SOFT_SPI_DATA_TO_MOSI(); SOFT_SPI_SCK_MID; /*soft_spi_tx_bit7=SOFT_SPI_MISO_BIT;*/ SOFT_SPI_SCK_POST; // bit 4
      soft_spi_tx_byte <<= 1;

      SOFT_SPI_SCK_PRE; SOFT_SPI_DATA_TO_MOSI(); SOFT_SPI_SCK_MID; /*soft_spi_tx_bit7=SOFT_SPI_MISO_BIT;*/ SOFT_SPI_SCK_POST; // bit 3
      soft_spi_tx_byte <<= 1;

      SOFT_SPI_SCK_PRE; SOFT_SPI_DATA_TO_MOSI(); SOFT_SPI_SCK_MID; /*soft_spi_tx_bit7=SOFT_SPI_MISO_BIT;*/ SOFT_SPI_SCK_POST; // bit 2
      soft_spi_tx_byte <<= 1;

      SOFT_SPI_SCK_PRE; SOFT_SPI_DATA_TO_MOSI(); SOFT_SPI_SCK_MID; /*soft_spi_tx_bit7=SOFT_SPI_MISO_BIT;*/ SOFT_SPI_SCK_POST; // bit 1
      soft_spi_tx_byte <<= 1;

      SOFT_SPI_SCK_PRE; SOFT_SPI_DATA_TO_MOSI(); SOFT_SPI_SCK_MID; /*soft_spi_tx_bit7=SOFT_SPI_MISO_BIT;*/ SOFT_SPI_SCK_POST; // bit 0
}
#else
//SDCC hack for fast bit variable access:
//actually this is quite fast but it sometimes collides with the stack
//i could not figure out how to do this properly in SDCC
//bit adressable memory is 0x20-0x2F on 8051 cores
__data uint8_t __at 0x0020 soft_spi_tx_byte;
__bit __at 0x00 soft_spi_tx_bit0;
__bit __at 0x01 soft_spi_tx_bit1;
__bit __at 0x02 soft_spi_tx_bit2;
__bit __at 0x03 soft_spi_tx_bit3;
__bit __at 0x04 soft_spi_tx_bit4;
__bit __at 0x05 soft_spi_tx_bit5;
__bit __at 0x06 soft_spi_tx_bit6;
__bit __at 0x07 soft_spi_tx_bit7;

/*__data uint8_t __at 0x002F soft_spi_tx_byte;
__bit __at 0x78 soft_spi_tx_bit0;
__bit __at 0x79 soft_spi_tx_bit1;
__bit __at 0x7A soft_spi_tx_bit2;
__bit __at 0x7B soft_spi_tx_bit3;
__bit __at 0x7C soft_spi_tx_bit4;
__bit __at 0x7D soft_spi_tx_bit5;
__bit __at 0x7E soft_spi_tx_bit6;
__bit __at 0x7F soft_spi_tx_bit7;*/


void soft_spi_tx_do(void){
      SOFT_SPI_SCK_PRE; SOFT_SPI_MOSI_BIT=soft_spi_tx_bit7; SOFT_SPI_SCK_MID; /*soft_spi_tx_bit7=SOFT_SPI_MISO_BIT;*/ SOFT_SPI_SCK_POST; // bit 7
      SOFT_SPI_SCK_PRE; SOFT_SPI_MOSI_BIT=soft_spi_tx_bit6; SOFT_SPI_SCK_MID; /*soft_spi_tx_bit6=SOFT_SPI_MISO_BIT;*/ SOFT_SPI_SCK_POST; // bit 6
      SOFT_SPI_SCK_PRE; SOFT_SPI_MOSI_BIT=soft_spi_tx_bit5; SOFT_SPI_SCK_MID; /*soft_spi_tx_bit5=SOFT_SPI_MISO_BIT;*/ SOFT_SPI_SCK_POST; // bit 5
      SOFT_SPI_SCK_PRE; SOFT_SPI_MOSI_BIT=soft_spi_tx_bit4; SOFT_SPI_SCK_MID; /*soft_spi_tx_bit4=SOFT_SPI_MISO_BIT;*/ SOFT_SPI_SCK_POST; // bit 4
      SOFT_SPI_SCK_PRE; SOFT_SPI_MOSI_BIT=soft_spi_tx_bit3; SOFT_SPI_SCK_MID; /*soft_spi_tx_bit3=SOFT_SPI_MISO_BIT;*/ SOFT_SPI_SCK_POST; // bit 3
      SOFT_SPI_SCK_PRE; SOFT_SPI_MOSI_BIT=soft_spi_tx_bit2; SOFT_SPI_SCK_MID; /*soft_spi_tx_bit2=SOFT_SPI_MISO_BIT;*/ SOFT_SPI_SCK_POST; // bit 2
      SOFT_SPI_SCK_PRE; SOFT_SPI_MOSI_BIT=soft_spi_tx_bit1; SOFT_SPI_SCK_MID; /*soft_spi_tx_bit1=SOFT_SPI_MISO_BIT;*/ SOFT_SPI_SCK_POST; // bit 1
      SOFT_SPI_SCK_PRE; SOFT_SPI_MOSI_BIT=soft_spi_tx_bit0; SOFT_SPI_SCK_MID; /*soft_spi_tx_bit0=SOFT_SPI_MISO_BIT;*/ SOFT_SPI_SCK_POST; // bit 0
}
#endif
