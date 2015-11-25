#
CC = sdcc
CFLAGS = --model-small --opt-code-speed -I /usr/share/sdcc/include
LDFLAGS_FLASH = \
--out-fmt-ihx \
--code-loc 0x000 --code-size 0x4000 \
--xram-loc 0xf000 --xram-size 0x300 \
--iram-size 0x100
ifdef DEBUG
CFLAGS += --debug
endif
SRC = main.c uart.c delay.c clocksource.c frsky.c timeout.c adc.c dma.c wdt.c storage.c flash.c ppm.c apa102.c soft_spi.c failsafe.c sbus.c
ADB=$(SRC:.c=.adb)
ASM=$(SRC:.c=.asm)
LNK=$(SRC:.c=.lnk)
LST=$(SRC:.c=.lst)
REL=$(SRC:.c=.rel)
RST=$(SRC:.c=.rst)
SYM=$(SRC:.c=.sym)
PROGS=main.hex
PCDB=$(PROGS:.hex=.cdb)
PLNK=$(PROGS:.hex=.lnk)
PMAP=$(PROGS:.hex=.map)
PMEM=$(PROGS:.hex=.mem)
PAOM=$(PROGS:.hex=)
%.rel : %.c
	$(CC) -c $(CFLAGS) -o$*.rel $<
all: $(PROGS)
main.hex: $(REL) Makefile
	$(CC) $(LDFLAGS_FLASH) $(CFLAGS) -o main.hex $(REL)
clean:
	rm -f $(ADB) $(ASM) $(LNK) $(LST) $(REL) $(RST) $(SYM)
	rm -f $(PROGS) $(PCDB) $(PLNK) $(PMAP) $(PMEM) $(PAOM)
