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

#ifndef HAL_SBUS_H_
#define HAL_SBUS_H_

#include <stdint.h>
#include "hal_defines.h"
#include "hal_uart.h"

// this helper routine will invert the data
// stored in buffer in case the sbus is set
// to inverted
#ifdef SBUS_INVERTED
  #define HAL_SBUS_PREPARE_DATA(a) (0xFF ^ (a))
#else
  #define HAL_SBUS_PREPARE_DATA(a) (a)
#endif  // SBUS_INVERTED

#endif  // HAL_SBUS_H_
