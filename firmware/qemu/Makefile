.PHONY: all clean gdb sim simgdb

TARGETS := qemu_core

PREFIX := arm-none-eabi-
CC := $(PREFIX)gcc
OBJCOPY := $(PREFIX)objcopy
OBJDUMP := $(PREFIX)objdump
AR := $(PREFIX)ar
GDB := $(PREFIX)gdb

CFLAGS := $(CFLAGS) -std=c11 -Wall -Wmissing-prototypes -Wstrict-prototypes -Werror=implicit-function-declaration -Werror=format -Os -g3 -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16 -mthumb
LDFLAGS := -Tgeneric-hosted.ld
LDFLAGS += -Wl,--gc-sections

OBJS := qemu_core.o syscalls.o ../ivt.o

all: $(TARGETS)

clean:
	rm -f $(OBJS) $(TARGETS)

sim: all
	qemu-system-arm -no-reboot -cpu cortex-m3 -nographic -serial null -monitor none -semihosting -kernel $(TARGETS) -machine lm3s6965evb -d unimp,guest_errors -icount auto

simgdb: all
	-killall $(GDB)
	-killall qemu-system-arm
	qemu-system-arm -no-reboot -cpu cortex-m3 -nographic -serial null -monitor none -semihosting -kernel $(TARGETS) -machine lm3s6965evb -gdb tcp::4242 -S -d unimp,guest_errors

gdb:
	$(GDB) qemu_core -ex "target extended-remote :4242"

qemu_core: $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(OBJS) $(STATICLIBS)

qemu_core.bin: qemu_core
	$(OBJCOPY) -O binary $< $@

.c.o:
	$(CC) $(CFLAGS) -c -o $@ $<

.s.o:
	$(CC) $(CFLAGS) -c -o $@ $<

