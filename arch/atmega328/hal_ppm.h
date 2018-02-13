#ifndef __HAL_PPM_H__
#define __HAL_PPM_H__

#include "config.h"

void hal_ppm_init(void);

void hal_ppm_failsafe_exit(void);
void hal_ppm_failsafe_enter(void);
void hal_ppm_update_ccvalue(uint16_t len);
void ppm_timer_isr(void);


//counter runs with 2MHz = 0.5us resolution
#define HAL_PPM_US_TO_TICKCOUNT(us) ((((F_CPU/1000) * us)/1000)/64)
//from frsky to ticks coresponding to 1000...2000 us
//frsky seems to send us*1.5 (~1480...3020) -> divide by 1.5 (=*2/3) to get us
#define HAL_PPM_FRSKY_TO_TICKCOUNT(_frsky) HAL_PPM_US_TO_TICKCOUNT((_frsky)*2/3)

#define PPM_TIMER_ISR(void) ppm_timer_isr()

#define HAL_PPM_UPDATE_CCVALUE(x) hal_ppm_update_ccvalue(x)
#define HAL_PPM_ISR_DISABLE() { }
#define HAL_PPM_ISR_ENABLE()  { }
#define HAL_PPM_ISR_FLAG_SET() ( 1 )
#define HAL_PPM_ISR_CLEAR_FLAG() { }


#endif // __HAL_PPM_H__
