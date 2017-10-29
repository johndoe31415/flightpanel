.PHONY: all clean test install-xplane test-xplane install gdb valgrind

vpath %.c ../firmware

WINDOWS_FSX_TARGET := fpconnect.exe
WINDOWS_FSX_OBJS := $(addprefix arch/fsx/,simconnect.o simconnect-events.o simconnect-datadefs.o fpconnect.res)
LINUX_EMU_TARGET := fpconnect
LINUX_EMU_OBJS := $(addprefix arch/emulation/,emulator.o)
LINUX_XPLANE_TARGET := fpconnect.so
LINUX_XPLANE_OBJS := $(addprefix arch/xplane/,xsquawkbox.o xplane-datadefs.o xplane_plugin.o)

ifeq ($(VARIANT),)
VARIANT := $(shell cat .default_variant)
endif

ifeq ($(VARIANT),windows-fsx)
ifeq ($(wildcard include/sdk-simconnect),)
$(error The FSX SimConnect header files were not found. Place the SimConnect.h and SimConnect.lib in the sdk-simconnect subdirectory and try again)
endif
TARGET := $(WINDOWS_FSX_TARGET)
PREFIX := i686-w64-mingw32-
CXX := $(PREFIX)g++
CC := $(PREFIX)gcc
AR := $(PREFIX)ar
GDB := $(PREFIX)gdb
STRIP := $(PREFIX)strip
WINDRES := $(PREFIX)windres
BUILDDIR := build/windows
PLATFORM_CFLAGS := -Iinclude/sdk-simconnect -DVARIANT_WINDOWS_FSX -Iarch/fsx
PLATFORM_LDFLAGS := -Linclude/sdk-simconnect -lSimConnect -Lbuild/windows -lhidapi
PLATFORM_OBJS := $(WINDOWS_FSX_OBJS)

else ifeq ($(VARIANT),linux-emulator)
TARGET := $(LINUX_EMU_TARGET)
PREFIX :=
CXX := $(PREFIX)g++
CC := $(PREFIX)gcc
AR := $(PREFIX)ar
GDB := $(PREFIX)gdb
STRIP := $(PREFIX)strip
BUILDDIR := build/linux
PLATFORM_CFLAGS := -DVARIANT_LINUX_EMU -Iarch/emulation
PLATFORM_CFLAGS += -fsanitize=address
PLATFORM_LDFLAGS := -Lbuild/linux -lhidapi-libusb
PLATFORM_OBJS := $(LINUX_EMU_OBJS)

else ifeq ($(VARIANT),linux-xplane)
ifeq ($(wildcard include/xsquawkbox/CHeaders/XPLM/XPLMDefs.h),)
$(error The XSquawkBox header files were not found. Place the SDK files in the xsquawkbox subdirectory and try again)
endif
TARGET := $(LINUX_XPLANE_TARGET)
PREFIX :=
CXX := $(PREFIX)g++
CC := $(PREFIX)gcc
AR := $(PREFIX)ar
GDB := $(PREFIX)gdb
STRIP := $(PREFIX)strip
BUILDDIR := build/linux
PLATFORM_CFLAGS := -fPIC -DLIN -DVARIANT_LINUX_XPLANE -Iinclude/xsquawkbox/CHeaders/XPLM/ -Iarch/xplane
PLATFORM_LDFLAGS := -Wl,--version-script=arch/xplane/fpconnect.ld -shared -Lbuild/linux -lhidapi-libusb
PLATFORM_OBJS := $(LINUX_XPLANE_OBJS)
XPLANE_PATH := $(HOME)/xplane

else
$(error The VARIANT environment variable needs to be either "linux-xplane", "linux-emulator" or "windows-fsx", not "$(VARIANT)")
endif

SHARED_CFLAGS := $(PLATFORM_CFLAGS) -I../firmware -I. -I.. -Ihidapi -Wall -Werror=implicit-function-declaration -Werror=format-extra-args -Werror=format -Wuninitialized -O3 -pthread -g -DDISABLE_FIRMWARE_DEBUG
CFLAGS := -std=c11 $(SHARED_CFLAGS)
CXXFLAGS := -std=c++11 $(SHARED_CFLAGS)
LDFLAGS := $(PLATFORM_LDFLAGS)

OBJS := fpconnect.o fsconnection.o fpconnection.o arbiter.o frequencies.o units.o thread.o $(PLATFORM_OBJS)

all: $(TARGET)

clean:
	rm -f $(OBJS)
	rm -f $(WINDOWS_FSX_TARGET) $(WINDOWS_FSX_OBJS)
	rm -f $(LINUX_EMU_TARGET) $(LINUX_EMU_OBJS)
	rm -f $(LINUX_XPLANE_TARGET) $(LINUX_XPLANE_OBJS)

test: all
	LD_LIBRARY_PATH="`pwd`/build/linux" ./$(TARGET)

install:
	mkdir -p $(BUILDDIR) >/dev/null 2>&1
	cp $(TARGET) $(BUILDDIR)/$(VARIANT)_$(TARGET)
	$(STRIP) -d $(BUILDDIR)/$(VARIANT)_$(TARGET)

install-xplane: all
	mkdir -p $(XPLANE_PATH)/Resources/plugins/flightpanel/64/
	cp $(TARGET) $(XPLANE_PATH)/Resources/plugins/flightpanel/64/lin.xpl

test-xplane: install-xplane
#	LD_LIBRARY_PATH="`pwd`/build/linux" $(XPLANE_PATH)/X-Plane-x86_64
	$(XPLANE_PATH)/X-Plane-x86_64

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS) $(LDFLAGS)

gdb: $(TARGET)
	gdb $(TARGET)

valgrind: $(TARGET)
	valgrind ./$(TARGET)

.cpp.o:
	$(CXX) $(CXXFLAGS) -c -o $@ $<

.c.o:
	$(CC) $(CFLAGS) -c -o $@ $<

arch/fsx/fpconnect.res: arch/fsx/fpconnect.rc
	$(WINDRES) --input $< --output $@ --output-format=coff

