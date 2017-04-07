.PHONY: all clean test

#VARIANT := windows

ifeq ($(VARIANT),windows)
TARGET := fpconnect.exe
PREFIX := i686-w64-mingw32-
CXX := $(PREFIX)g++
CC := $(PREFIX)gcc
AR := $(PREFIX)ar
GDB := $(PREFIX)gdb
WINDRES := $(PREFIX)windres
BUILDDIR := build/windows
PLATFORM_CFLAGS := -Isdk -DVARIANT_WINDOWS
PLATFORM_LDFLAGS := -Lsdk -lSimConnect -Lhidapi/windows/.libs -lhidapi
PLATFORM_OBJS := simconnect.o simconnect-events.o simconnect-datadefs.o fpconnect.res
else ifeq ($(VARIANT),linux)
TARGET := fpconnect
PREFIX :=
CXX := $(PREFIX)g++
CC := $(PREFIX)gcc
AR := $(PREFIX)ar
GDB := $(PREFIX)gdb
WINDRES := $(PREFIX)windres
BUILDDIR := build/linux
PLATFORM_CFLAGS := -DVARIANT_LINUX
PLATFORM_LDFLAGS := -Lhidapi/libusb/.libs -lhidapi-libusb
PLATFORM_OBJS := emulator.o
else
$(error The VARIANT variable needs to be either "linux" or "windows", not "$(VARIANT)")
endif

SHARED_CFLAGS := $(PLATFORM_CFLAGS) -Ihidapi/hidapi -Wall -Werror=implicit-function-declaration -Werror=format-extra-args -Werror=format -O3 -pthread -g
CFLAGS := -std=c11 $(SHARED_CFLAGS)
CXXFLAGS := -std=c++11 $(SHARED_CFLAGS)
LDFLAGS := $(PLATFORM_LDFLAGS)

OBJS := fpconnect.o fsconnection.o fpconnection.o $(PLATFORM_OBJS)

all: $(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)

test: all
	LD_LIBRARY_PATH="`pwd`/hidapi/libusb/.libs" ./$(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS) $(LDFLAGS)

.cpp.o:
	$(CXX) $(CXXFLAGS) -c -o $@ $<

.c.o:
	$(CC) $(CFLAGS) -c -o $@ $<

fpconnect.res: fpconnect.rc
	$(WINDRES) --input $< --output $@ --output-format=coff

