/**
 *	flightpanel - A Cortex-M4 based USB flight panel for flight simulators.
 *	Copyright (C) 2017-2017 Johannes Bauer
 *
 *	This file is part of flightpanel.
 *
 *	flightpanel is free software; you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation; this program is ONLY licensed under
 *	version 3 of the License, later versions are explicitly excluded.
 *
 *	flightpanel is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with flightpanel; if not, write to the Free Software
 *	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *	Johannes Bauer <JohannesBauer@gmx.de>
**/

#include "testbed.h"
#include <frequencies.h>

#define COM_MIN_FREQUENCY_KHZ		118000

static void test_fine(void) {
	subtest_start();
	test_assert_int_eq(frequency_index_to_khz(COM_RANGE, 0), COM_MIN_FREQUENCY_KHZ);
	test_assert_int_eq(frequency_index_to_khz(COM_RANGE, 1) , 118005);
	test_assert_int_eq(frequency_index_to_khz(COM_RANGE, 2) , 118010);
	test_assert_int_eq(frequency_index_to_khz(COM_RANGE, 3) , 118015);
	test_assert_int_eq(frequency_index_to_khz(COM_RANGE, 4) , 118025);
	test_assert_int_eq(frequency_index_to_khz(COM_RANGE, 5) , 118030);
	test_assert_int_eq(frequency_index_to_khz(COM_RANGE, 6) , 118035);
	test_assert_int_eq(frequency_index_to_khz(COM_RANGE, 7) , 118040);
	test_assert_int_eq(frequency_index_to_khz(COM_RANGE, 8) , 118050);
	test_assert_int_eq(frequency_index_to_khz(COM_RANGE, 9) , 118055);
	test_assert_int_eq(frequency_index_to_khz(COM_RANGE, 10), 118060);
	test_assert_int_eq(frequency_index_to_khz(COM_RANGE, 11), 118065);
	test_assert_int_eq(frequency_index_to_khz(COM_RANGE, 12), 118075);
	test_assert_int_eq(frequency_index_to_khz(COM_RANGE, 13), 118080);
	test_assert_int_eq(frequency_index_to_khz(COM_RANGE, 14), 118085);
	test_assert_int_eq(frequency_index_to_khz(COM_RANGE, 15), 118090);
	test_assert_int_eq(frequency_index_to_khz(COM_RANGE, 16), 118100);
	test_assert_int_eq(frequency_index_to_khz(COM_RANGE, 17), 118105);
	test_assert_int_eq(frequency_index_to_khz(COM_RANGE, 18), 118110);
	test_assert_int_eq(frequency_index_to_khz(COM_RANGE, 19), 118115);
	test_assert_int_eq(frequency_index_to_khz(COM_RANGE, 159), 118990);
	test_assert_int_eq(frequency_index_to_khz(COM_RANGE, 160), 119000);
	test_assert_int_eq(frequency_index_to_khz(COM_RANGE, 161), 119005);
	test_assert_int_eq(frequency_index_to_khz(COM_RANGE, (160 * 19) - 1), 136990);


	test_assert_int_eq(0, frequency_khz_to_index(COM_RANGE, COM_MIN_FREQUENCY_KHZ));
	test_assert_int_eq(1, frequency_khz_to_index(COM_RANGE, 118005));
	test_assert_int_eq(2, frequency_khz_to_index(COM_RANGE, 118010));
	test_assert_int_eq(3, frequency_khz_to_index(COM_RANGE, 118015));
	test_assert_int_eq(4, frequency_khz_to_index(COM_RANGE, 118025));
	test_assert_int_eq(5, frequency_khz_to_index(COM_RANGE, 118030));
	test_assert_int_eq(6, frequency_khz_to_index(COM_RANGE, 118035));
	test_assert_int_eq(7, frequency_khz_to_index(COM_RANGE, 118040));
	test_assert_int_eq(8, frequency_khz_to_index(COM_RANGE, 118050));
	test_assert_int_eq(9, frequency_khz_to_index(COM_RANGE, 118055));
	test_assert_int_eq(10, frequency_khz_to_index(COM_RANGE, 118060));
	test_assert_int_eq(11, frequency_khz_to_index(COM_RANGE, 118065));
	test_assert_int_eq(12, frequency_khz_to_index(COM_RANGE, 118075));
	test_assert_int_eq(13, frequency_khz_to_index(COM_RANGE, 118080));
	test_assert_int_eq(14, frequency_khz_to_index(COM_RANGE, 118085));
	test_assert_int_eq(15, frequency_khz_to_index(COM_RANGE, 118090));
	test_assert_int_eq(16, frequency_khz_to_index(COM_RANGE, 118100));
	test_assert_int_eq(17, frequency_khz_to_index(COM_RANGE, 118105));
	test_assert_int_eq(18, frequency_khz_to_index(COM_RANGE, 118110));
	test_assert_int_eq(19, frequency_khz_to_index(COM_RANGE, 118115));
	test_assert_int_eq(159, frequency_khz_to_index(COM_RANGE, 118990));
	test_assert_int_eq(160, frequency_khz_to_index(COM_RANGE, 119000));
	test_assert_int_eq(161, frequency_khz_to_index(COM_RANGE, 119005));
	test_assert_int_eq((160 * 19) - 1, frequency_khz_to_index(COM_RANGE, 136990));
	subtest_finished();
}

static void test_coarse(void) {
	subtest_start();
	for (int i = 0; i < 40 * 19; i++) {
		test_assert_int_eq(frequency_index_to_khz(COM_RANGE, 4 * i), COM_MIN_FREQUENCY_KHZ + (i * 25));
		test_assert_int_eq(4 * i, frequency_khz_to_index(COM_RANGE, COM_MIN_FREQUENCY_KHZ + (i * 25)));
	}
	subtest_finished();
}

static void test_close(void) {
	subtest_start();
	int last_index = -1;
	for (int freq = 118300; freq <= 119700; freq++) {
		int index = frequency_khz_to_index(COM_RANGE, freq);
		if (last_index == -1) {
			last_index = index;
		}
		int conv_freq = frequency_index_to_khz(COM_RANGE, index);
		test_assert(conv_freq >= freq);
		test_assert(last_index <= index);
		debug("%d kHz -> %d -> %d kHz\n", freq, index, conv_freq);
		last_index = index;
	}
	subtest_finished();
}

int main(int argc, char **argv) {
	test_start(argc, argv);
	test_fine();
	test_coarse();
	test_close();
	test_finished();
	return 0;
}

