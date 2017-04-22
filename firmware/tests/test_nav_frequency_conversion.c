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

