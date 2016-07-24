#ifndef __TIMEOUT_H__
#define __TIMEOUT_H__

void timeout_init(void);
#define timeout_set(x) hal_timeout_set(x)
#define timeout_timed_out() hal_timeout_timed_out()

#endif
