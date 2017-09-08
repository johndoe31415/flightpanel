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

#include <bitwise.h>
#include <stdint.h>
#include "testbed.h"

static void test_mask(void) {
	subtest_start();
	test_assert_int_eq(BIT_MASK(4), 0xf);
	test_assert_int_eq(BIT_MASK(7), 0x7f);
	test_assert_int_eq(BIT_MASK(8), 0xff);
	test_assert_int_eq(BIT_MASK(20), 0xfffff);
	subtest_finished();
}

static void test_shifted_mask(void) {
	subtest_start();
	test_assert_int_eq(SHIFTED_BIT_MASK(4, 0), 0xf);
	test_assert_int_eq(SHIFTED_BIT_MASK(7, 1), 0xfe);
	test_assert_int_eq(SHIFTED_BIT_MASK(8, 13), 0x1fe000);
	test_assert_int_eq(SHIFTED_BIT_MASK(20, 11), 0x7ffff800);
	subtest_finished();
}

static void test_patch(void) {
	subtest_start();
	test_assert_int_eq(BIT_PATCH(0xabcdef12, 0, 0, 0), 0xabcdef12);
	test_assert_int_eq(BIT_PATCH(0xabcdef12, 0, 8, 0), 0xabcdef00);
	test_assert_int_eq(BIT_PATCH(0xabcdef12, 8, 8, 0), 0xabcd0012);
	test_assert_int_eq(BIT_PATCH(0xabcdef12, 16, 8, 0), 0xab00ef12);
	test_assert_int_eq(BIT_PATCH(0xabcdef12, 16, 8, 0x99), 0xab99ef12);
	test_assert_int_eq(BIT_PATCH(0xabcdef12, 16, 8, 0xa5), 0xaba5ef12);
	test_assert_int_eq(BIT_PATCH(0xabcdef12, 11, 4, 0x0), 0xabcd8712);
	test_assert_int_eq(BIT_PATCH(0xabcdef12, 11, 4, 0xa), 0xabcdd712);
	subtest_finished();
}

static void test_patch_reg(void) {
	subtest_start();
	uint32_t reg = 0xabcdef12;
	BIT_PATCH_REGISTER(reg, 11, 4, 0xa);
	test_assert_int_eq(reg, 0xabcdd712);
	subtest_finished();
}

int main(int argc, char **argv) {
	test_start(argc, argv);
	test_mask();
	test_shifted_mask();
	test_patch();
	test_patch_reg();
	test_finished();
	return 0;
}
