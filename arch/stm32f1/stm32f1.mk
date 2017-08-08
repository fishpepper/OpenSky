# object files
STARTUP_SRCS := startup_stm32f10x.c \
                system_stm32f10x.c

DRIVER_SRCS  := stm32f10x_rcc.c \
                stm32f10x_gpio.c \
                stm32f10x_usart.c \
                misc.c \
                stm32f10x_iwdg.c \
                stm32f10x_spi.c \
                stm32f10x_dma.c \
                stm32f10x_adc.c \
                stm32f10x_i2c.c \
                stm32f10x_tim.c

HAL_SRCS     := hal_led.c \
                hal_uart.c \
                hal_timeout.c \
                hal_wdt.c \
                hal_delay.c \
                hal_clocksource.c \
                hal_spi.c \
                hal_cc25xx.c \
                hal_io.c \
                hal_adc.c \
                hal_storage.c \
                hal_sbus.c \
                hal_ppm.c \
                hal_soft_serial.c \
                hal_debug.c


# Code Paths
DEVICE_DIR   := $(ARCH_DIR)/device
CORE_DIR     := $(ARCH_DIR)/core
LINK_DIR     := $(ARCH_DIR)/linker
PERIPH_DIR   := $(ARCH_DIR)/peripheral_lib

ARCH_SRCS    := $(addprefix $(ARCH_DIR)/, $(HAL_SRCS)) 

PERIPH_SRCS  := $(addprefix $(PERIPH_DIR)/src/, $(DRIVER_SRCS)) \
                $(addprefix $(DEVICE_DIR)/, $(STARTUP_SRCS))

BOARD_SRCS   := $(ARCH_SRCS) \
                $(GENERIC_SRCS)

ALL_SRCS     := $(BOARD_SRCS) \
		$(PERIPH_SRCS)

#crystal frequency
CRYSTAL_FREQ = 12000000

# fetch this dir during include
SELF_DIR   := $(dir $(lastword $(MAKEFILE_LIST)))

#faster build
MAKEFLAGS  +="-j6 "

#opt
CFLAGS     += -O1 -g

# Tool path, only override if not set
TOOLROOT   ?= 

ARM-PREFIX := arm-none-eabi-
ifneq ($(TOOLROOT),)
ARM-PREFIX := $(TOOLROOT)/$(ARM-PREFIX)
endif

# Tools
CC       := $(ARM-PREFIX)gcc
LD       := $(ARM-PREFIX)gcc
AR       := $(ARM-PREFIX)ar
AS       := $(ARM-PREFIX)as
OBJ      := $(ARM-PREFIX)objcopy
SIZE     := $(ARM-PREFIX)size

INCLUDE_DIRS := $(INCLUDE_DIRS) \
                $(TARGET_DIR) \
                $(CORE_DIR) \
                $(DEVICE_DIR) \
                $(PERIPH_DIR)/inc \
                $(ARCH_DIR)

ARCH_HEADERS := $(ARCH_SRCS:.c=.h)


# Search path for standard files
#vpath %.c ./src
#vpath %.c $(ARCH_DIR)

# Search path for perpheral library
#vpath %.c $(CORE)
#vpath %.c $(PERIPH)/src
#vpath %.c $(DEVICE)

# Processor specific
PTYPE      = STM32F10X_MD
LDSCRIPT   = $(LINK_DIR)/stm32f103c8.ld

# Compilation Flags

FULLASSERT = -DUSE_FULL_ASSERT

LDFLAGS   += -T$(LDSCRIPT) \
             -mthumb \
             -mcpu=cortex-m3

CFLAGS    += -mcpu=cortex-m3 \
             -mthumb \
             -DHSE_VALUE=$(CRYSTAL_FREQ) \
             $(addprefix -I,$(INCLUDE_DIRS)) \
             -D$(PTYPE) \
             -DUSE_STDPERIPH_DRIVER \
             $(FULLASSERT) \
             -DBUILD_TARGET=$(TARGET_LC)

OPENOCD_PIDFILE = /tmp/openocd_opensky.pid

TARGET_OBJS     = $(addsuffix .o,$(addprefix $(OBJECT_DIR)/$(TARGET)/,$(basename $(ALL_SRCS))))
TARGET_DEPS     = $(addsuffix .d,$(addprefix $(OBJECT_DIR)/$(TARGET)/,$(basename $(ALL_SRCS))))
TARGET_ELF      = $(OBJECT_DIR)/$(RESULT).elf
TARGET_BIN      = $(OBJECT_DIR)/$(RESULT).bin
TARGET_HEX      = $(OBJECT_DIR)/$(RESULT).hex

# Build executable
board: $(TARGET_HEX) 

$(TARGET_ELF): $(TARGET_OBJS)
	$(V1) exit
	$(V1) echo Linking: $(TARGET)
	$(V1) $(LD) -o $@ $^ $(LDFLAGS)
	$(V0) $(SIZE) $(TARGET_ELF)

$(TARGET_BIN): $(TARGET_ELF)
	$(V1) $(OBJ) -O binary $< $@

$(TARGET_HEX): $(TARGET_ELF)
	$(V1) $(OBJ) -O ihex $< $@

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
	$(V1) rm -f $(OBJECT_DIR)/$(TARGET)/*.o $(OBJECT_DIR)/$(TARGET)/*.d $(OBJECT_DIR)/*.elf $(OBJECT_DIR)/*.bin

debug: $(TARGET_ELF)
	openocd -f /usr/share/openocd/scripts/interface/stlink-v2.cfg -f /usr/share/openocd/scripts/target/stm32f1x.cfg & echo $$! > $(OPENOCD_PIDFILE)
	sleep 1
	arm-none-eabi-gdb --eval-command="target remote localhost:3333" $(TARGET_ELF)
	if [ -a $(OPENOCD_PIDFILE) ]; then kill `cat $(OPENOCD_PIDFILE)`; fi;

unlock:
	openocd -f /usr/share/openocd/scripts/interface/stlink-v2.cfg -f /usr/share/openocd/scripts/target/stm32f1x.cfg -c "init" -c "reset halt" -c "stm32f1x unlock 0" -c "reset run" -c "exit" 

flash : $(TARGET_BIN)
	if [ -a $(OPENOCD_PIDFILE) ]; then kill `cat $(OPENOCD_PIDFILE)`; fi;
	st-flash write $(TARGET_BIN) 0x8000000

.PHONY: board clean flash debug
