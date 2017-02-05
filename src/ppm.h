#ifndef __PPM_H__
#define __PPM_H__
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

#endif

#endif
