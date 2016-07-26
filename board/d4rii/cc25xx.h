#ifndef __CC25XX_H__
#define __CC25XX_H__

#include "hal_cc25xx.h"

void cc25xx_init(void);
#define cc25xx_set_register(reg, val) hal_cc25xx_set_register(reg, val);
#define cc25xx_strobe(val) hal_cc25xx_strobe(val);
#define cc25xx_get_register(address) hal_cc25xx_get_register(address)
#define cc25xx_enable_receive() hal_cc25xx_enable_receive()
#define cc25xx_enter_rxmode() hal_cc25xx_enter_rxmode()
#define cc25xx_get_register_burst(address) hal_cc25xx_get_register_burst(address)
#define cc25xx_set_gdo_mode() hal_cc25xx_set_gdo_mode()

#define cc25xx_process_packets(ptr1, ptr2, len) hal_cc25xx_process_packets(ptr1, ptr2, len)
#define cc25xx_disable_rf_interrupt() hal_cc25xx_disable_rf_interrupt()

void cc25xx_switch_antenna(void);

#endif // __CC25XX_H__
