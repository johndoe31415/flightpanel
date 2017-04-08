.PHONY: all clean test

WINDOWS_TARGET := fpconnect.exe
WINDOWS_OBJS := simconnect.o simconnect-events.o simconnect-datadefs.o fpconnect.res
LINUX_TARGET := fpconnect
LINUX_OBJS := emulator.o

ifeq ($(VARIANT),windows)
TARGET := $(WINDOWS_TARGET)
PREFIX := i686-w64-mingw32-
CXX := $(PREFIX)g++
CC := $(PREFIX)gcc
AR := $(PREFIX)ar
GDB := $(PREFIX)gdb
WINDRES := $(PREFIX)windres
BUILDDIR := build/windows
PLATFORM_CFLAGS := -Isdk-simconnect -DVARIANT_WINDOWS
PLATFORM_LDFLAGS := -Lsdk-simconnect -lSimConnect -Lbuild/windows -lhidapi
PLATFORM_OBJS := $(WINDOWS_OBJS)
else ifeq ($(VARIANT),linux)
TARGET := $(LINUX_TARGET)
PREFIX :=
CXX := $(PREFIX)g++
CC := $(PREFIX)gcc
AR := $(PREFIX)ar
GDB := $(PREFIX)gdb
WINDRES := $(PREFIX)windres
BUILDDIR := build/linux
PLATFORM_CFLAGS := -DVARIANT_LINUX
PLATFORM_LDFLAGS := -Lbuild/linux -lhidapi-libusb
PLATFORM_OBJS := $(LINUX_OBJS)
else
$(error The VARIANT environment variable needs to be either "linux" or "windows", not "$(VARIANT)")
endif

SHARED_CFLAGS := $(PLATFORM_CFLAGS) -Ihidapi/hidapi -Wall -Werror=implicit-function-declaration -Werror=format-extra-args -Werror=format -O3 -pthread -g
CFLAGS := -std=c11 $(SHARED_CFLAGS)
CXXFLAGS := -std=c++11 $(SHARED_CFLAGS)
LDFLAGS := $(PLATFORM_LDFLAGS)

OBJS := fpconnect.o fsconnection.o fpconnection.o $(PLATFORM_OBJS)

all: $(TARGET)

clean:
	rm -f $(OBJS)
	rm -f $(WINDOWS_TARGET) $(WINDOWS_OBJS)
	rm -f $(LINUX_TARGET) $(LINUX_OBJS)

test: all
	LD_LIBRARY_PATH="`pwd`/build/linux" ./$(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS) $(LDFLAGS)

.cpp.o:
	$(CXX) $(CXXFLAGS) -c -o $@ $<

.c.o:
	$(CC) $(CFLAGS) -c -o $@ $<

fpconnect.res: fpconnect.rc
	$(WINDRES) --input $< --output $@ --output-format=coff

