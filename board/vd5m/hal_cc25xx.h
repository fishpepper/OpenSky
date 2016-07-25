#ifndef __HAL_CC25XX_H__
#define __HAL_CC25XX_H__

#include "cc2510fx.h"

#define hal_cc25xx_init() {}
#define hal_cc25xx_set_register(reg, val) { reg = val; }
#define hal_cc25xx_strobe(val) { RFST = val; }
#define hal_cc25xx_get_register(r) (r)

#define CC25XX_FIFO FIFO

void hal_cc25xx_set_gdo_mode(void) {
	//not necessary here IOCFG0 = 0x01
	//not necessary here IOCFG2 = 0x0E
}

   
void hal_enter_rxmode(void) {
    //set up dma for radio--->buffer
    frsky_setup_rf_dma(FRSKY_MODE_RX);

    //configure interrupt for every received packet
    IEN2 |= (IEN2_RFIE);

    //set highest prio for ch0 (RF)
    IP0 |= (1<<0);
    IP1 |= (1<<0);

    //mask done irq
    RFIM = (1<<4);
    //interrupts should be enabled globally already..
    //skip this! sei();
}

void hal_cc25xx_enable_receive(void) {
	//start receiving on dma channel 0
    DMAARM = DMA_ARM_CH0;
}

#endif // __HAL_CC25XX_H__
