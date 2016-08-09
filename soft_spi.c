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
#include "soft_spi.h"
#include "hal_soft_spi.h"

EXTERNAL_DATA uint8_t soft_spi_tx_byte;

void soft_spi_init(void){
    hal_soft_spi_init();

    //init to idle values:
    #if SOFT_SPI_SCK_INIT
        SOFT_SPI_CLOCK_HI();
    #else
        SOFT_SPI_CLOCK_LO();
    #endif
}

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

