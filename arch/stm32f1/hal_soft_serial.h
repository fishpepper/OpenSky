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

#ifndef HAL_SOFT_SERIAL_H_
#define HAL_SOFT_SERIAL_H_

#include "config.h"
#include "soft_serial.h"
#include "stm32f10x_gpio.h"


// at 9600 baud a bit duration is 1/9600s = 104.166667us
// the counter counts in 1/24th of us -> 104.1667us * 24 =
#define HAL_SOFTSERIAL_BIT_DURATION_TICKS  2500

void hal_soft_serial_init(void);

static void hal_soft_serial_init_rcc(void);
static void hal_soft_serial_init_gpio(void);
static void hal_soft_serial_init_timer(void);
static void hal_soft_serial_init_nvic(void);

#define HUB_TELEMETRY_PIN_LO_RAW() (GPIO_ReadInputDataBit(SOFT_SERIAL_GPIO, SOFT_SERIAL_PIN) == 0)
#define HUB_TELEMETRY_PIN_HI_RAW() (!HUB_TELEMETRY_PIN_LO_RAW())

#ifdef SOFT_SERIAL_PIN_HAS_INVERTER
  #define HUB_TELEMETRY_PIN_LO() HUB_TELEMETRY_PIN_HI_RAW()
  #define HUB_TELEMETRY_PIN_HI() HUB_TELEMETRY_PIN_LO_RAW()
#else
  #define HUB_TELEMETRY_PIN_LO() HUB_TELEMETRY_PIN_LO_RAW()
  #define HUB_TELEMETRY_PIN_HI() HUB_TELEMETRY_PIN_HI_RAW()
#endif  // SOFT_SERIAL_PIN_HAS_INVERTER

#define HAL_SOFT_SERIAL_IC_ISR_DISABLE()    {\
    TIM_ITConfig(SOFT_SERIAL_TIMER, SOFT_SERIAL_TIMER_IT_IC, DISABLE); }
#define HAL_SOFT_SERIAL_IC_ISR_ENABLE()     {\
    TIM_ITConfig(SOFT_SERIAL_TIMER, SOFT_SERIAL_TIMER_IT_IC, ENABLE); }
#define HAL_SOFT_SERIAL_IC_ISR_FLAG_SET()   \
    (TIM_GetITStatus(SOFT_SERIAL_TIMER, SOFT_SERIAL_TIMER_IT_IC) != RESET)
#define HAL_SOFT_SERIAL_IC_ISR_FLAG_CLEAR() {\
TIM_ClearITPendingBit(SOFT_SERIAL_TIMER, SOFT_SERIAL_TIMER_IT_IC); }

#define HAL_SOFT_SERIAL_UP_ISR_DISABLE()    {\
    TIM_ITConfig(SOFT_SERIAL_TIMER, SOFT_SERIAL_TIMER_IT_UP, DISABLE); }
#define HAL_SOFT_SERIAL_UP_ISR_ENABLE()     {\
TIM_ITConfig(SOFT_SERIAL_TIMER, SOFT_SERIAL_TIMER_IT_UP, ENABLE); }
#define HAL_SOFT_SERIAL_UP_ISR_FLAG_SET()   \
    (TIM_GetITStatus(SOFT_SERIAL_TIMER, SOFT_SERIAL_TIMER_IT_UP) != RESET)
#define HAL_SOFT_SERIAL_UP_ISR_FLAG_CLEAR() {\
TIM_ClearITPendingBit(SOFT_SERIAL_TIMER, SOFT_SERIAL_TIMER_IT_UP); }

#define HAL_SOFT_SERIAL_UPDATE_TOP_VALUE(x)   { SOFT_SERIAL_TIMER->ARR = x; }

void SOFT_SERIAL_TIMER_IC_IRQHandler(void);
void SOFT_SERIAL_TIMER_UP_IRQHandler(void);

#endif  // HAL_SOFT_SERIAL_H_
