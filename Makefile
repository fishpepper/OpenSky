#select target. supported: {VD5M, D4RII, USKY, TINYFISH, AFRX, RASP}
TARGET ?= D4RII

ASFLAGS       = -g
ROOT         := $(patsubst %/,%,$(dir $(lastword $(MAKEFILE_LIST))))
SRC_DIR       = $(ROOT)/src
INCLUDE_DIRS  = $(SRC_DIR)

GENERIC_SRCS    = $(wildcard $(SRC_DIR)/*.c)
GENERIC_HEADERS = $(GENERIC_SRCS:.c=.h)



#a special file can trigger the use of a fixed id (see storage.c)
#i use this during development to avoid uneccessary re-binding for vd5m targets
ifneq ($(wildcard .use_fixed_id),)
CFLAGS       += -DFRSKY_USE_FIXED_ID
endif

OBJECT_DIR      := $(ROOT)/obj
TARGET_LC       := $(shell echo $(TARGET) | tr '[:upper:]' '[:lower:]')
TARGET_DIR      := $(ROOT)/board/$(TARGET_LC)
TARGET_MAKEFILE := $(TARGET_DIR)/target.mk
CC251X_BL_DIR   := arch/cc251x/bootloader

RESULT          ?= opensky_$(notdir $(TARGET_LC))

## V                 : Set verbosity level based on the V= parameter
##                     V=0 Low
##                     V=1 High
export AT := @

ifndef V
export V0    :=
export V1    := $(AT)
export STDOUT   :=
else ifeq ($(V), 0)
export V0    := $(AT)
export V1    := $(AT)
export STDOUT:= "> /dev/null"
export MAKE  := $(MAKE) --no-print-directory
else ifeq ($(V), 1)
export V0    :=
export V1    :=
export STDOUT   :=
endif

all  : git_submodule_init ack_disclaimer board

# verifying that the git submodule for the cc2510 bootloader was initialized
git_submodule_init : 
	@if [ ! -f $(CC251X_BL_DIR)/Makefile ]; then git submodule init $(CC251X_BL_DIR) && git submodule update $(CC251X_BL_DIR); fi

ifneq ($(wildcard $(TARGET_MAKEFILE)),)
  #fine, target exists
  include $(TARGET_MAKEFILE)
  include $(ARCH_MAKEFILE)
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

all  : stylecheck board

stylecheck: $(GENERIC_SRCS) $(GENERIC_HEADERS) $(ARCH_SRCS) $(ARCH_HEADERS)
	./stylecheck/cpplint.py \
		--filter=-build/include,-build/storage_class,-readability/casting,-runtime/arrays \
		--extensions="h,c" --root=src --linelength=100 $(GENERIC_HEADERS) $(GENERIC_SRCS) || true
	./stylecheck/cpplint.py \
		--filter=-build/include,-build/storage_class,-readability/casting,-runtime/arrays \
		--extensions="h,c" --root=$(ARCH_DIR) --linelength=100 $(ARCH_HEADERS) $(ARCH_SRCS) || true


git_version:
	git log -n 1 --format=format:"#define GIT_COMMIT \"%h\"%n" HEAD > $@.h

.PHONY: git_version ack_disclaimer git_submodule_init stylecheck
