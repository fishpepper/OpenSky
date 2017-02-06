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

#ifndef SOFT_SERIAL_H_
#define SOFT_SERIAL_H_
#include <stdint.h>
#include "hal_soft_serial.h"
#include "main.h"

typedef void (*soft_serial_rx_callback_t)(uint8_t);

void soft_serial_init(void);
void soft_serial_set_rx_callback(soft_serial_rx_callback_t cb);

void soft_serial_process_startbit(void);
uint8_t soft_serial_process_databit(void);

#endif  // SOFT_SERIAL_H_
