#ifndef __MAIN_H__
#define __MAIN_H__

#include "hal_defines.h"

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
