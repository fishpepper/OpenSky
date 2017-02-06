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

#ifndef HAL_CLOCKSOURCE_H_
#define HAL_CLOCKSOURCE_H_

#include "hal_cc25xx.h"

void hal_clocksource_init(void);

// bit mask used to check the stability of XOSC
#define CLOCKSOURCE_XOSC_STABLE_BIT   0x40
// bit mak used to check the stability of the High-frequency RC oscillator
#define CLOCKSOURCE_HFRC_STB_BIT      0x20
// bit maks used to power down system clock oscillators
#define CLOCKSOURCE_OSC_PD_BIT        0x04

// bit mask used to control the system clock oscillator
#define CLOCKSOURCE_MAIN_OSC_BITS     0x7F

// bit mask used to select/check the system clock oscillator
#define CLOCKSOURCE_OSC_BIT           0x40

// macros to check for stable oscs:
#define CLOCKSOURCE_HFRC_OSC_STABLE() (SLEEP & (CLOCKSOURCE_HFRC_STB_BIT))
#define CLOCKSOURCE_XOSC_STABLE()     (SLEEP & (CLOCKSOURCE_XOSC_STABLE_BIT))

#define CLOCKSOURCE_XOSC 0
#define CLOCKSOURCE_HFRC 1

#endif  // HAL_CLOCKSOURCE_H_
