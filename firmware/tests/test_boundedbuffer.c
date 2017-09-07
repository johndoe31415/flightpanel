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
#include <boundedbuffer.h>

static void test_basics(void) {
	subtest_start();
	struct bounded_buffer_t buffer = {
		.bufsize = 32,
		.data = (uint8_t[32]) { },
	};

	for (int i = 0; i < 100; i++) {
		test_assert(boundedbuffer_put(&buffer, "foobar", 7));

		char copy[10];
		memset(copy, 0, sizeof(copy));
		test_assert(boundedbuffer_get(&buffer, copy, 7));
		test_assert(!strcmp(copy, "foobar"));
	}
	subtest_finished();
}

static void test_put_head(void) {
	subtest_start();
	struct bounded_buffer_t buffer = {
		.bufsize = 32,
		.data = (uint8_t[32]) { },
	};

	for (int i = 1; i <= 100; i++) {
		int expect_bytes_put;
		if (i <= 4) {
			expect_bytes_put = 7;
		} else if (i == 5) {
			expect_bytes_put = 4;
		} else {
			expect_bytes_put = 0;
		}
		test_assert_int_eq(boundedbuffer_put_head(&buffer, "foobar", 7), expect_bytes_put);
	}

	uint8_t result[buffer.bufsize];
	memset(result, 0, sizeof(result));
	test_assert(boundedbuffer_get(&buffer, result, buffer.bufsize));

	uint8_t expect_data[] = {
		0x66, 0x6f, 0x6f, 0x62, 0x61, 0x72, 0x00,
		0x66, 0x6f, 0x6f, 0x62, 0x61, 0x72, 0x00,
		0x66, 0x6f, 0x6f, 0x62, 0x61, 0x72, 0x00,
		0x66, 0x6f, 0x6f, 0x62, 0x61, 0x72, 0x00,
		0x66, 0x6f, 0x6f, 0x62
	};
	test_assert(!memcmp(expect_data, result, buffer.bufsize));
	subtest_finished();
}

static void test_single_char(void) {
	subtest_start();

	struct bounded_buffer_t buffer = {
		.bufsize = 32,
		.data = (uint8_t[32]) { },
	};

	test_assert_int_eq(boundedbuffer_getbyte(&buffer), -1);
	test_assert(boundedbuffer_putbyte(&buffer, 'X'));
	test_assert_int_eq(boundedbuffer_getbyte(&buffer), 'X');
	test_assert_int_eq(boundedbuffer_getbyte(&buffer), -1);
	test_assert(boundedbuffer_putbyte(&buffer, 'A'));
	test_assert(boundedbuffer_putbyte(&buffer, 'B'));
	test_assert(boundedbuffer_putbyte(&buffer, 'C'));
	test_assert_int_eq(boundedbuffer_getbyte(&buffer), 'A');
	test_assert_int_eq(boundedbuffer_getbyte(&buffer), 'B');
	test_assert_int_eq(boundedbuffer_getbyte(&buffer), 'C');
	test_assert_int_eq(boundedbuffer_getbyte(&buffer), -1);
	subtest_finished();
}

int main(int argc, char **argv) {
	test_start(argc, argv);
	test_basics();
	test_put_head();
	test_single_char();
	test_finished();
	return 0;
}

