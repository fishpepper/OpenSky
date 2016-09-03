#ifndef __MAIN_H__
#define __MAIN_H__

#include "hal_defines.h"

//enable SBUS output. NOTE: enabling SBUS will DISABLE ppm!
#define SBUS_ENABLED  1  //0 = disabled, 1 = enabled
//invert SBUS output
//Note: default/futaba is INVERTED=1
//for a CC3D running OpenPilot use SBUS_INVERTED=1 !
#define SBUS_INVERTED 1  //0 = not inverted => idle = high, 1 = INVERTED => idle = LOW
//invert ppm polarity?
//normal is non inverted (=0)
#define PPM_INVERTED 0

//is frsky hub telemerty inverted?
#define HUB_TELEMETRY_INVERTED 1

//debugging data
#define DEBUG 1


//useful for debugging. DO NOT USE!
#define ADC_DO_TEST 0
#define TELEMETRY_DO_TEST 0


#define min(a,b) (((a)<(b))?(a):(b))
#define max(a,b) (((a)>(b))?(a):(b))


#define DEFINE_TO_STR(x) #x
#define DEFINE_TO_STR_VAL(x) DEFINE_TO_STR(x)

#endif // __MAIN_H__
