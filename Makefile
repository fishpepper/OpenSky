#select target. supported: {VD5M, D4RII, USKY, TINYFISH, RASP}
TARGET ?= USKY
#TARGET ?= VD5M
#TARGET ?= D4RII

ASFLAGS = -g
GENERIC_SRCS  = main.c debug.c assert.c clocksource.c timeout.c wdt.c delay.c frsky.c spi.c cc25xx.c 
GENERIC_SRCS += io.c storage.c failsafe.c ppm.c adc.c sbus.c apa102.c soft_spi.c soft_serial.c telemetry.c

#a special file can trigger the use of a fixed id (see storage.c)
#i use this during development to avoid uneccessary re-binding for vd5m targets
ifneq ($(wildcard .use_fixed_id),)
CFLAGS += -DFRSKY_USE_FIXED_ID
endif

TARGET_LC = $(shell echo $(TARGET) | tr '[:upper:]' '[:lower:]')
TARGET_MAKEFILE = board/$(TARGET_LC)/Makefile.board

ifneq ($(wildcard $(TARGET_MAKEFILE)),)
  #fine, target exists
  include  $(TARGET_MAKEFILE)
else
  #does not exist
  $(error UNSUPPORTED Target ($(TARGET)) given. could not find makefile at $(TARGET_MAKEFILE). aborting)
endif

all  : board

git_version:
	git log -n 1 --format=format:"#define GIT_COMMIT \"%h\"%n" HEAD > $@.h

.PHONY: git_version
