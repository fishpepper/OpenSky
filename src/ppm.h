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

#ifndef PPM_H_
#define PPM_H_
#include <stdint.h>
#include "main.h"
#include "hal_ppm.h"

#ifdef SBUS_ENABLED

// ppm is not used then
#define ppm_init() {}
#define ppm_update(a) {}
#define ppm_exit_failsafe() {}
#define ppm_enter_failsafe() {}


#else
void ppm_init(void);
void ppm_update(EXTERNAL_MEMORY uint16_t *data);
void ppm_exit_failsafe(void);
void ppm_enter_failsafe(void);
void ppm_isr(void);

extern EXTERNAL_MEMORY volatile uint8_t ppm_output_index;
extern EXTERNAL_MEMORY uint16_t ppm_data_ticks[9];


// #define PPM_US_TO_TICKCOUNT(us) HAL_PPM_US_TO_TICKCOUNT(us)
#define PPM_FRSKY_TO_TICKCOUNT(_frsky) HAL_PPM_FRSKY_TO_TICKCOUNT(_frsky);
#define PPM_FRAME_LEN HAL_PPM_US_TO_TICKCOUNT(20000L)
#define PPM_SYNC_PULS_LEN_TICKS (HAL_PPM_US_TO_TICKCOUNT(PPM_SYNC_DURATION_US))

// 300us sync pulse
#define PPM_SYNC_DURATION_US 300
// #define PPM_SYNC_PULS_LEN_TICKS  PPM_US_TO_TICKCOUNT(PPM_SYNC_DURATION_US)

#endif  // SBUS_ENABLED

#endif  // PPM_H_
