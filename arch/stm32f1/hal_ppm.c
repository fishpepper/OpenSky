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

   author: fishpepper <AT> gmail.com
*/

#include "hal_ppm.h"
#include "ppm.h"
#include "wdt.h"
#include "led.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_tim.h"
#include "misc.h" //stm32 nvic stuff

#ifndef SBUS_ENABLED

void hal_ppm_init(void) {
    hal_ppm_init_rcc();
    hal_ppm_init_gpio();
    hal_ppm_init_timer();
    hal_ppm_init_nvic();
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
    tim_init.TIM_ClockDivision  = 0;
    tim_init.TIM_CounterMode    = TIM_CounterMode_Up;

    //set time base. NOTE: this will immediately trigger an INT!
    TIM_TimeBaseInit(PPM_TIMER, &tim_init);

    //clear IT flag (caused by TimeBaseInit()):
    TIM_ClearITPendingBit(PPM_TIMER, TIM_IT_Update);

    //Output Compare Active Mode configuration:
    TIM_OCStructInit(&tim_oc_init);
#if PPM_INVERTED
    tim_oc_init.TIM_OCMode      = TIM_OCMode_PWM2;
#else
    tim_oc_init.TIM_OCMode      = TIM_OCMode_PWM1;
#endif
    tim_oc_init.TIM_OutputState = TIM_OutputState_Enable;
    tim_oc_init.TIM_Pulse       = PPM_SYNC_PULS_LEN_TICKS;
    tim_oc_init.TIM_OCPolarity  = TIM_OCPolarity_High;
    hal_ppm_init_ocx(PPM_TIMER_CH, PPM_TIMER, &tim_oc_init);

    //enable counter
    TIM_Cmd(PPM_TIMER, ENABLE);
}

static void hal_ppm_init_nvic(void) {
    NVIC_InitTypeDef nvic_init;

    //strange, somehow the Timer IT Flags seem to be already enabled?!
    TIM_ITConfig(PPM_TIMER, TIM_IT_Update, DISABLE);
    TIM_ITConfig(PPM_TIMER, TIM_IT_Break, DISABLE);
    TIM_ITConfig(PPM_TIMER, TIM_IT_CC1 | TIM_IT_CC2 | TIM_IT_CC3 | TIM_IT_CC4, DISABLE);
    TIM_ITConfig(PPM_TIMER, TIM_IT_Trigger, DISABLE);

    // enable timer interrupt
    nvic_init.NVIC_IRQChannel                   = PPM_TIMER_IRQn;
    nvic_init.NVIC_IRQChannelPreemptionPriority = NVIC_PRIO_PPM;
    nvic_init.NVIC_IRQChannelSubPriority        = 0;
    nvic_init.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_Init(&nvic_init);

    //enable ONLY update interrupt
    TIM_ITConfig(PPM_TIMER, TIM_IT_Update, ENABLE);
}

void hal_ppm_failsafe_enter(void){
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
    //exit failsafe, back to pulse generation
    hal_ppm_init_gpio();

    //re-enable ppm output isr
    TIM_ITConfig(PPM_TIMER, TIM_IT_Update, ENABLE);
}

static void hal_ppm_init_ocx(uint8_t ch, TIM_TypeDef *TIMx, TIM_OCInitTypeDef *tim_oc_init) {
    switch(PPM_TIMER_CH){
        default:
            break;
        case(TIM_Channel_4):
            TIM_OC4Init(TIMx, tim_oc_init);
            TIM_OC4PreloadConfig(TIMx, TIM_OCPreload_Disable);
            break;
        case(TIM_Channel_3):
            TIM_OC3Init(TIMx, tim_oc_init);
            TIM_OC3PreloadConfig(TIMx, TIM_OCPreload_Disable);
            break;
        case(TIM_Channel_2):
            TIM_OC2Init(TIMx, tim_oc_init);
            TIM_OC2PreloadConfig(TIMx, TIM_OCPreload_Disable);
            break;
        case(TIM_Channel_1):
            TIM_OC1Init(TIMx, tim_oc_init);
            TIM_OC1PreloadConfig(TIMx, TIM_OCPreload_Disable);
            break;
    }
}

#endif
