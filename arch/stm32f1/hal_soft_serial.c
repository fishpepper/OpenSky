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

#include "hal_soft_serial.h"
#include "debug.h"
#include "config.h"
#include "led.h"
#include "soft_serial.h"
#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_tim.h"
#include "misc.h"  // stm32 nvic

void hal_soft_serial_init(void) {
    hal_soft_serial_init_gpio();
    hal_soft_serial_init_rcc();
    hal_soft_serial_init_timer();
    hal_soft_serial_init_nvic();
}

static void hal_soft_serial_init_rcc(void) {
    // enable clocks
    RCC_APBxPeriphClockCmd(SOFT_SERIAL_CLK_RCC, SOFT_SERIAL_CLK, ENABLE);
    // timer clock enable
    RCC_APBxPeriphClockCmd(SOFT_SERIAL_TIMER_CLK_RCC, SOFT_SERIAL_TIMER_CLK, ENABLE);
    // release reset cmd (?)
    RCC_APBxPeriphResetCmd(SOFT_SERIAL_TIMER_CLK_RCC, SOFT_SERIAL_TIMER_CLK, DISABLE);
}

static void hal_soft_serial_init_gpio(void) {
    GPIO_InitTypeDef gpio_init;

    // configure rx pin as input:
    gpio_init.GPIO_Pin   = SOFT_SERIAL_PIN;
    gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
    gpio_init.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
    GPIO_Init(SOFT_SERIAL_GPIO, &gpio_init);
}


static void hal_soft_serial_init_timer(void) {
    TIM_TimeBaseInitTypeDef tim_init;
    TIM_ICInitTypeDef tim_ic_init;

    // time base configuration:
    TIM_TimeBaseStructInit(&tim_init);
    // this serial uart runs at 9600 baud, thus bitlength = 1/9600 = 104.166667 us
    // a 1mhz counter gives us 1us resolution, a 24mhz counter gives us 1/24 us res
    // the finer the better -> go for 24mhz counter = prescaler = 0 (:1)
    tim_init.TIM_Prescaler      = (uint16_t) (0);
    // timer period = bit duration
    tim_init.TIM_Period         = 0xFFFF;
    tim_init.TIM_ClockDivision  = 0;
    tim_init.TIM_CounterMode    = TIM_CounterMode_Up;

    // set time base. NOTE: this will immediately trigger an INT!
    TIM_TimeBaseInit(SOFT_SERIAL_TIMER, &tim_init);

    // clear IT flag (caused by TimeBaseInit()):
    TIM_ClearITPendingBit(SOFT_SERIAL_TIMER, TIM_IT_Update);

    TIM_ICStructInit(&tim_ic_init);
    tim_ic_init.TIM_Channel        = SOFT_SERIAL_TIMER_CH;
#ifdef HUB_TELEMETRY_INVERTED
  #ifdef SOFT_SERIAL_PIN_HAS_INVERTER
    // board has inverter -> invert twice = no inversion
    tim_ic_init.TIM_ICPolarity     = TIM_ICPolarity_Falling;
  #else
    tim_ic_init.TIM_ICPolarity     = TIM_ICPolarity_Rising;
  #endif  // SOFT_SERIAL_PIN_HAS_INVERTER
#else
  #ifdef SOFT_SERIAL_PIN_HAS_INVERTER
    // board has inverter -> invert
    tim_ic_init.TIM_ICPolarity     = TIM_ICPolarity_Rising;
  #else
    tim_ic_init.TIM_ICPolarity     = TIM_ICPolarity_Falling;
  #endif  // SOFT_SERIAL_PIN_HAS_INVERTER
#endif  // HUB_TELEMETRY_INVERTED
    tim_ic_init.TIM_ICSelection    = TIM_ICSelection_DirectTI;
    tim_ic_init.TIM_ICPrescaler    = TIM_ICPSC_DIV1;
    tim_ic_init.TIM_ICFilter       = 0x0;
    TIM_ICInit(SOFT_SERIAL_TIMER, &tim_ic_init);

    TIM_ClearITPendingBit(SOFT_SERIAL_TIMER, SOFT_SERIAL_TIMER_IT_IC);
/*
    // Output Compare Active Mode configuration:
    TIM_OCStructInit(&tim_oc_init);
    tim_oc_init.TIM_OCMode      = TIM_OCMode_ disable;
    tim_oc_init.TIM_OutputState = TIM_OutputState_ disable;
    tim_oc_init.TIM_Pulse       = PPM_SYNC_PULS_LEN_TICKS;
    tim_oc_init.TIM_OCPolarity  = TIM_OCPolarity_High;
    hal_ppm_init_ocx(PPM_TIMER_CH, PPM_TIMER, &tim_oc_init);
    */
    // enable counter
    TIM_Cmd(SOFT_SERIAL_TIMER, ENABLE);
}


