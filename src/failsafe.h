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

#ifndef FAILSAFE_H__
#define FAILSAFE_H__
#include "main.h"
#include "ppm.h"
#include "sbus.h"
#include <stdint.h>

void failsafe_init(void);
void failsafe_enter(void);
void failsafe_exit(void);
void failsafe_tick(void);

extern EXTERNAL_MEMORY volatile uint8_t failsafe_active;
extern EXTERNAL_MEMORY volatile uint16_t failsafe_tick_counter;

#endif  // FAILSAFE_H_
