#select target. supported: {VD5M, D4RII, USKY, TINYFISH, AFF4RX, RASP}
TARGET ?= USKY

ASFLAGS = -g
GENERIC_SRCS  = main.c debug.c assert.c clocksource.c timeout.c wdt.c delay.c frsky.c spi.c cc25xx.c uart.c
GENERIC_SRCS += io.c storage.c failsafe.c ppm.c adc.c sbus.c apa102.c soft_spi.c soft_serial.c telemetry.c

#a special file can trigger the use of a fixed id (see storage.c)
#i use this during development to avoid uneccessary re-binding for vd5m targets
ifneq ($(wildcard .use_fixed_id),)
CFLAGS += -DFRSKY_USE_FIXED_ID
endif

TARGET_LC       = $(shell echo $(TARGET) | tr '[:upper:]' '[:lower:]')
TARGET_DIR      = $(abspath board/$(TARGET_LC))
TARGET_MAKEFILE = $(TARGET_DIR)/Makefile.board
CC251X_BL_DIR   = arch/cc251x/bootloader

RESULT ?= opensky_$(notdir $(TARGET_LC))

all  : git_submodule_init ack_disclaimer board

# verifying that the git submodule for the cc2510 bootloader was initialized
git_submodule_init : 
	@if [ ! -f $(CC251X_BL_DIR)/Makefile ]; then git submodule init $(CC251X_BL_DIR) && git submodule update $(CC251X_BL_DIR); fi

ifneq ($(wildcard $(TARGET_MAKEFILE)),)
  #fine, target exists
  include  $(TARGET_MAKEFILE)
else
  #does not exist
  $(error UNSUPPORTED Target ($(TARGET)) given. could not find makefile at $(TARGET_MAKEFILE). aborting)
endif

ack_disclaimer :
ifeq ($(wildcard .i_know_what_i_am_doing),)
	@echo "###############################################"
	@echo "# WARNING:                                    #"
	@echo "#=============================================#"
	@echo "# THIS SOFTWARE IS FOR EDUCATIONAL USE ONLY   #"
	@echo "#                                             #"
	@echo "# BAD things could happen if you use this     #"
	@echo "# code to control real planes/quadrocopters!  #"
	@echo "#                                             #"
	@echo "# It is meant to be used on small indoor toys #"
	@echo "#                                             #"
	@echo "# Using this code will probably void the FCC  #"
	@echo "# compliance of your RX and might void        #"
	@echo "# transmission laws depending of your country #"
	@echo "#                                             #"
	@echo "#   I AM NOT RESPONSIBLE FOR ANY DAMAGE or    #"
	@echo "#     INJURIES CAUSED BY USING THIS CODE!     #"
	@echo "###############################################"
	@echo ""

	@while [ -z "$$CONTINUE" ]; do \
	    read -r -p "Do you accept the disclaimer? [y/N] : " CONTINUE; \
	done ; \
	[ $$CONTINUE = "y" ] || [ $$CONTINUE = "Y" ] || (echo "\ndisclaimer not accepted. will exit now."; echo ""; exit 1;)

	@echo "fine. you know what you are doing. will build now"
	@touch .i_know_what_i_am_doing
endif

all  : board

git_version:
	git log -n 1 --format=format:"#define GIT_COMMIT \"%h\"%n" HEAD > $@.h

.PHONY: git_version ack_disclaimer git_submodule_init 
