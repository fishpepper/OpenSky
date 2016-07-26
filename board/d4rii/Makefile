# compilation flags for gdb
CFLAGS  = -O1 -g
ASFLAGS = -g

# object files
DRIVER   = stm32f10x_rcc.o stm32f10x_gpio.o stm32f10x_usart.o misc.o stm32f10x_wwdg.o  stm32f10x_spi.o
HAL_OBJS = hal_led.o hal_uart.o hal_clocksource.o hal_timeout.o hal_wdt.o hal_delay.o hal_dma.o hal_spi.o hal_cc25xx.o hal_io.o
OBJS     =  $(STARTUP) $(DRIVER) $(HAL_OBJS) main.o assert.o uart.o clocksource.o timeout.o wdt.o delay.o dma.o frsky.o spi.o cc25xx.o io.o storage.o failsafe.o ppm.o

# name of executable
RESULT ?= opensky_$(notdir $(CURDIR))

#crystal frequency
CRYSTAL_FREQ = 12000000

# Tool path
ifeq ($(TOOLROOT),)
	TOOLROOT=/usr/bin
endif

# Library path
LIBROOT=../../driver/stm32f10x

# Tools
CC=$(TOOLROOT)/arm-none-eabi-gcc
LD=$(TOOLROOT)/arm-none-eabi-gcc
AR=$(TOOLROOT)/arm-none-eabi-ar
AS=$(TOOLROOT)/arm-none-eabi-as
OBJ=$(TOOLROOT)/arm-none-eabi-objcopy

# Code Paths
DEVICE=$(LIBROOT)/device
CORE=$(LIBROOT)/core
LINK=$(LIBROOT)/linker
PERIPH=$(LIBROOT)/peripheral_lib

# Search path for standard files

vpath %.c ./

# Search path for perpheral library
vpath %.c $(CORE)
vpath %.c $(PERIPH)/src
vpath %.c $(DEVICE)

# Search path for Library
vpath %.c $(TEMPLATEROOT)/Library/ff9/src
vpath %.c $(TEMPLATEROOT)/Library/ff9/src/option
vpath %.c $(TEMPLATEROOT)/Library

#  Processor specific
PTYPE = STM32F10X_MD_VL 
LDSCRIPT = $(LINK)/stm32f103c8.ld
STARTUP = startup_stm32f10x.o system_stm32f10x.o 

# Compilation Flags

FULLASSERT = -DUSE_FULL_ASSERT 

LDFLAGS+= -T$(LDSCRIPT) -mthumb -mcpu=cortex-m3 
CFLAGS+= -mcpu=cortex-m3 -mthumb 
CFLAGS+= -DHSE_VALUE=$(CRYSTAL_FREQ)
CFLAGS+= -I$(DEVICE) -I$(CORE) -I$(PERIPH)/inc -I.
CFLAGS+= -D$(PTYPE) -DUSE_STDPERIPH_DRIVER $(FULLASSERT)
#CFLAGS+= -I$(TEMPLATEROOT)/Library/ff9/src -I$(TEMPLATEROOT)/Library

OPENOCD_PIDFILE = /tmp/openocd_opensky.pid

# Build executable 

$(RESULT).elf : $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $(OBJS) $(LDLIBS)
	
%.bin: %.elf
	$(OBJ) -O binary $< $@

# compile and generate dependency info

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@
	$(CC) -MM $(CFLAGS) $< > $*.d

%.o: %.s
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm -f $(OBJS) $(OBJS:.o=.d) $(RESULT).* $(CLEANOTHER)

debug: $(RESULT).elf
	openocd -f /usr/share/openocd/scripts/interface/stlink-v2.cfg -f /usr/share/openocd/scripts/target/stm32f1x.cfg & echo $$! > $(OPENOCD_PIDFILE)
	sleep 1
	arm-none-eabi-gdb --eval-command="target remote localhost:3333" $(RESULT).elf
	if [ -a $(OPENOCD_PIDFILE) ]; then kill `cat $(OPENOCD_PIDFILE)`; fi;

flash : $(RESULT).bin
	if [ -a $(OPENOCD_PIDFILE) ]; then kill `cat $(OPENOCD_PIDFILE)`; fi;
	st-flash write $(RESULT).bin 0x8000000

# pull in dependencies

-include $(OBJS:.o=.d)




