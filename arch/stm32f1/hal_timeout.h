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

#ifndef HAL_TIMEOUT_H_
#define HAL_TIMEOUT_H_

#include "stm32f10x.h"

void hal_timeout_init(void);
void hal_timeout_set(__IO uint32_t ms);
void hal_timeout_set_100us(__IO uint32_t hus);
uint8_t hal_timeout_timed_out(void);
void hal_timeout_delay_ms(uint32_t timeout);

void hal_timeout2_set(__IO uint32_t ms);
void hal_timeout2_set_100us(__IO uint32_t hus);
uint8_t hal_timeout2_timed_out(void);

uint32_t hal_timeout_time_remaining(void);

void SysTick_Handler(void);

#endif  // HAL_TIMEOUT_H_
