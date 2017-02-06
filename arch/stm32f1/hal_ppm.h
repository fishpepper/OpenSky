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

#ifndef HAL_PPM_H_
#define HAL_PPM_H_

#include "config.h"
#include "stm32f10x_tim.h"

void hal_ppm_init(void);

void hal_ppm_failsafe_exit(void);
void hal_ppm_failsafe_enter(void);

static void hal_ppm_init_rcc(void);
static void hal_ppm_init_gpio(void);
static void hal_ppm_init_timer(void);
static void hal_ppm_init_nvic(void);
static void hal_ppm_init_ocx(uint8_t ch, TIM_TypeDef *TIMx, TIM_OCInitTypeDef *tim_oc_init);

// counter runs with 2MHz = 0.5us resolution
#define HAL_PPM_US_TO_TICKCOUNT(us) ((us * 2)-1)
// from frsky to ticks coresponding to 1000...2000 us
// frsky seems to send us*1.5 (~1480...3020)
// -> divide by 1.5 (=*2/3) to get us -> multiply by 2 to get us
#define HAL_PPM_FRSKY_TO_TICKCOUNT(_frsky) ((_frsky)*2*2/3)

#define PPM_TIMER_ISR(void) PPM_TIMER_IRQHANDLER(void)

#define HAL_PPM_UPDATE_CCVALUE(x) { PPM_TIMER->ARR = x; }
#define HAL_PPM_ISR_DISABLE() { __disable_irq(); }
#define HAL_PPM_ISR_ENABLE()  { __enable_irq(); }
#define HAL_PPM_ISR_FLAG_SET() (TIM_GetITStatus(PPM_TIMER, TIM_IT_Update) != RESET)
#define HAL_PPM_ISR_CLEAR_FLAG() { TIM_ClearITPendingBit(PPM_TIMER, TIM_IT_Update); }


#endif  // HAL_PPM_H_
