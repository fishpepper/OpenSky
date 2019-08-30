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

#include "hal_cc25xx.h"
#include "hal_spi.h"
#include "hal_io.h"
#include "cc25xx.h"
#include "debug.h"
#include "timeout.h"
#include <util/delay.h>

void hal_cc25xx_init(void){
    hal_spi_init();
}

inline uint32_t hal_cc25xx_set_antenna(uint8_t id){
    return id;
}

inline void hal_cc25xx_set_gdo_mode(void) {
    cc25xx_set_register(IOCFG0, 0x01); //6);
    //cc25xx_set_register(IOCFG1, ???);
    cc25xx_set_register(IOCFG2, 0x01); //6);
}

inline void hal_cc25xx_set_register(uint8_t address, uint8_t data){
    //select device
    hal_spi_csn_lo();

    //wait for ready signal
    hal_io_wait_miso_low();

    hal_spi_tx(address);
    hal_spi_tx(data);
    //debug("hal_cc25xx_set_register address: 0x"); debug_put_hex8(address); debug(" data: 0x"); debug_put_hex8(data); debug("\n"); debug_flush();

    //deslect
    hal_spi_csn_hi();
    // TO VERIFY
    // if (hal_cc25xx_get_register(address) != data) while(1) do debug("FAIL");
}

inline uint8_t hal_cc25xx_get_register(uint8_t address){
    uint8_t result;

    //select device:
    hal_spi_csn_lo();

    //wait for RDY signal:
    hal_io_wait_miso_low();

    //request address (read request has bit7 set)
    uint8_t status = hal_spi_tx(address | 0x80);

    //fetch result:
    result = hal_spi_rx();
    //debug("hal_cc25xx_get_register: 0x"); debug_put_hex8(address & ~(READ_FLAG | BURST_FLAG));
    //if (address & READ_FLAG) debug(" READ");
    //if (address & BURST_FLAG) debug(" BURST");
    //debug(" got: 0x"); debug_put_hex8(result); debug("\n");

    //deselect device
    hal_spi_csn_hi();

    //return result
    return(result);
}

inline void hal_cc25xx_strobe(uint8_t address){
    hal_spi_csn_lo();
    uint8_t status = hal_spi_tx(address);
    //debug("strobe: 0x"); debug_put_hex8(address); debug(" status: 0x"); debug_put_hex8(status); debug_put_newline();
    hal_spi_csn_hi();
}

inline uint8_t hal_cc25xx_get_status(void) {
    hal_spi_csn_lo();
    uint8_t status = hal_spi_tx(0xFF);
    hal_spi_csn_hi();
    return status;
}

uint8_t hal_cc25xx_transmission_completed(void) {
    //after tx cc25xx goes back to RX (configured by mcsm1 register)
    return ((hal_cc25xx_get_status() & (0x70)) == CC2500_STATUS_STATE_RX);
}

inline void hal_cc25xx_enter_rxmode(void) {
    //add pa/lna config bit setting here
    //debug("RX\n");
    hal_io_enable_pa(0);
    _delay_us(20);
    hal_io_enable_lna(1);
    _delay_us(5);

}

inline void hal_cc25xx_enter_txmode(void) {
    //add pa/lna config bit setting here
    //debug("TX\n");
    hal_io_enable_lna(0);
    _delay_us(20);
    hal_io_enable_pa(1);
    _delay_us(5);
}


inline void hal_cc25xx_enable_receive(void){
    //switch on rx again
    hal_cc25xx_enter_rxmode();
}


inline void hal_cc25xx_read_fifo(uint8_t *buf, uint8_t len){
    hal_cc25xx_register_read_multi(CC25XX_FIFO | READ_FLAG | BURST_FLAG, buf, len);
}

inline void hal_cc25xx_register_read_multi(uint8_t address, uint8_t *buffer, uint8_t len){
    // select device:
    hal_spi_csn_lo();

    // wait for ready signal
    //debug("read_multi address: 0x");
    //debug_put_hex8(address);
    //debug(" len: ");
    //debug_put_uint8(len);
    //debug("\n");
    hal_io_wait_miso_low();

    // request address (read request)
    uint8_t status = hal_spi_tx(address);

    //fill buffer with read commands:
    memset(buffer, 0xFF, len);
    uint8_t len2 = len;
    uint8_t *buffer2 = buffer;

    while(len--){
        *buffer = hal_spi_rx();
        buffer++;
    }

#if DEBUG_PRINT_READ
    if (len2 > 0) {
      debug("read data len: ");
      debug_put_uint8(len2);
      debug(" status:");
      debug_put_hex8(status);
      debug(" :");
      while(len2--) {
        debug(" 0x");
        debug_put_hex8(*buffer2);
        buffer2++;
        if (len2 % 10 == 0)
          debug_flush();
      }
      debug("\n");
    }
#endif

    // deselect device
    hal_spi_csn_hi();
}

inline void hal_cc25xx_register_write_multi(uint8_t address, uint8_t *buffer, uint8_t len){
    //s elect device:
    hal_spi_csn_lo();

    //debug("write "); debug_put_uint8(len); debug_flush();
    // wait for RDY signal:
    hal_io_wait_miso_low();

    //request address (write request)
    hal_spi_tx(address | BURST_FLAG);

    // send array
    while(len--){
        hal_spi_tx(*buffer);
        buffer++;
    }

    //deselect device
    hal_spi_csn_hi();
}

inline void hal_cc25xx_process_packet(volatile uint8_t *packet_received, volatile uint8_t *buffer, uint8_t maxlen){
    if(hal_io_get_gdo() == 1){
        // debug("GDO\n");
        //data received, fetch data
        //timeout_set_100us(5);

        *packet_received = 0;
        //hal_io_debug(1);

        //there is a bug in the cc2500
        //see p3 http://www.ti.com/lit/er/swrz002e/swrz002e.pdf
        //workaround: read len register very quickly twice:
        uint8_t len1, len2, len, i;

        //try this 10 times befor giving up:
        for (i=0; i<10; i++){
            len1 = hal_cc25xx_get_register_burst(RXBYTES) & 0x7F;
            len2 = hal_cc25xx_get_register_burst(RXBYTES) & 0x7F;
            if (len1==len2) break;
        }
        /*
        debug("Got len1: ");
        debug_put_hex8(len1);
        debug(" len2: ");
        debug_put_hex8(len2);
        debug(" maxlen: ");
        debug_put_hex8(maxlen);
        debug("\n");
        */

        //valid len found?
        if (len1==len2){
            len = len1;

            //packet received, grab data
            uint8_t tmp_buffer[len];
            hal_cc25xx_read_fifo(tmp_buffer, len);

            //only accept valid packet lenbghts:
            if (len == maxlen){
                uint8_t i;
                for(i=0; i<maxlen; i++){
                    buffer[i] = tmp_buffer[i];
                }
                *packet_received = 1;
                //debug("PACKET_RECEIVED\n");
            } else {
                //debug("INCOMPLETE_PACKET\n");
            }
        }else{
            //no, ignore this
            len = 0;
            //debug("IGNORED\n");
        }
        //hal_io_debug(0);
    } else {
        //debug("NO GDO\n");
    }
}

void hal_cc25xx_transmit_packet(volatile uint8_t *buffer, uint8_t len) {
    //flush tx fifo
    hal_cc25xx_strobe(RFST_SFTX);
    //copy to fifo
    hal_cc25xx_register_write_multi(CC25XX_FIFO, (uint8_t *)buffer, buffer[0]+1);
    //and send!
    hal_cc25xx_strobe(RFST_STX);
}
