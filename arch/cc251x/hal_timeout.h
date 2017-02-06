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

#include <stdint.h>
#include "cc2510fx.h"

extern volatile uint16_t hal_timeout_countdown;

void hal_timeout_init(void);
void hal_timeout_set(uint16_t timeout_ms);
uint8_t hal_timeout_timed_out(void);

void hal_timeout2_set(uint16_t ms);
void hal_timeout2_set_100us(uint16_t hus);
uint8_t hal_timeout2_timed_out(void);

void hal_timeout_interrupt(void) __interrupt T3_VECTOR;

#endif  // HAL_TIMEOUT_H_
