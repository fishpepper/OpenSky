/*
    Copyright 2017 fishpepper <AT> gmail.com

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

#ifndef CC25XX_H_
#define CC25XX_H_

#include "hal_cc25xx.h"

extern uint8_t cc25xx_current_antenna;

void cc25xx_init(void);
void cc25xx_switch_antenna(void);
void cc25xx_wait_for_transmission_complete(void);

#define cc25xx_set_register(reg, val) hal_cc25xx_set_register(reg, val);
#define cc25xx_strobe(val) hal_cc25xx_strobe(val);
#define cc25xx_get_register(address) hal_cc25xx_get_register(address)
#define cc25xx_enable_receive() hal_cc25xx_enable_receive()
#define cc25xx_enter_rxmode() hal_cc25xx_enter_rxmode()
#define cc25xx_enter_txmode() hal_cc25xx_enter_txmode()
#define cc25xx_get_register_burst(address) hal_cc25xx_get_register_burst(address)
#define cc25xx_set_gdo_mode() hal_cc25xx_set_gdo_mode()
#define cc25xx_setup_rf_dma(mode) hal_cc25xx_setup_rf_dma(mode)
#define cc25xx_rx_sleep() hal_cc25xx_rx_sleep()
#define cc25xx_tx_sleep() hal_cc25xx_tx_sleep()
#define cc25xx_process_packet(ptr1, ptr2, len) hal_cc25xx_process_packet(ptr1, ptr2, len)
#define cc25xx_disable_rf_interrupt() hal_cc25xx_disable_rf_interrupt()
#define cc25xx_transmit_packet(buffer, len) hal_cc25xx_transmit_packet(buffer, len)

#define cc25xx_get_current_antenna() (cc25xx_current_antenna)
#define cc25xx_partnum_valid(p, v) hal_cc25xx_partnum_valid(p, v)
#define CC25XX_MODE_RX 0
#define CC25XX_MODE_TX 1

#endif  // CC25XX_H_

