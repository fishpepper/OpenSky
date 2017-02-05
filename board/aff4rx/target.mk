# configuration of this board
# cc2510f16 has 16k flash -> 0x4000
# cc2510f32 has 32k flash -> 0x8000
FLASH_SIZE=0x8000

# continue with normal cc251x build
ARCH_DIR = arch/cc251x
ARCH_MAKEFILE = $(ARCH_DIR)/cc251x.mk

