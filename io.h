#ifndef __IO_H__
#define __IO_H__
#include "hal_io.h"

void io_init(void);
#define io_bind_request() hal_io_bind_request()

#endif // __IO_H__
