#ifndef __CC25XX_H__
#define __CC25XX_H__

#include "hal_cc25xx.h"

extern uint8_t cc25xx_current_antenna;

void cc25xx_init(void);
void cc25xx_switch_antenna(void);

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
#define cc25xx_partnum_valid(p, v) hal_cc25xx_partnum_valid(p,v)
#define CC25XX_MODE_RX 0
#define CC25XX_MODE_TX 1

#endif // __CC25XX_H__

