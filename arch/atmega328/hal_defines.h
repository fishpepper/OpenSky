#ifndef __HAL_DEFINES_H__
#define __HAL_DEFINES_H__

#define EXTERNAL_MEMORY
#define EXTERNAL_DATA

#define LO(w) (w & 0xFF)
#define HI(w) ((w>>8) & 0xFF)
#define __IO

#endif // __HAL_DEFINES_H__
