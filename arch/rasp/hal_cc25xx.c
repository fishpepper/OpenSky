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
   along with this program.  If not, see <http:// www.gnu.org/licenses/>.

author: fishpepper <AT> gmail.com
*/

#include "hal_cc25xx.h"
#include "hal_spi.h"
#include "cc25xx.h"
#include "debug.h"
#include "timeout.h"
#include <string.h>
#include <stdio.h>

void hal_cc25xx_init(void) {
    hal_spi_init();
    hal_cc25xx_init_gpio();
}

static void hal_cc25xx_init_gpio(void) {
    hal_cc25xx_enter_rxmode();
}

inline uint32_t hal_cc25xx_set_antenna(uint8_t id) {
    return id;
}

const char *registers[] = {
    "IOCFG2", // 0x00
    "IOCFG1",
    "IOCFG0",
    "FIFOTHR",
    "SYNC1",
    "SYNC0",
    "PKTLEN",
    "PKTCTRL1",
    "PKTCTRL0",
    "ADDR",
    "CHANNR",
    "FSCTRL1",
    "FSCTRL0",
    "FREQ2",
    "FREQ1",
    "FREQ0",
    "MDMCFG4", // 0x10
    "MDMCFG3",
    "MDMCFG2",
    "MDMCFG1",
    "MDMCFG0",
    "DEVIATN",
    "MCSM2",
    "MCSM1",
    "MCSM0",
    "FOCCFG",
    "BSCFG",
    "AGCCTRL2",
    "AGCCTRL1",
    "AGCCTRL0",
    "WOREVT1",
    "WOREVT0",
    "WORCTRL", // 0x20
    "FREND1",
    "FREND0",
    "FSCAL3",
    "FSCAL2",
    "FSCAL1",
    "FSCAL0",
    "RCCTRL1",
    "RCCTRL0",
    "FSTEST",
    "PTEST",
    "AGCTEST",
    "TEST2",
    "TEST1",
    "TEST0",
    "RESERVED",
    "PARTNUM", // 0x30
    "VERSION",
    "FREQEST",
    "LQI",
    "RSSI",
    "MARCSTATE",
    "WORTIME1",
    "WORTIME0",
    "PKTSTATUS",
    "VCO_VC_DAC",
    "TXBYTES",
    "RXBYTES",
    "RCCTRL1_STATUS",
    "RCCTRL0_STATUS",
    "PA_TABLE0" // 0x3E
};


inline void hal_cc25xx_set_gdo_mode(void) {
    cc25xx_set_register(IOCFG0, 0x01); // 6);
    // cc25xx_set_register(IOCFG1, ???);
    cc25xx_set_register(IOCFG2, 0x01); // 6);
}

static int count =0;
inline void hal_cc25xx_set_register(uint8_t address, uint8_t data) {
    uint8_t buffer[2] = {address, data};
    hal_spi_dma_xfer(buffer, sizeof(buffer));
    // printf("\nset_register 0x%x (0x%x) '%s' data 0x%x\n", address & 0x3f, address, registers[address & 0x3f], data);
}

inline uint8_t hal_cc25xx_get_register(uint8_t address) {
    uint8_t buffer[2] = {address | 0x80, 0xff};
    hal_spi_dma_xfer(buffer, sizeof(buffer));
    // printf("\nget_register 0x%x (0x%x) '%s' got 0x%x\n", address & 0x3f, address, registers[address & 0x3f], buffer[1]);
    return buffer[1];
}

static const char *stobes[] = {
    "RFST_SRES",
    "RFST_SFSTXON",
    "RFST_SXOFF",
    "RFST_SCAL",
    "RFST_SRX",
    "RFST_STX",
    "RFST_SIDLE",
    "RFST_SWOR",
    "RFST_SPWD",
    "RFST_SFRX",
    "RFST_SFTX",
    "RFST_SWORRST",
    "RFST_SNOP"
};

