#ifndef __HAL_DEFINES_H__
#define __HAL_DEFINES_H__

#define EXTERNAL_MEMORY __xdata
#define EXTERNAL_DATA __data
#define inline

#define sei() { IEN0 |= IEN0_EA; }
#define cli() { IEN0 &= ~IEN0_EA; }

#ifndef NOP
    #define NOP() { __asm nop __endasm; }
#endif
#define NOP45() { NOP(); NOP(); NOP(); NOP(); NOP(); NOP(); NOP(); NOP();

#define HI(a)     (uint8_t) ((uint16_t)(a) >> 8 )
#define LO(a)     (uint8_t)  (uint16_t)(a)
#define SET_WORD(H, L, val) { (H) = HI(val); (L) = LO(val); }
// necessary for timer registers. todo: check if necessary for others as well...
#define SET_WORD_LO_FIRST(H, L, val) {(L) = LO(val); (H) = HI(val);  }

#define UNUSED(x) (void)(x);

#define USART0_P0 0
#define USART0_P1 1
#define USART1_P0 2
#define USART1_P1 3

#endif // __HAL_DEFINES_H__
