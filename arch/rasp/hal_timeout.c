/*
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

#include "hal_timeout.h"
#include "debug.h"
#include "wdt.h"

#include <time.h>
#include <string.h>
#include <errno.h>

struct timespec timer_1;
struct timespec timer_2;
struct timespec timer_ppm;

void hal_timeout_init(void) {
    memset(&timer_1, 0, sizeof(timer_1));
    memset(&timer_2, 0, sizeof(timer_2));
    clock_gettime(CLOCK_REALTIME, &timer_ppm);
}

/* Returns true if a is greather then b */
static inline uint8_t timeval_gt(const struct timespec *a, const struct timespec *b) {
    if (a->tv_sec > b->tv_sec)
        return 1;
    if (a->tv_sec == b->tv_sec && a->tv_nsec > b->tv_nsec)
        return 1;
    return 0;
}

static inline void timer_add(struct timespec *timer, int us) {
    timer->tv_nsec += us * 1000;
    if (timer->tv_nsec > 1000000000) {
        timer->tv_nsec -= 1000000000;
        timer->tv_sec += 1;
    }
}

/* Subtract the ‘struct timeval’ values X and Y,
   storing the result in RESULT.
   Return 1 if the difference is negative, otherwise 0. */
static inline int timeval_subtract(struct timespec *result, struct timespec *x, struct timespec *y)
{
    /* Perform the carry for the later subtraction by updating y. */
    if (x->tv_nsec < y->tv_nsec) {
        int nsec = (y->tv_nsec - x->tv_nsec) / 1000000000 + 1;
        y->tv_nsec -= 1000000000 * nsec;
        y->tv_sec += nsec;
    }
    if (x->tv_nsec - y->tv_nsec > 1000000000) {
        int nsec = (x->tv_nsec - y->tv_nsec) / 1000000000;
        y->tv_nsec += 1000000000 * nsec;
        y->tv_sec -= nsec;
    }

    /* Compute the time remaining to wait.
       tv_nsec is certainly positive. */
    result->tv_sec = x->tv_sec - y->tv_sec;
    result->tv_nsec = x->tv_nsec - y->tv_nsec;

    /* Return 1 if result is negative. */
    return x->tv_sec < y->tv_sec;
}

void hal_timeout_set_100us(uint32_t hus) {
    clock_gettime(CLOCK_REALTIME, &timer_1);
    timer_add(&timer_1, hus * 100);
}

void hal_timeout2_set_100us(uint32_t hus) {
    clock_gettime(CLOCK_REALTIME, &timer_2);
    timer_add(&timer_2, hus * 100);
}

void hal_timeout_set(uint32_t ms) {
    clock_gettime(CLOCK_REALTIME, &timer_1);
    timer_add(&timer_1, ms * 1000);
}


uint8_t hal_timeout_timed_out(void) {
    struct timespec now;
    clock_gettime(CLOCK_REALTIME, &now);

    if (timeval_gt(&now, &timer_ppm)) {
        hal_ppm_tick();
    }

    return timeval_gt(&now, &timer_1);
}

uint8_t hal_timeout2_timed_out(void) {
    struct timespec now;
    clock_gettime(CLOCK_REALTIME, &now);

    if (timeval_gt(&now, &timer_ppm)) {
        hal_ppm_tick();
    }
    return timeval_gt(&now, &timer_2);
}

void hal_timeout_delay_us(int32_t timeout_us) {
    struct timespec now, sleep, delay_timer, rem;
    clock_gettime(CLOCK_REALTIME, &now);
    memcpy(&delay_timer, &now, sizeof(struct timespec));
    timer_add(&delay_timer, timeout_us);

    do {

        /* If ppm happens before delay timer times out. (delay_timer gt timer_ppm) */
        if (timeval_gt(&delay_timer, &timer_ppm)) {

            /* Calculate how far to sleep */
            if (timeval_subtract(&sleep, &now, &timer_ppm) > 0) {
                nanosleep(&sleep, &rem);
            }

            /* Get current time again */
            clock_gettime(CLOCK_REALTIME, &now);
            if (timeval_gt(&now, &timer_ppm)) {
                hal_ppm_tick();
            }

            /* Loop, as a new ppm timer might be scheduled again before the delay ends */
            continue;
        }

        /* Calculate how long to sleep */
        if (timeval_subtract(&sleep, &now, &delay_timer) > 0) {
            nanosleep(&sleep, &rem);
        }
    } while(0);
}

void hal_timeout_add_ppm(uint32_t us) {
    timer_add(&timer_ppm, us);
}


