PROJECT = Audio_recorder
BUILD_DIR = bin
SOURCE_DIR = drivers/src
INCLUDE_DIR = drivers/includes

CFILES = main.c
CFILES += $(SOURCE_DIR)/*.c

# TODO - you will need to edit these two lines!
DEVICE = stm32f411ve #ve does not exist but have same memories (ROM and RAM)
# OOCD_FILE = board/stm32f4discovery.cfg

LDSCRIPT = ./stm32-clicker.ld
LDFLAGS += -u _printf_float

# You shouldn't have to edit anything below here.
VPATH += $(SHARED_DIR)
INCLUDES += $(patsubst %,-I%, . $(SHARED_DIR))
#INCLUDES += $(patsubst %,-I-%, . $(INCLUDE_DIR))
INCLUDES += -I-./$(INCLUDE_DIR)


OPENCM3_DIR=../libopencm3

include $(OPENCM3_DIR)/mk/genlink-config.mk
include ./rules.mk
include $(OPENCM3_DIR)/mk/genlink-rules.mk
