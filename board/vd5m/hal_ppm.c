
void ppm_timer1_interrupt(void) __interrupt T1_VECTOR{
    //clear pending interrupt flags (IRCON is reset by hw)
    T1CTL &= ~(T1CTL_CH0_IF | T1CTL_CH1_IF | T1CTL_CH2_IF | T1CTL_OVFIF);

    ppm_isr();
}


void hal_ppm_init(void) {

    //no int on overflow:
    OVFIM = 0;

    //set channels to compare interupt:
    //CH0: off
    T1CCTL0 = 0;
    //CH1: off
    T1CCTL1 = 0;
    //CH2: toggle pin on cmp match, will generate sync pulses
    #if PPM_INVERTED
    //inverted, set on match, clear on zero
    T1CCTL2 = T1CCTLx_MODE_COMPARE | T1CCTLx_CMP_SETCLR0;
    #else
    //non-inverted, clear on match, set on zero
    T1CCTL2 = T1CCTLx_MODE_COMPARE | T1CCTLx_CMP_CLRSET0;
    #endif

    //configure peripheral alternative1 for timer 1:
    //use alt config 1 -> clr flag -> P0_4 = output
    PERCFG &= ~(PERCFG_T1CFG);

    //USART1 use ALT2 in order to free up P0_4 for peripheral func
    PERCFG |= PERCFG_U1CFG;

    //select P0_4 for peripheral function
    //NOTE: make sure to set usart1 to alt2 config!
    P0SEL |= (1<<4);

    //select P0_4 as output
    P0DIR |= (1<<4);

    //prescaler = 128
    //tickspeed = 26MHz / 8 = 3,25MHz (TICKSPD is set in timeout.c!)
    //1us = 3.25 ticks -> 2ms = 6500 ticks, 4ms = 13000
    T1CTL = T1CTL_MODE_MODULO | T1CTL_DIV_1;


    //ch2 cmp: sync pulse length
    SET_WORD_LO_FIRST(T1CC2H, T1CC2L, PPM_SYNC_PULS_LEN_TICKS);

    //overflow:
    SET_WORD_LO_FIRST(T1CC0H, T1CC0L, PPM_US_TO_TICKCOUNT(1000));

    ppm_output_index = 0;

    //clear pending interrupt flags (IRCON is reset by hw)
    T1CTL &= ~(T1CTL_CH0_IF | T1CTL_CH1_IF | T1CTL_CH2_IF | T1CTL_OVFIF);

    //overflow causes an int -> reload next channel data
    OVFIM = 1;

    //enable T1 interrups
    T1IE = 1;
}


void hal_ppm_failsafe_exit() {
    //configure p0_4 as peripheral:
    P0SEL |= (1<<4);

    //reset counter:
    SET_WORD_LO_FIRST(T1CNTH, T1CNTL, 0);

    //re enable timer interrupts:
    OVFIM = 1;

    //disable T1 interrups
    T1IE = 1;
}

void hal_ppm_failsafe_enter(void) {
    //disable interrupts
    OVFIM = 0;

    //disable T1 interrups
    T1IE = 0;

    //configure p0_4 as normal i/o:
    P0SEL &= ~(1<<4);

    //set pins to failsafe level:
    #if PPM_INVERTED
    //clear on zero -> default is high
    P0 |= (1<<4);
    #else
    //set on zero -> default is low
    P0 &= ~(1<<4);
    #endif
}
