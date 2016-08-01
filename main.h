#ifndef __MAIN_H__
#define __MAIN_H__

#include "hal_defines.h"

//invert ppm polarity?
#define PPM_INVERTED 1
//enable SBUS ? (will disable PPM)
#define SBUS_ENABLED  0
#define SBUS_INVERTED 1

#define min(a,b) (((a)<(b))?(a):(b))
#define max(a,b) (((a)>(b))?(a):(b))


#endif // __MAIN_H__
