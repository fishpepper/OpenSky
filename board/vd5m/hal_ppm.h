
#define HAL_PPM_UPDATE_CCVALUE(val) SET_WORD_LO_FIRST(T1CC0H, T1CC0L, val)

#define HAL_PPM_ISR_DISABLE() { cli(); }
#define HAL_PPM_ISR_ENABLE()  { sei(); }

