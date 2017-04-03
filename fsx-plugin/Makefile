.PHONY: all clean

TARGETS := fpconnect.exe

PREFIX := i686-w64-mingw32-
CXX := $(PREFIX)g++
CC := $(PREFIX)gcc
OBJCOPY := $(PREFIX)objcopy
OBJDUMP := $(PREFIX)objdump
AR := $(PREFIX)ar
GDB := $(PREFIX)gdb

SHARED_CFLAGS := -Isdk -Ihidapi/hidapi -Wall -Werror=implicit-function-declaration -Werror=format-extra-args -Werror=format -O3 -pthread
CFLAGS := -std=c11 $(SHARED_CFLAGS)
CXXFLAGS := -std=c++11 $(SHARED_CFLAGS)
LDFLAGS := -Lsdk -lSimConnect -lpthread -Lhidapi/windows/.libs -lhidapi

OBJS := fpconnect.o fsconnection.o simconnect.o simconnect-events.o simconnect-datadefs.o fpconnection.o

all: $(TARGETS)

clean:
	rm -f $(OBJS) $(TARGETS)

fpconnect.exe: $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS) $(LDFLAGS)

.cpp.o:
	$(CXX) $(CXXFLAGS) -c -o $@ $<

.c.o:
	$(CC) $(CFLAGS) -c -o $@ $<
