#include "testbed.h"
#include <frequencies.h>

static void test_basics(void) {
	test_assert_int_eq(VHF_MIN_FREQUENCY_KHZ, 118000);
}

static void test_fine(void) {
	test_assert_int_eq(vhf_index_to_frequency_khz(0), VHF_MIN_FREQUENCY_KHZ);
	test_assert_int_eq(vhf_index_to_frequency_khz(1) , 118005);
	test_assert_int_eq(vhf_index_to_frequency_khz(2) , 118010);
	test_assert_int_eq(vhf_index_to_frequency_khz(3) , 118015);
	test_assert_int_eq(vhf_index_to_frequency_khz(4) , 118025);
	test_assert_int_eq(vhf_index_to_frequency_khz(5) , 118030);
	test_assert_int_eq(vhf_index_to_frequency_khz(6) , 118035);
	test_assert_int_eq(vhf_index_to_frequency_khz(7) , 118040);
	test_assert_int_eq(vhf_index_to_frequency_khz(8) , 118050);
	test_assert_int_eq(vhf_index_to_frequency_khz(9) , 118055);
	test_assert_int_eq(vhf_index_to_frequency_khz(10), 118060);
	test_assert_int_eq(vhf_index_to_frequency_khz(11), 118065);
	test_assert_int_eq(vhf_index_to_frequency_khz(12), 118075);
	test_assert_int_eq(vhf_index_to_frequency_khz(13), 118080);
	test_assert_int_eq(vhf_index_to_frequency_khz(14), 118085);
	test_assert_int_eq(vhf_index_to_frequency_khz(15), 118090);
	test_assert_int_eq(vhf_index_to_frequency_khz(16), 118100);
	test_assert_int_eq(vhf_index_to_frequency_khz(17), 118105);
	test_assert_int_eq(vhf_index_to_frequency_khz(18), 118110);
	test_assert_int_eq(vhf_index_to_frequency_khz(19), 118115);
	test_assert_int_eq(vhf_index_to_frequency_khz(159), 118990);
	test_assert_int_eq(vhf_index_to_frequency_khz(160), 119000);
	test_assert_int_eq(vhf_index_to_frequency_khz(161), 119005);
	test_assert_int_eq(vhf_index_to_frequency_khz((160 * 19) - 1), 136990);
}

static void test_coarse(void) {
	for (int i = 0; i < 40 * 19; i++) {
		test_assert_int_eq(vhf_index_to_frequency_khz(4 * i), VHF_MIN_FREQUENCY_KHZ + (i * 25));
	}
}

int main(int argc, char **argv) {
	test_start(argc, argv);
	test_basics();
	test_fine();
	test_coarse();
	test_success();
	return 0;
}

