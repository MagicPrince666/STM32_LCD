################################################################################
#                              stm32  Makefile
#   It uses GCC and stlink.
#
#   Copyright © 2016 Cyan. All rights reserved.
#
#   Author:    ZhiweiSun
#   E-mail:    szwathub@gmail.com
################################################################################
BUILDDIR = build

CORE = stm32/core
PERIPH = stm32/periph


###############################################################################
SOURCES += $(PERIPH)/src/stm32f10x_gpio.c		\
			$(PERIPH)/src/stm32f10x_i2c.c		\
			$(PERIPH)/src/stm32f10x_rcc.c		\
			$(PERIPH)/src/stm32f10x_spi.c		\
			$(PERIPH)/src/stm32f10x_exti.c		\
			$(PERIPH)/src/stm32f10x_flash.c		\
			$(PERIPH)/src/stm32f10x_usart.c		\
			$(PERIPH)/src/misc.c				\
			$(CORE)/src/core_cm3.c

SOURCES += startup_stm32f10x_xl.s
SOURCES += stm32f10x_it.c
SOURCES += system_stm32f10x.c
SOURCES += led.c
SOURCES += main.c

OBJECTS = $(addprefix $(BUILDDIR)/, $(addsuffix .o, $(basename $(SOURCES))))

INCLUDE +=  -I$(CORE)/include	\
			-I$(PERIPH)/include

ELF = $(BUILDDIR)/program.elf
HEX = $(BUILDDIR)/program.hex
BIN = $(BUILDDIR)/program.bin
TARGETMAP = stm32f10x.map

LDSCRIPT = stm32f103xe_flash.ld
TARGET_ARCH = -mthumb -mcpu=cortex-m3
LDFLAGS = -nostartfiles -Wl,--gc-sections,-Map=$(TARGETMAP),-cref $(TARGET_ARCH)

###------------------------  Cross Compile Toolchain ------------------------###
CC      = arm-none-eabi-gcc
LD      = arm-none-eabi-gcc
AR      = arm-none-eabi-ar
OBJCOPY = arm-none-eabi-objcopy

WARNINGS = -Wall -Wshadow -Wcast-qual -Wwrite-strings -Winline -Wno-strict-aliasing

CFLAGS += -I.
CFLAGS += $(TARGET_ARCH)
CFLAGS += -std=gnu99
CFLAGS += $(WARNINGS)
CFLAGS += -ffunction-sections
CFLAGS += -fdata-sections
CFLAGS += $(INCLUDE)
CFLAGS += -DUSE_STDPERIPH_DRIVER

#-mfpu=fpv4-sp-d16 -mfloat-abi=hard

#---------------------------------- Build ----------------------------------#
.PHONY: build
build: $(BIN) $(HEX)

#---------------------------------- Flash ----------------------------------#
flash: build
	st-flash write $(BIN) 0x8000000

#---------------------------------- Clean ----------------------------------#
clean:
	rm -rf build
	rm -f $(TARGETMAP)

$(BIN): $(ELF)
	$(OBJCOPY) -O binary $< $@

$(HEX): $(ELF)
	$(OBJCOPY) -O ihex $< $@

$(ELF): $(OBJECTS)
	$(LD) $(LDFLAGS) -T$(LDSCRIPT) -o $@ $(OBJECTS) $(LDLIBS)

$(BUILDDIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) -c $(CFLAGS) $< -o $@

$(BUILDDIR)/%.o: %.s
	@mkdir -p $(dir $@)
	$(CC) -c $(CFLAGS) $< -o $@
