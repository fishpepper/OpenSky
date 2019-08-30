

# Compilation Flags
BOARD_NAME?=atmega328p
BOARD_SPEED?=16000000
BOARD_FLAGS?= -mmcu=$(BOARD_NAME) -DF_CPU=$(BOARD_SPEED)L
SERIAL_PORT?=/dev/tty.usbserial-A50285BI
SERIAL_SPEED?=56700

# continue with normal atmega build
ARCH_DIR = arch/atmega328
ARCH_MAKEFILE = $(ARCH_DIR)/atmega328.mk


