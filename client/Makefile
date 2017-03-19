.PHONY: all clean test

CFLAGS := -std=c11 -Wall -Wmissing-prototypes -Wstrict-prototypes -Os -g3 -I../hidapi/hidapi
LDFLAGS := -lhidapi-libusb -L../hidapi/libusb/.libs

OBJS := client.o

all: client

clean:
	rm -f $(OBJS) client

test: all
	LD_LIBRARY_PATH="`pwd`/../hidapi/libusb/.libs" ./client

client: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LDFLAGS)

.c.o:
	$(CC) $(CFLAGS) -c -o $@ $<


