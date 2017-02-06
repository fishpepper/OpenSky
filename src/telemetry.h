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

#ifndef TELEMETRY_H_
#define TELEMETRY_H_

#include "hal_defines.h"
#include <stdint.h>

// this has to be a power of 2 (2,4,8,16,32,...)
#define TELEMETRY_BUFFER_SIZE 64
typedef struct {
    uint8_t data[TELEMETRY_BUFFER_SIZE];
    uint8_t read;
    uint8_t read_ok;
    uint8_t write;
} telemetry_buffer_t;

extern volatile EXTERNAL_MEMORY telemetry_buffer_t telemetry_buffer;
extern volatile EXTERNAL_MEMORY uint8_t telemetry_expected_id;

void telemetry_init(void);
void telemetry_rx_callback(uint8_t data);
static void telemetry_rx_echo_test(void);
void telemetry_fill_buffer(volatile EXTERNAL_MEMORY uint8_t *buffer, uint8_t telemetry_id);
uint8_t telemetry_pop(volatile EXTERNAL_MEMORY uint8_t *byte);

#endif  // TELEMETRY_H_