inline void hal_cc25xx_strobe(uint8_t address) {
    uint8_t buffer[1] = {address};
    hal_spi_dma_xfer(buffer, sizeof(buffer));

    // printf("strobe 0x%x '%s' got 0x%x\n", address & 0x3f, stobes[address - 0x30], buffer[0]);
    // debug("s"); debug_put_hex8(status); debug_put_newline();
}

uint8_t hal_cc25xx_get_status(void) {
    uint8_t buffer[2] = {0xff, 0xff};
    hal_spi_dma_xfer(buffer, sizeof(buffer));
    return buffer[1];
}

uint8_t hal_cc25xx_transmission_completed(void) {
    return 1; // return ((hal_cc25xx_get_status() & (0x70)) == CC2500_STATUS_STATE_RX);
}

inline void hal_cc25xx_enter_rxmode(void) {
    hal_set_amp(0);
}

inline void hal_cc25xx_enter_txmode(void) {
    hal_set_amp(1);
}


inline void hal_cc25xx_enable_receive(void) {
    // switch on rx again
    hal_cc25xx_enter_rxmode();
}


inline void hal_cc25xx_read_fifo(uint8_t *buf, uint8_t len) {
    hal_cc25xx_register_read_multi(CC25XX_FIFO | READ_FLAG | BURST_FLAG, buf, len);
}

inline void hal_cc25xx_register_read_multi(uint8_t address, uint8_t *buffer, uint8_t len) {
    uint8_t buffer2[len + 1];
    buffer2[0] = address;
    memset(buffer2+1, 0xFF, len);
    hal_spi_dma_xfer(buffer2, len+1);
    memcpy(buffer, buffer2+1, len);
}

inline void hal_cc25xx_register_write_multi(uint8_t address, uint8_t *buffer, uint8_t len) {
    uint8_t buffer2[len + 1];
    buffer2[0] = address | BURST_FLAG;
    memcpy(buffer2+1, buffer, len);
    hal_spi_dma_xfer(buffer2, len+1);
}

inline void hal_cc25xx_process_packet(volatile uint8_t *packet_received, volatile uint8_t *buffer, uint8_t maxlen) {
    if (hal_spi_get_gdo0() == 1) {
        // data received, fetch data
        // timeout_set_100us(5);

        *packet_received = 0;

        // there is a bug in the cc2500
        // see p3 http:// www.ti.com/lit/er/swrz002e/swrz002e.pdf
        // workaround: read len register very quickly twice:
        uint8_t len1, len2, len, i;

        // try this 10 times befor giving up:
        for (i=0; i<10; i++) {
            len1 = hal_cc25xx_get_register_burst(RXBYTES) & 0x7F;
            len2 = hal_cc25xx_get_register_burst(RXBYTES) & 0x7F;
            if (len1==len2) break;
        }

        // valid len found?
        if (len1==len2 && len1 > 0) {
            // debug("process_packet got: "); debug_put_uint8(len1);
            // debug(" expecting: "); debug_put_uint8(maxlen);
            len = len1;

            // packet received, grab data
            uint8_t tmp_buffer[len];
            hal_cc25xx_read_fifo(tmp_buffer, len);

            // only accept valid packet lenbghts:
            if (len == maxlen) {
                uint8_t i;
                for(i=0; i<maxlen; i++) {
                    buffer[i] = tmp_buffer[i];
                }
                *packet_received = 1;
                // debug(" OK");
            }
            // debug("\n"); debug_flush();
        }else{
            // no, ignore this
            len = 0;
        }
    }
}

void hal_cc25xx_transmit_packet(volatile uint8_t *buffer, uint8_t len) {
    // flush tx fifo
    hal_cc25xx_strobe(RFST_SFTX);
    // copy to fifo
    hal_cc25xx_register_write_multi(CC25XX_FIFO, (uint8_t *)buffer, buffer[0]+1);
    // and send!
    hal_cc25xx_strobe(RFST_STX);
}
