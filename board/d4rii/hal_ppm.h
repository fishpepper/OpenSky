#ifndef __HAL_PPM_H__
#define __HAL_PPM_H__

#include "config.h"
#include "stm32f10x_tim.h"

void hal_ppm_init(void);

void hal_ppm_failsafe_exit(void);
void hal_ppm_failsafe_enter(void);

static void hal_ppm_init_rcc(void);
static void hal_ppm_init_gpio(void);
static void hal_ppm_init_timer(void);
static void hal_ppm_init_nvic(void);
static void hal_ppm_init_ocx(uint8_t ch, TIM_TypeDef *TIMx, TIM_OCInitTypeDef *tim_oc_init);

//counter runs with 2MHz = 0.5us resolution
#define HAL_PPM_US_TO_TICKCOUNT(us) (us * 2)


#define HAL_PPM_UPDATE_CCVALUE(x) { PPM_TIMER->ARR = x; }
#define HAL_PPM_ISR_DISABLE() { __disable_irq(); }
#define HAL_PPM_ISR_ENABLE()  { __enable_irq(); }

#endif // __HAL_PPM_H__
