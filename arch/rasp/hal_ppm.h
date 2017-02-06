/*
    Copyright 2017 fishpepper <AT> gmail.com

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http:// www.gnu.org/licenses/>.

   author: fishpepper <AT> gmail.com
*/

#ifndef HAL_PPM_H_
#define HAL_PPM_H_

void hal_ppm_init(void);

void hal_ppm_failsafe_exit(void);
void hal_ppm_failsafe_enter(void);
void hal_ppm_tick();
void hal_ppm_update_cvalue(int us);

void hal_ppm_irq_callback(void);

#define HAL_PPM_US_TO_TICKCOUNT(us) ((us * 1)-1)
#define HAL_PPM_FRSKY_TO_TICKCOUNT(_frsky) ((_frsky)*2*2/3)

#define PPM_TIMER_ISR(void) hal_ppm_irq_callback(void)

#define HAL_PPM_UPDATE_CCVALUE(x) hal_ppm_update_cvalue(x)
#define HAL_PPM_ISR_DISABLE() { }
#define HAL_PPM_ISR_ENABLE()  { }
#define HAL_PPM_ISR_FLAG_SET() (1)
#define HAL_PPM_ISR_CLEAR_FLAG() {}

#endif  // HAL_PPM_H_
