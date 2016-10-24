#ifndef __HAL_PPM_H__
#define __HAL_PPM_H__


void hal_ppm_init(void);

void hal_ppm_failsafe_exit(void);
void hal_ppm_failsafe_enter(void);
void hal_ppm_tick();
void hal_ppm_update_cvalue(int us);

#define HAL_PPM_US_TO_TICKCOUNT(us) ((us * 1)-1)
#define HAL_PPM_FRSKY_TO_TICKCOUNT(_frsky) ((_frsky)*2*2/3)

#define PPM_TIMER_ISR(void) hal_ppm_irq_callback(void)

#define HAL_PPM_UPDATE_CCVALUE(x) hal_ppm_update_cvalue(x)
#define HAL_PPM_ISR_DISABLE() { }
#define HAL_PPM_ISR_ENABLE()  { }
#define HAL_PPM_ISR_FLAG_SET() (1)
#define HAL_PPM_ISR_CLEAR_FLAG() {}

#endif // __HAL_PPM_H__
