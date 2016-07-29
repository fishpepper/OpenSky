#select target. supported: {VD5M, D4RII}
TARGET ?= D4RII

CFLAGS  = -O1 -g
ASFLAGS = -g
GENERIC_SRCS    =  main.o assert.o uart.o clocksource.o timeout.o wdt.o delay.o dma.o frsky.o spi.o cc25xx.o io.o storage.o failsafe.o ppm.o adc.o

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

