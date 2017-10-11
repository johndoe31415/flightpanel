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

#define NAV_MIN_FREQUENCY_KHZ		108000

static void test_coarse(void) {
	subtest_start();
	test_assert_int_eq(frequency_index_to_khz(NAV_RANGE, 0), 108000);
	test_assert_int_eq(frequency_index_to_khz(NAV_RANGE, 1), 108050);
	test_assert_int_eq(frequency_index_to_khz(NAV_RANGE, 2), 108100);
	test_assert_int_eq(frequency_index_to_khz(NAV_RANGE, 3), 108150);
	test_assert_int_eq(frequency_index_to_khz(NAV_RANGE, 4), 108200);
	test_assert_int_eq(frequency_index_to_khz(NAV_RANGE, 5), 108250);
	test_assert_int_eq(frequency_index_to_khz(NAV_RANGE, 20), 109000);

	test_assert_int_eq(0, frequency_khz_to_index(NAV_RANGE, 108000));
	test_assert_int_eq(1, frequency_khz_to_index(NAV_RANGE, 108050));
	test_assert_int_eq(2, frequency_khz_to_index(NAV_RANGE, 108100));
	test_assert_int_eq(3, frequency_khz_to_index(NAV_RANGE, 108150));
	test_assert_int_eq(4, frequency_khz_to_index(NAV_RANGE, 108200));
	test_assert_int_eq(5, frequency_khz_to_index(NAV_RANGE, 108250));
	test_assert_int_eq(20, frequency_khz_to_index(NAV_RANGE, 109000));
	subtest_finished();
}

static void test_fine(void) {
	subtest_start();
	for (int i = 0; i < 20 * 10; i++) {
		test_assert_int_eq(frequency_index_to_khz(NAV_RANGE, i), NAV_MIN_FREQUENCY_KHZ + (i * 50));
		test_assert_int_eq(i, frequency_khz_to_index(NAV_RANGE, NAV_MIN_FREQUENCY_KHZ + (i * 50)));
	}
	subtest_finished();
}

int main(int argc, char **argv) {
	test_start(argc, argv);
	test_coarse();
	test_fine();
	test_finished();
	return 0;
}

