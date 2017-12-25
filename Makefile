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
DRIVER = stm32/driver
SYSTEM = SYSTEM
HARDWARE = HARDWARE


###############################################################################
SOURCES += 	$(CORE)/src/core_cm3.c				\
			$(SYSTEM)/sys/sys.c					\
			$(SYSTEM)/delay/delay.c				\
			$(SYSTEM)/usart/usart.c				\
			$(HARDWARE)/src/led.c				\
			$(HARDWARE)/src/ILI93xx.c			\
			$(HARDWARE)/src/adc.c				\
			$(HARDWARE)/src/exti.c				\
			$(HARDWARE)/src/key.c				\
			$(HARDWARE)/src/spi.c				\
			$(HARDWARE)/src/spi_tft.c

SOURCES += startup_stm32f10x_xl.s
SOURCES += stm32f10x_it.c
SOURCES += system_stm32f10x.c
SOURCES += main.c

OBJECTS = $(addprefix $(BUILDDIR)/, $(addsuffix .o, $(basename $(SOURCES))))

INCLUDE +=  -I$(CORE)/include	\
			-I$(SYSTEM)/sys		\
			-I$(SYSTEM)/delay	\
			-I$(SYSTEM)/usart	\
			-I$(HARDWARE)/include

ELF = $(BUILDDIR)/program.elf
HEX = $(BUILDDIR)/program.hex
BIN = $(BUILDDIR)/program.bin
TARGETMAP = stm32f10x.map

LDSCRIPT = stm32f103xe_flash.ld
TARGET_ARCH = -mthumb -mcpu=cortex-m3 --specs=nano.specs --specs=nosys.specs
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
