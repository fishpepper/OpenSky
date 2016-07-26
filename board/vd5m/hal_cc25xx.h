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

void cc25xx_process_packets(uint8_t *frsky_packet_received){
	//nothing to do, for vd5m we set the flag in the rf interrupt
}

void hal_cc25xx_disable_rf_interrupt(void) {
    IEN2 &= ~(IEN2_RFIE);
    RFIM = 0;
}
   
void hal_cc25xx_enter_rxmode(void) {
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

void hal_cc25xx_enter_txmode(void) {
    //abort ch0
    DMAARM = DMA_ARM_ABORT | DMA_ARM_CH0;
    frsky_setup_rf_dma(FRSKY_MODE_TX);
}

void hal_cc25xx_setup_rf_dma(uint8_t mode){
    // CPU has priority over DMA
    // Use 8 bits for transfer count
    // No DMA interrupt when done
    // DMA triggers on radio
    // Single transfer per trigger.
    // One byte is transferred each time.

    dma_config[0].PRIORITY       = DMA_PRI_HIGH;
    dma_config[0].M8             = DMA_M8_USE_8_BITS;
    dma_config[0].IRQMASK        = DMA_IRQMASK_DISABLE;
    dma_config[0].TRIG           = DMA_TRIG_RADIO;
    dma_config[0].TMODE          = DMA_TMODE_SINGLE;
    dma_config[0].WORDSIZE       = DMA_WORDSIZE_BYTE;

    //store mode
    frsky_mode = mode;

    if (frsky_mode == FRSKY_MODE_TX) {
        // Transmitter specific DMA settings
        // Source: radioPktBuffer
        // Destination: RFD register
        // Use the first byte read + 1
        // Sets the maximum transfer count allowed (length byte + data)
        // Data source address is incremented by 1 byte
        // Destination address is constant
        SET_WORD(dma_config[0].SRCADDRH, dma_config[0].SRCADDRL, frsky_packet_buffer);
        SET_WORD(dma_config[0].DESTADDRH, dma_config[0].DESTADDRL, &X_RFD);
        dma_config[0].VLEN           = DMA_VLEN_FIRST_BYTE_P_1;
        SET_WORD(dma_config[0].LENH, dma_config[0].LENL, (FRSKY_PACKET_LENGTH+1));
        dma_config[0].SRCINC         = DMA_SRCINC_1;
        dma_config[0].DESTINC        = DMA_DESTINC_0;
    }else{
        // Receiver specific DMA settings:
        // Source: RFD register
        // Destination: radioPktBuffer
        // Use the first byte read + 3 (incl. 2 status bytes)
        // Sets maximum transfer count allowed (length byte + data + 2 status bytes)
        // Data source address is constant
        // Destination address is incremented by 1 byte for each write
        SET_WORD(dma_config[0].SRCADDRH, dma_config[0].SRCADDRL, &X_RFD);
        SET_WORD(dma_config[0].DESTADDRH, dma_config[0].DESTADDRL, &frsky_packet_buffer[0]);
        dma_config[0].VLEN           = DMA_VLEN_FIRST_BYTE_P_3;
        SET_WORD(dma_config[0].LENH, dma_config[0].LENL, (FRSKY_PACKET_LENGTH+3));
        dma_config[0].SRCINC         = DMA_SRCINC_0;
        dma_config[0].DESTINC        = DMA_DESTINC_1;
    }

    // Save pointer to the DMA configuration struct into DMA-channel 0
    // configuration registers
    SET_WORD(DMA0CFGH, DMA0CFGL, &dma_config[0]);

    frsky_packet_received = 0;
}


void hal_cc25xx_enable_receive(void) {
	//start receiving on dma channel 0
	DMAARM = DMA_ARM_CH0;
}


void hal_cc25xx_enable_transmit(void) {
	//start transmitting on dma channel 0
	DMAARM = DMA_ARM_CH0;
	
	//tricky: this will force an int request and
	//        initiate the actual transmission
	S1CON |= 0x03;
	
	
    //wait some time here. packet should be sent within our 9ms
    //frame (actually within 5-6ms). if not print an error...
    frsky_packet_sent = 0;
    while(!frsky_packet_sent){
        if (timeout_timed_out()){
            break;
        }
    }
    if (timeout_timed_out()){
        debug("\nfrsky: ERROR tx timed out\n");
    }

    frsky_packet_sent = 0;
}

void hal_cc25xx_transmit_packet(buffer, len){
	nothing
	
}

#endif // __HAL_CC25XX_H__
