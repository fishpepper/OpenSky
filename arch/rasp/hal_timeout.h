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

void hal_timeout_init(void);
void hal_timeout_set(uint32_t ms);
void hal_timeout_set_100us(uint32_t hus);
uint8_t hal_timeout_timed_out(void);
void hal_timeout_delay_us(int32_t timeout);

void hal_timeout2_set(uint32_t ms);
void hal_timeout2_set_100us(uint32_t hus);
uint8_t hal_timeout2_timed_out(void);
void hal_timeout_add_ppm(uint32_t us);

// void hal_timeout_delay_100us(uint32_t timeout);
uint32_t hal_timeout_time_remaining(void);


#endif  // HAL_TIMEOUT_H_
