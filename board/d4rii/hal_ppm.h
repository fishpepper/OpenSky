#ifndef __HAL_PPM_H__
#define __HAL_PPM_H__

#include "pin_config.h"
#include "stm32f10x_tim.h"

void hal_ppm_init(void);

void hal_ppm_failsafe_exit(void);
void hal_ppm_failsafe_enter(void);

static void hal_ppm_init_rcc(void);
static void hal_ppm_init_gpio(void);
static void hal_ppm_init_timer(void);
static void hal_sbus_init_nvic(void);

//counter runs with 2MHz = 0.5us resolution
#define HAL_PPM_US_TO_TICKCOUNT(us) (us * 2)


#define HAL_PPM_UPDATE_CCVALUE(x) { PPM_TIMER->ARR = x; }
#define HAL_PPM_ISR_DISABLE() { TIM_ITConfig(PPM_TIMER, TIM_IT_Update, DISABLE); }
#define HAL_PPM_ISR_ENABLE() { TIM_ITConfig(PPM_TIMER, TIM_IT_Update, ENABLE); }

#endif // __HAL_PPM_H__
