.PHONY: all test

CFLAGS := -std=c11 -Wall -Wmissing-prototypes -Wstrict-prototypes -Werror=implicit-function-declaration -O3 -I..
LDFLAGS :=

TEST_COMMON_OBJS := testbed.o
TEST_OBJS := \
	test_vhf_frequency_conversion \
	test_nav_frequency_conversion \
	test_surface \
	test_font \
	test_rotary \
	test_debounce

all: $(TEST_COMMON_OBJS) $(TEST_OBJS)

test_vhf_frequency_conversion: $(TEST_COMMON_OBJS) ../frequencies.o
test_nav_frequency_conversion: $(TEST_COMMON_OBJS) ../frequencies.o
test_surface: $(TEST_COMMON_OBJS) ../surface.o helper_surface.o
test_font: $(TEST_COMMON_OBJS) ../surface.o ../font.o ../vcr-osd-mono-20.o helper_surface.o
test_rotary: $(TEST_COMMON_OBJS) ../rotary.o
test_debounce: $(TEST_COMMON_OBJS) ../debounce.o

test: all
	rm -f tests.log
	for testname in $(TEST_OBJS); do ./$$testname; done
	@./$(firstword $(TEST_OBJS)) --summary

clean:
	rm -f $(TEST_COMMON_OBJS) $(TEST_OBJS) ../*.o
	rm -f helper_surface.o
	rm -f tests.log

.c:
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $+

.c.o:
	$(CC) $(CFLAGS) -include testbed.h -c -o $@ $<

.s.o:
	$(CC) $(CFLAGS) -c -o $@ $<

