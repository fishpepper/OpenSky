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

   author: fishpepper <AT> gmail.com, jimmyw <AT> github
*/

#include "hal_spi.h"
#include <avr/io.h>
#include <avr/interrupt.h>

/*  SPCR
 *  SPI2X    SPR01   SPR00   DESCRIPTION
 *  0        0       0       Clock / 4
 *  0        0       1       Clock / 16
 *  0        1       0       Clock / 64
 *  0        1       1       Clock / 128
 *  1        0       0       Clock / 2
 *  1        0       0       Clock / 8
 *  1        0       1       Clock / 32
 *  1        1       1       Clock / 64
 *
 *  SPE:  Enable
 *  MSTR: Master
 */

/* Note, by some reason telemetry fails if divider is higher the 4 */

void hal_spi_init(void) {

  /* Enable SPI, Master, set clock rate fck/4 */
  SPCR = (1<<SPE)|(1<<MSTR)|(0<<SPI2X)|(0<<SPR1)|(0<<SPR0);

  /* PUT SS HI */
  hal_spi_csn_hi();
}

uint8_t inline hal_spi_tx(uint8_t address) {

  /* Send byte */
  SPDR = address;

  /* Wait for transmission complete */
  while(!(SPSR & (1<<SPIF))) {}

  /* Return byte */
  const uint8_t r = SPDR;
  //debug("hal_spi_tx sent: 0x"); debug_put_hex8(address); debug(" got: 0x"); debug_put_hex8(r); debug("\n"); debug_flush();
  return r;
}

uint8_t hal_spi_rx(void) {
  return hal_spi_tx(0xff);
}

void hal_spi_csn_lo() {
  DD_SS_PORT &= ~ (1 << DD_SS_PIN);
}

void hal_spi_csn_hi() {
  DD_SS_PORT |= 1 << DD_SS_PIN;
}

