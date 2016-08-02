#ifndef __MAIN_H__
#define __MAIN_H__

#include "hal_defines.h"

//enable SBUS output. NOTE: enabling SBUS will DISABLE ppm!
#define SBUS_ENABLED  0  //0 = disabled, 1 = enabled
//invert SBUS output
//Note: default/futaba is INVERTED=1
//for a CC3D running OpenPilot use SBUS_INVERTED=1 !
#define SBUS_INVERTED 1  //0 = not inverted => idle = high, 1 = INVERTED => idle = LOW
//invert ppm polarity?
#define PPM_INVERTED 1


//send ADC data as hub telemetry as well:
#define FRSKY_SEND_HUB_TELEMETRY 0



//useful for debugging. DO NOT USE!
#define ADC_DO_TEST 0


#define min(a,b) (((a)<(b))?(a):(b))
#define max(a,b) (((a)>(b))?(a):(b))


#endif // __MAIN_H__
