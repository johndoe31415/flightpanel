.PHONY: all clean test install-xplane test-xplane

WINDOWS_FSX_TARGET := fpconnect.exe
WINDOWS_FSX_OBJS := simconnect.o simconnect-events.o simconnect-datadefs.o fpconnect.res
LINUX_EMU_TARGET := fpconnect
LINUX_EMU_OBJS := emulator.o
LINUX_XPLANE_TARGET := fpconnect.so
LINUX_XPLANE_OBJS := xsquawkbox.o xplane-datadefs.o

ifeq ($(VARIANT),windows-fsx)
ifeq ($(wildcard sdk-simconnect),)
$(error The FSX SimConnect header files were not found. Place the SimConnect.h and SimConnect.lib in the sdk-simconnect subdirectory and try again)
endif
TARGET := $(WINDOWS_FSX_TARGET)
PREFIX := i686-w64-mingw32-
CXX := $(PREFIX)g++
CC := $(PREFIX)gcc
AR := $(PREFIX)ar
GDB := $(PREFIX)gdb
WINDRES := $(PREFIX)windres
BUILDDIR := build/windows
PLATFORM_CFLAGS := -Isdk-simconnect -DVARIANT_WINDOWS_FSX
PLATFORM_LDFLAGS := -Lsdk-simconnect -lSimConnect -Lbuild/windows -lhidapi
PLATFORM_OBJS := $(WINDOWS_FSX_OBJS)

else ifeq ($(VARIANT),linux-emulator)
TARGET := $(LINUX_EMU_TARGET)
PREFIX :=
CXX := $(PREFIX)g++
CC := $(PREFIX)gcc
AR := $(PREFIX)ar
GDB := $(PREFIX)gdb
BUILDDIR := build/linux
PLATFORM_CFLAGS := -DVARIANT_LINUX_EMU
PLATFORM_LDFLAGS := -Lbuild/linux -lhidapi-libusb
PLATFORM_OBJS := $(LINUX_EMU_OBJS)

else ifeq ($(VARIANT),linux-xplane)
ifeq ($(wildcard xsquawkbox/CHeaders/XPLM/XPLMDefs.h),)
$(error The XSquawkBox header files were not found. Place the SDK files in the xsquawkbox subdirectory and try again)
endif
TARGET := $(LINUX_XPLANE_TARGET)
PREFIX :=
CXX := $(PREFIX)g++
CC := $(PREFIX)gcc
AR := $(PREFIX)ar
GDB := $(PREFIX)gdb
BUILDDIR := build/linux
PLATFORM_CFLAGS := -fPIC -DLIN -DVARIANT_LINUX_XPLANE -Ixsquawkbox/CHeaders/XPLM/
PLATFORM_LDFLAGS := -Wl,--version-script=fpconnect.ld -shared -Lbuild/linux -lhidapi-libusb
PLATFORM_OBJS := $(LINUX_XPLANE_OBJS)
XPLANE_PATH := $(HOME)/xplane

else
$(error The VARIANT environment variable needs to be either "linux-xplane", "linux-emulator" or "windows-fsx", not "$(VARIANT)")
endif

SHARED_CFLAGS := $(PLATFORM_CFLAGS) -Ihidapi/hidapi -Wall -Werror=implicit-function-declaration -Werror=format-extra-args -Werror=format -O3 -pthread -g
CFLAGS := -std=c11 $(SHARED_CFLAGS)
CXXFLAGS := -std=c++11 $(SHARED_CFLAGS)
LDFLAGS := $(PLATFORM_LDFLAGS)

OBJS := fpconnect.o fsconnection.o fpconnection.o $(PLATFORM_OBJS)

all: $(TARGET)

clean:
	rm -f $(OBJS)
	rm -f $(WINDOWS_FSX_TARGET) $(WINDOWS_FSX_OBJS)
	rm -f $(LINUX_EMU_TARGET) $(LINUX_EMU_OBJS)
	rm -f $(LINUX_XPLANE_TARGET) $(LINUX_XPLANE_OBJS)

test: all
	LD_LIBRARY_PATH="`pwd`/build/linux" ./$(TARGET)

install-xplane: all
	mkdir -p $(XPLANE_PATH)/Resources/plugins/flightpanel/64/
	cp $(TARGET) $(XPLANE_PATH)/Resources/plugins/flightpanel/64/lin.xpl

test-xplane: install-xplane
#	LD_LIBRARY_PATH="`pwd`/build/linux" $(XPLANE_PATH)/X-Plane-x86_64
	$(XPLANE_PATH)/X-Plane-x86_64

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS) $(LDFLAGS)

.cpp.o:
	$(CXX) $(CXXFLAGS) -c -o $@ $<

.c.o:
	$(CC) $(CFLAGS) -c -o $@ $<

fpconnect.res: fpconnect.rc
	$(WINDRES) --input $< --output $@ --output-format=coff

