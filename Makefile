.PHONY: all clean readflash program bindump debug

TARGETS := flightpanel flightpanel.bin
PROCESSOR := STM32F407VGT6

PREFIX := arm-none-eabi-
CC := $(PREFIX)gcc
OBJCOPY := $(PREFIX)objcopy
OBJDUMP := $(PREFIX)objdump
AR := $(PREFIX)ar
GDB := $(PREFIX)gdb
STFLASH := st-flash

CFLAGS := -std=c11 -Wall -Os -g -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16 -mthumb -include ext-st/library_config.h -Iext-st/include -Iext-st/include-cmsis -Iext-st/include-cmsis-dev
#CFLAGS += -specs=nano.specs -specs=rdimon.specs
LDFLAGS := -Text-st/stm32f407.ld
STATICLIBS := ext-st/init.o ext-st/stdperiph.a

OBJS := flightpanel.o syscalls.o init.o

all: $(TARGETS)

clean:
	rm -f $(OBJS) $(TARGETS)

readflash:
	$(STFLASH) read firmware.bin 0x08000000 65536

debug:
	$(GDB) flightpanel -ex "target extended-remote :4242"

program: flightpanel.bin
	$(STFLASH) write $< 0x08000000

bindump: flightpanel.bin
	$(OBJDUMP) -b binary -m armv3m -D $<

flightpanel: $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(OBJS) $(STATICLIBS)

flightpanel.bin: flightpanel
	$(OBJCOPY) -O binary $< $@

.c.o:
	$(CC) $(CFLAGS) -c -o $@ $<

.s.o:
	$(CC) $(CFLAGS) -c -o $@ $<

