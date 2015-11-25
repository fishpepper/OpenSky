#ifndef __PPM_H__
#define __PPM_H__
#include <stdio.h>
#include <stdint.h>
#include <cc2510fx.h>
#include "main.h"

void ppm_init(void);
void ppm_timer1_interrupt(void) __interrupt T1_VECTOR;


void ppm_update(__xdata uint16_t *data);
void ppm_exit_failsafe(void);
void ppm_enter_failsafe(void);

extern __xdata volatile uint8_t ppm_output_index;
extern __xdata uint16_t ppm_data_ticks[9];


//300us sync pulse
#define PPM_SYNC_DURATION_US 300
//from frsky to ticks coresponding to 1000...2000 us
//frsky seems to send us*1.5 (~1480...3020) -> divide by 1.5 (=*2/3) to get us
//us -> ticks = ((_us*13)/4) -> (((_frsky*2/3)*13)/4) = ((_frsky*13)/6)
#define PPM_FRSKY_TO_TICKCOUNT(_frsky) (((_frsky<<3)+(_frsky<<2)+(_frsky))/6)
//from us to ticks:
//                               ((_us*13)/4) = ((_us * (8+4+1))/4) = (((_us<<3)+(_us<<2)+(_us))>>2)
#define PPM_US_TO_TICKCOUNT(_us) (((_us<<3)+(_us<<2)+(_us))>>2)
#define PPM_FRAME_LEN PPM_US_TO_TICKCOUNT(20000L)
#define PPM_SYNC_PULS_LEN_TICKS (PPM_US_TO_TICKCOUNT(PPM_SYNC_DURATION_US))



#endif
