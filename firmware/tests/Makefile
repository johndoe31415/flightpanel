.PHONY: all test

CFLAGS := -std=c11 -Wall -Wmissing-prototypes -Wstrict-prototypes -Werror=implicit-function-declaration -O3 -I..
LDFLAGS :=

TEST_COMMON_OBJS := testbed.o
TEST_OBJS := test_frequency_conversion

all: $(TEST_COMMON_OBJS) $(TEST_OBJS)

test_frequency_conversion: ../frequencies.o

test: all
	for testname in $(TEST_OBJS); do ./$$testname; done

clean:
	rm -f $(TEST_COMMON_OBJS) $(TEST_OBJS)

.c:
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $+ $(TEST_COMMON_OBJS)

.c.o:
	$(CC) $(CFLAGS) -include testbed.h -c -o $@ $<

.s.o:
	$(CC) $(CFLAGS) -c -o $@ $<

