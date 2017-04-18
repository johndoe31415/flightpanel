#include "testbed.h"
#include <frequencies.h>

static void test_basics(void) {
	test_assert_int_eq(NAV_MIN_FREQUENCY_KHZ, 108000);
}

static void test_coarse(void) {
	test_assert_int_eq(nav_index_to_frequency_khz(0), 108000);
	test_assert_int_eq(nav_index_to_frequency_khz(1), 108050);
	test_assert_int_eq(nav_index_to_frequency_khz(2), 108100);
	test_assert_int_eq(nav_index_to_frequency_khz(3), 108150);
	test_assert_int_eq(nav_index_to_frequency_khz(4), 108200);
	test_assert_int_eq(nav_index_to_frequency_khz(5), 108250);
	test_assert_int_eq(nav_index_to_frequency_khz(20), 109000);
}

static void test_fine(void) {
	for (int i = 0; i < 20 * 10; i++) {
		test_assert_int_eq(nav_index_to_frequency_khz(i), NAV_MIN_FREQUENCY_KHZ + (i * 50));
	}
}

int main(int argc, char **argv) {
	test_start(argc, argv);
	test_basics();
	test_coarse();
	test_fine();
	test_success();
	return 0;
}

