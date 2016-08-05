#ifndef __HAL_PPM_H__
#define __HAL_PPM_H__

#include "config.h"
#include "hal_cc25xx.h"
#include <stdint.h>

void hal_ppm_init(void);

void hal_ppm_failsafe_exit(void);
void hal_ppm_failsafe_enter(void);

void hal_ppm_timer1_interrupt(void) __interrupt T1_VECTOR;

/*
static void hal_ppm_init_rcc(void);
static void hal_ppm_init_gpio(void);
static void hal_ppm_init_timer(void);
static void hal_ppm_init_nvic(void);
static void hal_ppm_init_ocx(uint8_t ch, TIM_TypeDef *TIMx, TIM_OCInitTypeDef *tim_oc_init);
*/



//from frsky to ticks coresponding to 1000...2000 us
//frsky seems to send us*1.5 (~1480...3020) -> divide by 1.5 (=*2/3) to get us
//us -> ticks = ((_us*13)/4) -> (((_frsky*2/3)*13)/4) = ((_frsky*13)/6)
#define HAL_PPM_FRSKY_TO_TICKCOUNT(_frsky) (((_frsky<<3)+(_frsky<<2)+(_frsky))/6)

//from us to ticks:
//                               ((_us*13)/4) = ((_us * (8+4+1))/4) = (((_us<<3)+(_us<<2)+(_us))>>2)
#define HAL_PPM_US_TO_TICKCOUNT(_us) (((_us<<3)+(_us<<2)+(_us))>>2)


#define HAL_PPM_UPDATE_CCVALUE(val) SET_WORD_LO_FIRST(T1CC0H, T1CC0L, val)
#define HAL_PPM_ISR_DISABLE() { cli(); }
#define HAL_PPM_ISR_ENABLE()  { sei(); }

#endif // __HAL_PPM_H__
