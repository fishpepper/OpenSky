void hal_sbus_init(uint8_t *sbus_data_ptr) {

    EXTERNAL_MEMORY union uart_config_t sbus_uart_config;

    //we will use SERVO_4 as sbus output:
    //therefore we configure
    //USART1 use ALT1 -> Clear flag -> Port P0_4 = TX
    PERCFG &= ~(PERCFG_U1CFG);

    //configure pin P0_4 (TX) as output:
    P0SEL |= (1<<4);

    //make tx pin output:
    P0DIR |= (1<<4);

    //this assumes cpu runs from XOSC (26mhz) !
    //see sbus.h for calc and defines
    U1BAUD = SBUS_BAUD_M;
    U1GCR = (U1GCR & ~0x1F) | (SBUS_BAUD_E);

    //set up config for USART -> 8E2
    #if SBUS_INVERTED
        //this is a really nice feature of the cc2510:
        //we can invert the idle level of the usart
        //by setting STOP to zero. by inverting
        //the parity, the startbit, and the data
        //by using the SBUS_PREPARE_DATA() macro
        //we can effectively invert the usart in software :)
        sbus_uart_config.bit.START  = 1; //startbit level = low
        sbus_uart_config.bit.STOP   = 0; //stopbit level = high
        sbus_uart_config.bit.D9     = 1; //UNEven parity
    #else
        //standard usart, non-inverted mode
        //NOTE: most sbus implementations use inverted mode
        sbus_uart_config.bit.START  = 0; //startbit level = low
        sbus_uart_config.bit.STOP   = 1; //stopbit level = high
        sbus_uart_config.bit.D9     = 0; //Even parity
    #endif
    sbus_uart_config.bit.SPB    = 1; //1 = 2 stopbits
    sbus_uart_config.bit.PARITY = 1; //1 = parity enabled, D9=0 -> even parity
    sbus_uart_config.bit.BIT9   = 1; //8bit
    sbus_uart_config.bit.FLOW   = 0; //no hw flow control
    sbus_uart_config.bit.ORDER  = 0; //lsb first
    sbus_uart_set_mode(&sbus_uart_config);

    //use dma channel 3 for transmission:
    dma_config[3].PRIORITY       = DMA_PRI_LOW;
    dma_config[3].M8             = DMA_M8_USE_7_BITS;
    dma_config[3].IRQMASK        = DMA_IRQMASK_DISABLE;
    dma_config[3].TRIG           = DMA_TRIG_UTX1;
    dma_config[3].TMODE          = DMA_TMODE_SINGLE;
    dma_config[3].WORDSIZE       = DMA_WORDSIZE_BYTE;

    //important: src addr start is sbus_data[1] as we
    //initiate the transfer by manually sending sbus_data[0]!
    SET_WORD(dma_config[3].SRCADDRH,  dma_config[3].SRCADDRL,  &sbus_data_ptr[1]);
    SET_WORD(dma_config[3].DESTADDRH, dma_config[3].DESTADDRL, &X_U1DBUF);
    dma_config[3].VLEN           = DMA_VLEN_USE_LEN;

    //transfer SBUS_DATA_LEN-1 bytes (first byte is transmitted on start of transmission)
    SET_WORD(dma_config[3].LENH, dma_config[3].LENL, SBUS_DATA_LEN-1);
    dma_config[3].SRCINC         = DMA_SRCINC_1;
    dma_config[3].DESTINC        = DMA_DESTINC_0;

    //set pointer to the DMA configuration struct into DMA-channel 1-4
    //configuration, should have happened in adc.c already...
    SET_WORD(DMA1CFGH, DMA1CFGL, &dma_config[1]);

    //arm the relevant DMA channel for UART TX, and apply 45 NOP's
    //to allow the DMA configuration to load
    //-> do a sleep instead of those nops...
    DMAARM |= DMA_ARM_CH3;
    delay_us(100);
}


void sbus_uart_set_mode(EXTERNAL_MEMORY union uart_config_t *cfg){
    //enable uart mode
    U1CSR |= 0x80;

    //store config to U1UCR register
    U1UCR = cfg->byte & (0x7F);

    //store config to U1GCR: (msb/lsb)
    if (cfg->bit.ORDER){
        U1GCR |= U1GCR_ORDER;
    }else{
        U1GCR &= ~U1GCR_ORDER;
    }

    //interrupt prio to 1 (0..3=highest)
    IP0 |= (1<<3);
    IP1 &= ~(1<<3);
}

void hal_sbus_start_transmission(uint8_t *data, uint8_t len){
    //time to send this frame!
    //re-arm dma:
    DMAARM |= DMA_ARM_CH3;

    //send the very first UART byte to trigger a UART TX session:
    U1DBUF = data[0];
}
