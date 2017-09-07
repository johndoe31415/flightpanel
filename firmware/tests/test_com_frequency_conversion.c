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

static void test_basics(void) {
	subtest_start();
	test_assert_int_eq(COM_MIN_FREQUENCY_KHZ, 118000);
	subtest_finished();
}

static void test_fine(void) {
	subtest_start();
	test_assert_int_eq(com_index_to_frequency_khz(0), COM_MIN_FREQUENCY_KHZ);
	test_assert_int_eq(com_index_to_frequency_khz(1) , 118005);
	test_assert_int_eq(com_index_to_frequency_khz(2) , 118010);
	test_assert_int_eq(com_index_to_frequency_khz(3) , 118015);
	test_assert_int_eq(com_index_to_frequency_khz(4) , 118025);
	test_assert_int_eq(com_index_to_frequency_khz(5) , 118030);
	test_assert_int_eq(com_index_to_frequency_khz(6) , 118035);
	test_assert_int_eq(com_index_to_frequency_khz(7) , 118040);
	test_assert_int_eq(com_index_to_frequency_khz(8) , 118050);
	test_assert_int_eq(com_index_to_frequency_khz(9) , 118055);
	test_assert_int_eq(com_index_to_frequency_khz(10), 118060);
	test_assert_int_eq(com_index_to_frequency_khz(11), 118065);
	test_assert_int_eq(com_index_to_frequency_khz(12), 118075);
	test_assert_int_eq(com_index_to_frequency_khz(13), 118080);
	test_assert_int_eq(com_index_to_frequency_khz(14), 118085);
	test_assert_int_eq(com_index_to_frequency_khz(15), 118090);
	test_assert_int_eq(com_index_to_frequency_khz(16), 118100);
	test_assert_int_eq(com_index_to_frequency_khz(17), 118105);
	test_assert_int_eq(com_index_to_frequency_khz(18), 118110);
	test_assert_int_eq(com_index_to_frequency_khz(19), 118115);
	test_assert_int_eq(com_index_to_frequency_khz(159), 118990);
	test_assert_int_eq(com_index_to_frequency_khz(160), 119000);
	test_assert_int_eq(com_index_to_frequency_khz(161), 119005);
	test_assert_int_eq(com_index_to_frequency_khz((160 * 19) - 1), 136990);


	test_assert_int_eq(0, com_frequency_khz_to_index(COM_MIN_FREQUENCY_KHZ));
	test_assert_int_eq(1, com_frequency_khz_to_index(118005));
	test_assert_int_eq(2, com_frequency_khz_to_index(118010));
	test_assert_int_eq(3, com_frequency_khz_to_index(118015));
	test_assert_int_eq(4, com_frequency_khz_to_index(118025));
	test_assert_int_eq(5, com_frequency_khz_to_index(118030));
	test_assert_int_eq(6, com_frequency_khz_to_index(118035));
	test_assert_int_eq(7, com_frequency_khz_to_index(118040));
	test_assert_int_eq(8, com_frequency_khz_to_index(118050));
	test_assert_int_eq(9, com_frequency_khz_to_index(118055));
	test_assert_int_eq(10, com_frequency_khz_to_index(118060));
	test_assert_int_eq(11, com_frequency_khz_to_index(118065));
	test_assert_int_eq(12, com_frequency_khz_to_index(118075));
	test_assert_int_eq(13, com_frequency_khz_to_index(118080));
	test_assert_int_eq(14, com_frequency_khz_to_index(118085));
	test_assert_int_eq(15, com_frequency_khz_to_index(118090));
	test_assert_int_eq(16, com_frequency_khz_to_index(118100));
	test_assert_int_eq(17, com_frequency_khz_to_index(118105));
	test_assert_int_eq(18, com_frequency_khz_to_index(118110));
	test_assert_int_eq(19, com_frequency_khz_to_index(118115));
	test_assert_int_eq(159, com_frequency_khz_to_index(118990));
	test_assert_int_eq(160, com_frequency_khz_to_index(119000));
	test_assert_int_eq(161, com_frequency_khz_to_index(119005));
	test_assert_int_eq((160 * 19) - 1, com_frequency_khz_to_index(136990));
	subtest_finished();
}

static void test_coarse(void) {
	subtest_start();
	for (int i = 0; i < 40 * 19; i++) {
		test_assert_int_eq(com_index_to_frequency_khz(4 * i), COM_MIN_FREQUENCY_KHZ + (i * 25));
		test_assert_int_eq(4 * i, com_frequency_khz_to_index(COM_MIN_FREQUENCY_KHZ + (i * 25)));
	}
	subtest_finished();
}

int main(int argc, char **argv) {
	test_start(argc, argv);
	test_basics();
	test_fine();
	test_coarse();
	test_finished();
	return 0;
}