static void hal_soft_serial_init_nvic(void) {
    NVIC_InitTypeDef nvic_init;

    // strange, somehow the Timer IT Flags seem to be already enabled?!
    TIM_ITConfig(SOFT_SERIAL_TIMER, TIM_IT_Update, DISABLE);
    TIM_ITConfig(SOFT_SERIAL_TIMER, TIM_IT_Break, DISABLE);
    TIM_ITConfig(SOFT_SERIAL_TIMER, TIM_IT_CC1 | TIM_IT_CC2 | TIM_IT_CC3 | TIM_IT_CC4, DISABLE);
    TIM_ITConfig(SOFT_SERIAL_TIMER, TIM_IT_Trigger, DISABLE);

    // enable input capture timer interrupt
    nvic_init.NVIC_IRQChannel                   = SOFT_SERIAL_TIMER_IC_IRQn;
    nvic_init.NVIC_IRQChannelPreemptionPriority = NVIC_PRIO_SOFT_SERIAL;
    nvic_init.NVIC_IRQChannelSubPriority        = 0;
    nvic_init.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_Init(&nvic_init);

    // enable update timer interrupt
    nvic_init.NVIC_IRQChannel                   = SOFT_SERIAL_TIMER_UP_IRQn;
    nvic_init.NVIC_IRQChannelPreemptionPriority = NVIC_PRIO_SOFT_SERIAL;
    nvic_init.NVIC_IRQChannelSubPriority        = 0;
    nvic_init.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_Init(&nvic_init);


    // for now enable ONLY the capture interrupt
    TIM_ClearITPendingBit(SOFT_SERIAL_TIMER, SOFT_SERIAL_TIMER_IT_IC);
    TIM_ITConfig(SOFT_SERIAL_TIMER, SOFT_SERIAL_TIMER_IT_IC, ENABLE);
}


void SOFT_SERIAL_TIMER_IC_IRQHandler(void) {
    // handle startbit:
    if (HAL_SOFT_SERIAL_IC_ISR_FLAG_SET()) {
        // reset counter
        TIM_SetCounter(SOFT_SERIAL_TIMER, 0);
        // this is the startbit -> re synchronize the timer to this
        // by setting the next cc interrupt to 1/2 bit length:
        HAL_SOFT_SERIAL_UPDATE_TOP_VALUE(HAL_SOFTSERIAL_BIT_DURATION_TICKS / 2);

        // disable IC interrupt (only compare match interrupts will trigger this isr)
        HAL_SOFT_SERIAL_IC_ISR_DISABLE();

        // enable overflow isr
        HAL_SOFT_SERIAL_UP_ISR_ENABLE();

        // clear flag - NOTE: this should never be done at the end of the isr!
        HAL_SOFT_SERIAL_IC_ISR_FLAG_CLEAR();
        // clear any pending update flags as well
        HAL_SOFT_SERIAL_UP_ISR_FLAG_CLEAR();

        // process
        soft_serial_process_startbit();
    }
}

void SOFT_SERIAL_TIMER_UP_IRQHandler(void) {
    if (HAL_SOFT_SERIAL_UP_ISR_FLAG_SET()) {
        // clear flag - NOTE: this should never be done at the end of the isr!
        HAL_SOFT_SERIAL_UP_ISR_FLAG_CLEAR();

        // re-arm for the next bit
        HAL_SOFT_SERIAL_UPDATE_TOP_VALUE(HAL_SOFTSERIAL_BIT_DURATION_TICKS);

        if (soft_serial_process_databit()) {
            // finished transmission, disable UP and enable IC isr
            HAL_SOFT_SERIAL_UP_ISR_DISABLE();
            HAL_SOFT_SERIAL_IC_ISR_FLAG_CLEAR();
            HAL_SOFT_SERIAL_IC_ISR_ENABLE();
        }
    }
}


