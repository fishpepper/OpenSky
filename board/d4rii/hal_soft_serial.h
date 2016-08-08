#ifndef __HAL_SOFT_SERIAL_H__
#define __HAL_SOFT_SERIAL_H__
#include "soft_serial.h"
#include "config.h"
//#include "stm32f10x_tim.h"
#include "stm32f10x_gpio.h"


// at 9600 baud a bit duration is 1/9600s = 104.166667us
// the counter counts in 1/24th of us -> 104.1667us * 24 =
#define HAL_SOFTSERIAL_BIT_DURATION_TICKS  2500

void hal_soft_serial_init(void);

static void hal_soft_serial_init_rcc(void);
static void hal_soft_serial_init_gpio(void);
static void hal_soft_serial_init_timer(void);
static void hal_soft_serial_init_nvic(void);

#define HAL_SOFT_SERIAL_PIN_LO() (GPIO_ReadInputDataBit(SOFT_SERIAL_GPIO, SOFT_SERIAL_PIN) == 0)
#define HAL_SOFT_SERIAL_PIN_HI() (! HAL_SOFT_SERIAL_PIN_LO())


#define HAL_SOFT_SERIAL_IC_ISR_DISABLE()    { TIM_ITConfig(SOFT_SERIAL_TIMER, SOFT_SERIAL_TIMER_IT_IC, DISABLE); }
#define HAL_SOFT_SERIAL_IC_ISR_ENABLE()     { TIM_ITConfig(SOFT_SERIAL_TIMER, SOFT_SERIAL_TIMER_IT_IC, ENABLE); }
#define HAL_SOFT_SERIAL_IC_ISR_FLAG_SET()   (TIM_GetITStatus(SOFT_SERIAL_TIMER, SOFT_SERIAL_TIMER_IT_IC) != RESET)
#define HAL_SOFT_SERIAL_IC_ISR_FLAG_CLEAR() { TIM_ClearITPendingBit(SOFT_SERIAL_TIMER, SOFT_SERIAL_TIMER_IT_IC); }

#define HAL_SOFT_SERIAL_UP_ISR_DISABLE()    { TIM_ITConfig(SOFT_SERIAL_TIMER, SOFT_SERIAL_TIMER_IT_UP, DISABLE); }
#define HAL_SOFT_SERIAL_UP_ISR_ENABLE()     { TIM_ITConfig(SOFT_SERIAL_TIMER, SOFT_SERIAL_TIMER_IT_UP, ENABLE); }
#define HAL_SOFT_SERIAL_UP_ISR_FLAG_SET()   (TIM_GetITStatus(SOFT_SERIAL_TIMER, SOFT_SERIAL_TIMER_IT_UP) != RESET)
#define HAL_SOFT_SERIAL_UP_ISR_FLAG_CLEAR() { TIM_ClearITPendingBit(SOFT_SERIAL_TIMER, SOFT_SERIAL_TIMER_IT_UP); }

#define HAL_SOFT_SERIAL_UPDATE_TOP_VALUE(x)   { SOFT_SERIAL_TIMER->ARR = x; }

void SOFT_SERIAL_TIMER_IC_IRQHandler(void);
void SOFT_SERIAL_TIMER_UP_IRQHandler(void);
#endif // __HAL_SOFT_SERIAL_H__
