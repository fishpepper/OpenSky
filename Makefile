#select target. supported: {VD5M, D4RII}
TARGET ?= D4RII

CFLAGS  = -O1 -g
ASFLAGS = -g
GENERIC_SRCS    =  main.c assert.c uart.c clocksource.c timeout.c wdt.c delay.c dma.c frsky.c spi.c cc25xx.c io.c storage.c failsafe.c ppm.c adc.c sbus.c

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

all: board

