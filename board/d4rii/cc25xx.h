#ifndef __CC25XX_H__
#define __CC25XX_H__

#include "hal_cc25xx.h"

void cc25xx_init(void);
#define cc25xx_set_register(reg, val) hal_cc25xx_set_register(reg, val);
#define cc25xx_strobe(val) hal_cc25xx_strobe(val);


#endif // __CC25XX_H__
