# object files
DRIVER_SRCS   = 
HAL_SRCS     := hal_adc.c \
                hal_cc25xx.c \
                hal_io.c \
                hal_sbus.c \
                hal_soft_serial.c \
                hal_spi.c \
                hal_storage.c \
                hal_timeout.c \
                hal_uart.c \
                hal_wdt.c \
                hal_ppm.c \
                hal_debug.c

ARCH_DIR = arch/rasp
ARCH_SRCS    := $(addprefix $(ARCH_DIR)/, $(HAL_SRCS))
ARCH_HEADERS := $(ARCH_SRCS:.c=.h)

BOARD_SRCS   := $(ARCH_SRCS) \
                $(GENERIC_SRCS)

# fetch this dir during include
SELF_DIR := $(dir $(lastword $(MAKEFILE_LIST)))

#name of executable
RESULT ?= opensky_$(notdir $(TARGET_LC))

#faster build
MAKEFLAGS+="-j1 "

#opt
CFLAGS += -O1 -g

# Tool path, only override if not set
TOOLROOT ?= /usr/bin

# Tools (note: for cross compiling we use gcc as linker!)
CC   = gcc
LD   = gcc
AR   = ar
AS   = gcc
OBJ  = objcopy

# Search path for standard files
vpath %.c $(SRC_DIR)
vpath %.c $(ARCH_DIR)

# Search path for perpheral library
vpath %.c $(CORE)
vpath %.c $(PERIPH)/src
vpath %.c $(DEVICE)

# Processor specific
PTYPE = RASP

# Compilation Flags

FULLASSERT = -DUSE_FULL_ASSERT

INCLUDE_DIRS := $(INCLUDE_DIRS) \
                $(SELF_DIR) \
                $(SRC_DIR) \
                $(DEVICE) \
                $(CORE) \
                $(PERIPH)/inc \
                $(ARCH_DIR)

CFLAGS  += $(addprefix -I,$(INCLUDE_DIRS)) \
           -D$(PTYPE) \
           -DUSE_STDPERIPH_DRIVER \
           $(FULLASSERT) \
           -DBUILD_TARGET=$(TARGET_LC)

TARGET_OBJS     = $(addsuffix .o,$(addprefix $(OBJECT_DIR)/$(TARGET)/,$(basename $(BOARD_SRCS))))
TARGET_DEPS     = $(addsuffix .d,$(addprefix $(OBJECT_DIR)/$(TARGET)/,$(basename $(BOARD_SRCS))))

# Build executable
board: $(OBJECT_DIR)/$(RESULT)

$(OBJECT_DIR)/$(RESULT): $(TARGET_OBJS)
	$(V1) exit
	$(V1) echo Linking: $(TARGET)
	$(V1) $(LD) -o $@ $^ $(LDFLAGS)

# compile and generate dependency info
$(OBJECT_DIR)/$(TARGET)/%.o: %.c 
	$(V1) mkdir -p $(dir $@)
	$(V1) echo "%% $(notdir $<)" "$(STDOUT)" && \
	$(CC) -c -o $@ $(CFLAGS) $<

$(OBJECT_DIR)/$(TARGET)/%.o: %.s
	$(V1) mkdir -p $(dir $@)
	$(V1) echo "%% $(notdir $<)" "$(STDOUT)"
	$(V1) $(CC) -c $(CFLAGS) $(DEPFLAGS) $< -o $@

clean:
	$(V1) echo Cleaning: $(TARGET)
	$(V1) rm -f $(OBJECT_DIR)/$(TARGET)/*.o $(OBJECT_DIR)/$(TARGET)/*.d

.PHONY: board clean flash debug
