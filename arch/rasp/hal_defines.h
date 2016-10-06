#ifndef __HAL_DEFINES_H__
#define __HAL_DEFINES_H__

#define EXTERNAL_MEMORY
#define EXTERNAL_DATA

#define HI(a)     (uint8_t) ((uint16_t)(a) >> 8 )
#define LO(a)     (uint8_t)  (uint16_t)(a)
#define SET_WORD(H, L, val) { (H) = HI(val); (L) = LO(val); }
//necessary for timer registers. todo: check if necessary for others as well...
#define SET_WORD_LO_FIRST(H, L, val) {(L) = LO(val); (H) = HI(val);  }

#define UNUSED(x) (void)(x);

#endif // __HAL_DEFINES_H__
