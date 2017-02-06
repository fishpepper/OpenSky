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
#include "hal_cc25xx.h"
#include <stdint.h>

#ifdef SBUS_ENABLED
#define hal_ppm_init() {}
#else
void hal_ppm_init(void);

void hal_ppm_failsafe_exit(void);
void hal_ppm_failsafe_enter(void);

void hal_ppm_timer1_interrupt(void) __interrupt T1_VECTOR;

/*
static void hal_ppm_init_rcc(void);
static void hal_ppm_init_gpio(void);
static void hal_ppm_init_timer(void);
static void hal_ppm_init_nvic(void);
static void hal_ppm_init_ocx(uint8_t ch, TIM_TypeDef *TIMx, TIM_OCInitTypeDef *tim_oc_init);
*/



// from frsky to ticks coresponding to 1000...2000 us
// frsky seems to send us*1.5 (~1480...3020) -> divide by 1.5 (=*2/3) to get us
// us -> ticks = ((_us*13)/4) -> (((_frsky*2/3)*13)/4) = ((_frsky*13)/6)
#define HAL_PPM_FRSKY_TO_TICKCOUNT(_frsky) (((_frsky << 3) + (_frsky << 2)+(_frsky))/6)

// from us to ticks:
//                               ((_us*13)/4) = ((_us * (8+4+1))/4) = (((_us<<3)+(_us<<2)+(_us))>>2)
#define HAL_PPM_US_TO_TICKCOUNT(_us) (((_us << 3) + (_us << 2)+(_us)) >> 2)


#define PPM_TIMER_ISR(void) hal_ppm_timer1_interrupt(void) __interrupt T1_VECTOR

#define HAL_PPM_UPDATE_CCVALUE(val) SET_WORD_LO_FIRST(T1CC0H, T1CC0L, val)
#define HAL_PPM_ISR_DISABLE() { cli(); }
#define HAL_PPM_ISR_ENABLE()  { sei(); }
#define HAL_PPM_ISR_FLAG_SET() (1)
#define HAL_PPM_ISR_CLEAR_FLAG() { \
    T1CTL &= ~(T1CTL_CH0_IF | T1CTL_CH1_IF | T1CTL_CH2_IF | T1CTL_OVFIF);}

#endif  // SBUS_ENABLED

#endif  // HAL_PPM_H_
