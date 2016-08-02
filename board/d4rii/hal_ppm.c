#include "hal_ppm.h"
#include "ppm.h"
#include "debug.h"
#include "led.h"
#include "delay.h"
#include "wdt.h"
#include "pin_config.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_tim.h"
#include "misc.h" //stm32 nvic stuff

void hal_ppm_init(void) {
    hal_ppm_init_rcc();
    hal_ppm_init_gpio();
    hal_ppm_init_timer();
    hal_sbus_init_nvic();
}

static void hal_ppm_init_rcc(void) {
    // PCLK = HCLK set in system init...
    // do not set it here...

    // timer clock enable */
    RCC_APBxPeriphClockCmd(PPM_TIMER_CLK_RCC, PPM_TIMER_CLK, ENABLE);

    // gpio clock
    RCC_APBxPeriphClockCmd(PPM_GPIO_CLK_RCC, PPM_GPIO_CLK, ENABLE);

    // afio clk
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
}

static void hal_ppm_init_gpio(void) {
    GPIO_InitTypeDef gpio_init;

     // activate AF for ppm gpio:
     gpio_init.GPIO_Pin   = PPM_PIN;
     gpio_init.GPIO_Mode  = GPIO_Mode_AF_PP;
     gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
     GPIO_Init(PPM_GPIO, &gpio_init);
}

static void hal_ppm_init_timer(void) {
    TIM_TimeBaseInitTypeDef tim_init;
    TIM_OCInitTypeDef  tim_oc_init;

    TIM_TimeBaseStructInit(&tim_init);

    // time base configuration: count to 1000us (will be set properly lateron)
    tim_init.TIM_Period         = HAL_PPM_US_TO_TICKCOUNT(1000);
    // compute the prescaler value, we want a 0.5us resolution (= count with 2mhz):
    tim_init.TIM_Prescaler      = (uint16_t) (SystemCoreClock / 2000000) - 1;
    tim_init.TIM_Period         = 0xFFFF;
    //tim_init.TIM_Prescaler      = (uint16_t) (SystemCoreClock / 200) - 1;
    tim_init.TIM_ClockDivision  = TIM_CKD_DIV1;
    tim_init.TIM_CounterMode    = TIM_CounterMode_Up;
    TIM_TimeBaseInit(PPM_TIMER, &tim_init);


    //Output Compare Active Mode configuration:
#if PPM_INVERTED
    tim_oc_init.TIM_OCMode      = TIM_OCMode_PWM2;
#else
    tim_oc_init.TIM_OCMode      = TIM_OCMode_PWM1;
#endif
    tim_oc_init.TIM_OutputState = TIM_OutputState_Enable;
    tim_oc_init.TIM_Pulse       = PPM_SYNC_PULS_LEN_TICKS;
    tim_oc_init.TIM_OCPolarity  = TIM_OCPolarity_High;

    switch(PPM_TIMER_CH){
        default:
        case(3):
            TIM_OC3Init(PPM_TIMER, &tim_oc_init);
            TIM_OC3PreloadConfig(PPM_TIMER, TIM_OCPreload_Disable);
            break;
        case(2):
            TIM_OC2Init(PPM_TIMER, &tim_oc_init);
            TIM_OC2PreloadConfig(PPM_TIMER, TIM_OCPreload_Disable);
            break;
        case(1):
            TIM_OC1Init(PPM_TIMER, &tim_oc_init);
            TIM_OC1PreloadConfig(PPM_TIMER, TIM_OCPreload_Disable);
            break;
    }

    TIM_Cmd(PPM_TIMER, ENABLE);
}

static void hal_sbus_init_nvic(void) {
    NVIC_InitTypeDef nvic_init;

    // configure the NVIC Preemption Priority Bits
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);

    //disable all timer interrupts
    TIM_ITConfig(PPM_TIMER, TIM_IT_Update, DISABLE);
    NVIC_DisableIRQ(PPM_TIMER_IRQn);

    // enable timer interrupt
    nvic_init.NVIC_IRQChannel                   = PPM_TIMER_IRQn;
    nvic_init.NVIC_IRQChannelPreemptionPriority = 0;
    nvic_init.NVIC_IRQChannelSubPriority        = 14;
    nvic_init.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_Init(&nvic_init);

    //NVIC_EnableIRQ(PPM_TIMER_IRQn);

    //enable interrupt
    TIM_ITConfig(PPM_TIMER, TIM_IT_Update, ENABLE);

    //output enable for counter
    //TIM_CtrlPWMOutputs(PPM_TIMER, ENABLE);
}

void hal_ppm_failsafe_enter(void){
     return;
    //set output to static value ZERO
    TIM_ITConfig(PPM_TIMER, TIM_IT_Update, DISABLE);

    //set output to static low or high level:
    GPIO_InitTypeDef gpio_init;
    gpio_init.GPIO_Pin   = PPM_PIN;
    gpio_init.GPIO_Mode  = GPIO_Mode_Out_PP;
    gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(PPM_GPIO, &gpio_init);

#if PPM_INVERTED
    //clear on zero -> default is high
    PPM_GPIO->BSRR = (PPM_PIN);
#else
    //set on zero -> default is low
    PPM_GPIO->BRR  = (PPM_PIN);
#endif
}

void hal_ppm_failsafe_exit(void) {
    return;

    //exit failsafe, back to pulse generation
    hal_ppm_init_gpio();

    //re-enable ppm output isr
    TIM_ITConfig(PPM_TIMER, TIM_IT_Update, ENABLE);
}


void PPM_TIMER_IRQHANDLER(void){
    led_red_on();

    do a toggle here
    //led_green_off();
    //debug_put_uint16(TIM3->SR);
    uint32_t r = TIM3->SR;
    wdt_reset();
    if (TIM_GetITStatus(PPM_TIMER, TIM_IT_Update) != RESET){
        //clear flag
        TIM_ClearITPendingBit(PPM_TIMER, TIM_IT_Update);
        //do processing
        //ppm_isr();
    }
    if (TIM_GetITStatus(PPM_TIMER, TIM_IT_CC1) != RESET){
        TIM_ClearITPendingBit(PPM_TIMER, TIM_IT_CC1);
    }
    if (TIM_GetITStatus(PPM_TIMER, TIM_IT_CC2) != RESET){
        TIM_ClearITPendingBit(PPM_TIMER, TIM_IT_CC2);
    }
    if (TIM_GetITStatus(PPM_TIMER, TIM_IT_CC3) != RESET){
        TIM_ClearITPendingBit(PPM_TIMER, TIM_IT_CC3);
    }
    if (TIM_GetITStatus(PPM_TIMER, TIM_IT_CC4) != RESET){
        TIM_ClearITPendingBit(PPM_TIMER, TIM_IT_CC4);
    }
    //TIM3->SR = 0xffff;
}

