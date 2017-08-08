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

#ifndef MAIN_H_
#define MAIN_H_

#include "hal_defines.h"

// debugging data
#define DEBUG 1

// useful for debugging. DO NOT USE!
#define ADC_DO_TEST 0
#define TELEMETRY_DO_TEST 0


#define min(a, b) (((a) < (b)) ? (a) : (b))
#define max(a, b) (((a) > (b)) ? (a) : (b))


#define DEFINE_TO_STR(x) #x
#define DEFINE_TO_STR_VAL(x) DEFINE_TO_STR(x)

#endif  // MAIN_H_
