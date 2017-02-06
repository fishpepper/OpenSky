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


#ifndef TIMEOUT_H_
#define TIMEOUT_H_
#include "hal_timeout.h"

void timeout_init(void);
#define timeout_set(x) hal_timeout_set(x)
#define timeout_set_100us(x) hal_timeout_set_100us(x)
#define timeout_timed_out() hal_timeout_timed_out()
#define timeout_time_remaining() hal_timeout_time_remaining()

#define timeout2_set_100us(x) hal_timeout2_set_100us(x)
#define timeout2_timed_out() hal_timeout2_timed_out()
void timeout2_delay_100us(uint16_t us);

#endif  // TIMEOUT_H_
