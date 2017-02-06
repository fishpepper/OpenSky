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

#ifndef SBUS_H_
#define SBUS_H_
#include <stdint.h>

#ifdef SBUS_ENABLED

void sbus_init(void);
void sbus_update(EXTERNAL_MEMORY uint16_t *data);
void sbus_start_transmission(uint8_t frame_lost);
void sbus_exit_failsafe(void);
void sbus_enter_failsafe(void);

// best match for 100kbit/s = 99975.5859375 bit/s
// baudrate = (((256.0 + baud_m)*2.0**baud_e) / (2**28)) * 26000000.0
#define SBUS_BAUD_E 11
#define SBUS_BAUD_M 248

#define SBUS_DATA_LEN 25
extern EXTERNAL_MEMORY uint8_t sbus_data[SBUS_DATA_LEN];

#define SBUS_SYNCBYTE 0x0F
#define SBUS_ENDBYTE  0x00
#define SBUS_FLAG_FRAME_LOST      (1<<2)
#define SBUS_FLAG_FAILSAFE_ACTIVE (1<<3)
#define SBUS_FRAME_LOST 1
#define SBUS_FRAME_NOT_LOST 0

#else

#define sbus_init() {}
#define sbus_update(a) {}
#define sbus_start_transmission(a) {}
#define sbus_exit_failsafe() {}
#define sbus_enter_failsafe() {}

#endif  // SBUS_ENABLED

#endif  // SBUS_H_
