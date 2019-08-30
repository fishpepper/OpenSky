# object files
HAL_SRCS      = \
								hal_uart.c \
								hal_timeout.c \
								hal_wdt.c \
								hal_clocksource.c \
								hal_spi.c \
								hal_cc25xx.c \
								hal_io.c \
								hal_adc.c \
								hal_storage.c \
								hal_ppm.c \
								hal_soft_spi.c \
								hal_soft_serial.c

ALL_SRCS    = $(HAL_SRCS) $(GENERIC_SRCS)

# fetch this dir during include
SELF_DIR := $(dir $(lastword $(MAKEFILE_LIST)))

# include path
CFLAGS += -I$(SELF_DIR)

# Tool path, only override if not set
TOOLROOT ?= /Applications/Arduino.app/Contents/Java/hardware/tools/avr/bin

# Tools
CC=$(TOOLROOT)/avr-gcc
LD=$(TOOLROOT)/avr-gcc
AR=$(TOOLROOT)/avr-gcc-ar
AS=$(TOOLROOT)/avr-as
OBJ=$(TOOLROOT)/avr-objcopy
SIZE=$(TOOLROOT)/avr-size
AVRDUDE=$(TOOLROOT)/avrdude

# Code Paths
ARCH_DIR=arch/atmega328

INCLUDE_DIRS := $(INCLUDE_DIRS) \
                $(SRC_DIR) \
                $(ARCH_DIR) \
                $(TARGET_DIR)

# Search path for standard files
vpath %.c ./
vpath %.c $(ARCH_DIR)

LDFLAGS+= $(BOARD_FLAGS) -Wl,--gc-sections -Os -flto -fuse-linker-plugin
CFLAGS+= $(BOARD_FLAGS) -std=gnu11 -Os -flto -fno-fat-lto-objects
CFLAGS+= -I. -I$(ARCH_DIR)
CFLAGS+= -DBUILD_TARGET=$(TARGET)
CFLAGS+= $(addprefix -I,$(INCLUDE_DIRS))

TARGET_OBJS     = $(addsuffix .o,$(addprefix $(OBJECT_DIR)/$(TARGET)/,$(basename $(ALL_SRCS))))
TARGET_DEPS     = $(addsuffix .d,$(addprefix $(OBJECT_DIR)/$(TARGET)/,$(basename $(ALL_SRCS))))
TARGET_ELF      = $(OBJECT_DIR)/$(RESULT).elf
TARGET_HEX      = $(OBJECT_DIR)/$(RESULT).hex
TARGET_EEP      = $(OBJECT_DIR)/$(RESULT).eep
TARGET_LST      = $(OBJECT_DIR)/$(RESULT).lst

# Build executable
board: $(TARGET_HEX)

$(TARGET_ELF): $(TARGET_OBJS)
	$(V1) exit
	$(V1) echo Linking: $(TARGET)
	$(V1) $(LD) -o $@ $^ $(LDFLAGS)
	$(V0) $(SIZE) $(TARGET_ELF)
	$(OBJ) -j .eeprom --set-section-flags=.eeprom='alloc,load' \
		--no-change-warnings --change-section-lma .eeprom=0 -O ihex $(TARGET_ELF) $(TARGET_EEP)
	$(SIZE) --mcu=$(BOARD_NAME) -C --format=avr $(TARGET_ELF)

$(TARGET_HEX): $(TARGET_ELF)
	$(OBJ) -O ihex -R .eeprom $(TARGET_ELF) $(TARGET_HEX)

$(TARGET_LST): $(TARGET_ELF)
	$(DUMP) -h -S $(TARGET_ELF) > $(TARGET_LST)

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
	$(V1) rm -f $(OBJECT_DIR)/$(TARGET)/src/*.o $(OBJECT_DIR)/$(TARGET)/src/*.d $(OBJECT_DIR)/$(TARGET)/*.o $(OBJECT_DIR)/$(TARGET)/*.d $(OBJECT_DIR)/*.elf $(OBJECT_DIR)/*.bin $(OBJECT_DIR)/*.hex $(OBJECT_DIR)/*.eep

flash: $(TARGET_HEX)
	$(AVRDUDE) -q -V -p $(BOARD_NAME) \
		-C $(TOOLROOT)/../etc/avrdude.conf \
		-D -c arduino -b $(SERIAL_SPEED) -P $(SERIAL_PORT) \
		-U flash:w:$^:i

.PHONY: board clean flash debug


