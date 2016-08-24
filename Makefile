#select target. supported: {VD5M, D4RII}
TARGET ?= VD5M
#TARGET ?= D4RII

CFLAGS  = -O1 -g
ASFLAGS = -g
GENERIC_SRCS  = main.c debug.c assert.c clocksource.c timeout.c wdt.c delay.c frsky.c spi.c cc25xx.c 
GENERIC_SRCS += io.c storage.c failsafe.c ppm.c adc.c sbus.c apa102.c soft_spi.c soft_serial.c telemetry.c

ifeq ($(TARGET),D4RII)
  include board/d4rii/Makefile.board
else
  ifeq ($(TARGET),VD5M)
    include board/vd5m/Makefile.board
  else
    @echo "ERROR, UNSUPPORTED TARGET"
    @exit 0
  endif
endif

all  : board
