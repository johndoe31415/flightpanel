.PHONY: all clean readflash program bindump gdb gdbserver console tests
.SUFFIXES: .txt

TARGETS := flightpanel flightpanel.bin
PROCESSOR := STM32F407VGT6

PREFIX := arm-none-eabi-
CC := $(PREFIX)gcc
OBJCOPY := $(PREFIX)objcopy
OBJDUMP := $(PREFIX)objdump
AR := $(PREFIX)ar
GDB := $(PREFIX)gdb
STFLASH := st-flash
BUILD_TIMESTAMP_UTC := $(shell /bin/date +'%Y-%m-%d %H:%M:%S')
BUILD_REVISION := $(shell git describe --abbrev=10 --dirty --always)

CFLAGS := $(CFLAGS) -std=c11
CFLAGS += -Wall -Wmissing-prototypes -Wstrict-prototypes -Werror=implicit-function-declaration -Werror=format -Wimplicit-fallthrough -Wshadow
CFLAGS += -Os -g3
CFLAGS += -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16 -mthumb
CFLAGS += -include library_config.h -Iext-st/include -Iext-st/include-cmsis -Iext-st/include-cmsis-dev
CFLAGS += -DBUILD_TIMESTAMP_UTC='"$(BUILD_TIMESTAMP_UTC)"' -DBUILD_REVISION='"$(BUILD_REVISION)"'
CFLAGS += -ffunction-sections -fdata-sections
CFLAGS += -Icube -Icube/include-core -Icube/include-hid -Icube/include-hal -Icube/include-cmsis
#CFLAGS += -specs=nano.specs -specs=rdimon.specs
LDFLAGS := -Tstm32f407.ld
WRITE_ADDR := 0x08000000
#LDFLAGS := -Tstm32f407_boot_ram.ld
#WRITE_ADDR := 0x20000000
LDFLAGS += -Wl,--gc-sections
STATICLIBS := ext-st/stdperiph.a cube/cube.a

OBJS := pinmap.o iomux_pinmap.o ivt.o flightpanel.o syscalls.o init.o rs232.o fault.o timer.o rotary.o spi.o usb.o usbd_hid.o i2c.o eeprom.o configuration.o ssd1306.o displays.o surface.o font.o vcr-osd-mono-20.o inconsolata-30.o instruments.o iomux.o frequencies.o debounce.o stm32f4xx_devid.o stm32f4xx_stackpaint.o corefault.o boundedbuffer.o debugconsole.o debug.o stm32f4xx_reset.o stm32f407_adc.o stm32f407_tempsensor.o dsr_tasks.o stm32f4xx_debug.o atomic.o atomic_mutex.o instrument_visuals.o symbol-font.o morse.o selftest.o
ADDITIONAL_DEPENDENCY := pinmap.h iomux_pinmap.h

all: $(TARGETS)

tests:
	make -C tests all test

clean:
	make -C tests clean
	rm -f $(OBJS) $(TARGETS)
	rm -f flightpanel.sym
	rm -f boot_ram boot_ram.bin

readflash:
	$(STFLASH) read firmware.bin 0x08000000 65536

gdb:
	$(GDB) -d ext-st -d cube flightpanel -ex "py import sys; sys.path.insert(0, '.'); import cm4gdb" -ex "target extended-remote :4242"

console:
	picocom --baud 115200 /dev/ttyUSB*

reset:
	$(STFLASH) reset

program: flightpanel.bin
	-killall st-util
	$(STFLASH) --reset write $< $(WRITE_ADDR)

bindump: flightpanel.bin
	$(OBJDUMP) -b binary -m armv3m -D $<

flightpanel: $(ADDITIONAL_DEPENDENCY) $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(OBJS) $(STATICLIBS)

flightpanel.bin: flightpanel
	$(OBJCOPY) -O binary $< $@

boot_ram: boot_ram.s
	$(CC) $(CFLAGS) -Tstm32f407.ld -nostdlib -o $@ $<

boot_ram.bin: boot_ram
	$(OBJCOPY) -O binary $< $@

program_ramloader: boot_ram.bin
	-killall st-util
	$(STFLASH) write boot_ram.bin 0x08000000

flightpanel.sym: flightpanel.c
	$(CC) $(CFLAGS) -E -dM -o $@ $<

.c.o:
	$(CC) $(CFLAGS) -c -o $@ $<

.s.o:
	$(CC) $(CFLAGS) -c -o $@ $<

pinmap.h: pinmap.txt
	./genpinmap.py

iomux_pinmap.h: iomux.txt
	./geniomuxpinmap.py
