
CC       = sdcc

HAL_SRCS     := hal_led.c \
                hal_debug.c \
                hal_uart.c \
                hal_clocksource.c \
                hal_timeout.c \
                hal_wdt.c \
                hal_delay.c \
                hal_dma.c \
                hal_spi.c \
                hal_cc25xx.c \
                hal_io.c \
                hal_adc.c \
                hal_storage.c \
                hal_sbus.c \
                hal_ppm.c \
                hal_soft_serial.c

ARCH_SRCS    := $(addprefix $(ARCH_DIR)/, $(HAL_SRCS))
ARCH_HEADERS := $(ARCH_SRCS:.c=.h)

BOARD_SRCS   := $(GENERIC_SRCS) \
                $(ARCH_SRCS)

INCLUDE_DIRS := $(INCLUDE_DIRS) \
                /usr/share/sdcc/include \
                $(SRC_DIR) \
                $(ARCH_DIR) \
                $(TARGET_DIR)

LDFLAGS_FLASH = --out-fmt-ihx \
                --code-loc 0x0c00 \
                --code-size $(FLASH_SIZE) \
                --xram-loc 0xf000 \
                --xram-size 0x300 \
                --iram-size 0x100

#programmer binary
CC_TOOL    ?= cc-tool

CFLAGS     += --model-small \
              --opt-code-speed \
              $(addprefix -I,$(INCLUDE_DIRS))

ifdef DEBUG
CFLAGS     += --debug
endif

HEADERS := $(BOARD_SRCS:.c=.h)
ADB      = $(BOARD_SRCS:.c=.adb)
ASM      = $(BOARD_SRCS:.c=.asm)
LNK      = $(BOARD_SRCS:.c=.lnk)
LST      = $(BOARD_SRCS:.c=.lst)
REL      = $(BOARD_SRCS:.c=.rel)
RST      = $(BOARD_SRCS:.c=.rst)
SYM      = $(BOARD_SRCS:.c=.sym)

#we build two flavours: 
# _full  : includes the bootloader, use this for initial flashing
# _update: just the opensky fw, relocated to be stored after the bootloader
TARGET_FULL   = $(OBJECT_DIR)/$(RESULT)_full.hex
TARGET_UPDATE = $(OBJECT_DIR)/$(RESULT)_update.hex
TARGET_NO_BL  = $(OBJECT_DIR)/$(RESULT)_no_bl.hex
BL_DIR        = arch/cc251x/bootloader
BL_HEX        = bootloader.hex

PCDB          = $(PROGS:.hex=.cdb)
PLNK          = $(PROGS:.hex=.lnk)
PMAP          = $(PROGS:.hex=.map)
PMEM          = $(PROGS:.hex=.mem)
PAOM          = $(PROGS:.hex=)

SREC_CAT_FOUND := $(shell command -v srec_cat 2> /dev/null)

TARGET_OBJS     = $(addsuffix .rel,$(addprefix $(OBJECT_DIR)/$(TARGET)/,$(basename $(BOARD_SRCS))))
TARGET_DEPS     = $(addsuffix .d,$(addprefix $(OBJECT_DIR)/$(TARGET)/,$(basename $(BOARD_SRCS))))

# Search path for standard files
#vpath %.c ./src
#vpath %.c ./$(ARCH_DIR)

board: $(TARGET_UPDATE) $(TARGET_FULL)

bootloader:
	@echo "### Building bootloader ###"
	$(MAKE) -C $(BL_DIR) \
		STYLECHECK_DISABLED=1 \
		FLASH_SIZE=$(FLASH_SIZE) \
		CONFIG_INCLUDE_DIR=../../../$(TARGET_DIR) \
		clean all

$(TARGET_FULL): $(TARGET_UPDATE) bootloader
	@echo "merging bootloader and main code"
ifndef SREC_CAT_FOUND
	$(error "could not find srec_cat binary. make sure to install the srecord package")
else
	srec_cat -disable_sequence_warnings \
		$(TARGET_UPDATE) -intel \
		$(BL_DIR)/$(BL_HEX) -intel \
		-o $(TARGET_FULL) -intel 
	@echo "done."
endif

$(TARGET_UPDATE): $(TARGET_OBJS)
	$(V1) echo Linking: $(TARGET)
	$(V1) $(CC) $(LDFLAGS_FLASH) $(CFLAGS) -o $@ $^

# this is just for development, DO NOT flash this for production
$(TARGET_NO_BL): $(TARGET_OBJS)
	$(V1) $(CC) $(LDFLAGS_FLASH) $(CFLAGS) --code-loc 0x000 -o $@ $^

$(OBJECT_DIR)/$(TARGET)/%.rel: %.c
	$(V1) mkdir -p $(dir $@)
	$(V1) echo "%% $(notdir $<)" "$(STDOUT)" && \
	$(CC) -c -o $@ $(CFLAGS) $<

clean:
	$(V1) echo Cleaning: $(TARGET)
	$(V1) rm -f $(ADB) $(ASM) $(LNK) $(LST) $(TARGET_OBJS) $(RST) $(SYM)
	$(V1) rm -f $(PROGS) $(PCDB) $(PLNK) $(PMAP) $(PMEM) $(PAOM)
	$(V1) cd $(BL_DIR) && $(MAKE) clean

flash: $(OUTPUT_FULL)
	$(CC_TOOL) -f -e -w $(TARGET_FULL)

flash_no_bl: $(OUTPUT_NO_BL)
	$(CC_TOOL) -f -e -w $(TARGET_NO_BL)
