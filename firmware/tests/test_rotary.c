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
#include <rotary.h>

struct rotary_input_t {
	bool in1;
	bool in2;
	bool expect_switch;
};

static struct rotary_input_t inputs[] = {
	{ false, false, true },
	{ true, false, false },
	{ true, true, true },
	{ false, true, false },
};

static void test_rotary_normal(void) {
	struct rotary_encoder_t rotary = {
		.detent_cnt = 26,
		.wrap_around = false,
	};

	for (int repeats = 1; repeats < 20; repeats += 1) {
		int q = 0;
		test_assert_int_eq(rotary.value, 0);
		for (; q <= rotary.detent_cnt * 5; q++) {
			const struct rotary_input_t *input = &inputs[q % 4];
			for (int x = 0; x < repeats; x++) {
				rotary_encoder_update(&rotary, input->in1, input->in2);
			}
		}
		test_assert_int_eq(rotary.value, rotary.detent_cnt - 1);

		for (; q >= 0; q--) {
			const struct rotary_input_t *input = &inputs[q % 4];
			for (int x = 0; x < repeats; x++) {
				rotary_encoder_update(&rotary, input->in1, input->in2);
			}
		}
		test_assert_int_eq(rotary.value, 0);
	}
}

static void test_rotary_glitch(void) {
	struct rotary_encoder_t rotary = {
		.detent_cnt = 51,
		.wrap_around = false,
	};

	int q = 0;
	test_assert_int_eq(rotary.value, 0);
	for (; q < rotary.detent_cnt * 5; q++) {
		const struct rotary_input_t *input = &inputs[q % 4];
		bool changed_value = rotary_encoder_update(&rotary, input->in1, input->in2);
		int expect_value = rotary.value;

		const struct rotary_input_t *prev_input = &inputs[(q - 1) % 4];
		const struct rotary_input_t *next_input = &inputs[(q + 1) % 4];
		rotary_encoder_update(&rotary, prev_input->in1, prev_input->in2);
		test_assert_int_eq(rotary.value, expect_value);
		rotary_encoder_update(&rotary, input->in1, input->in2);
		test_assert_int_eq(rotary.value, expect_value);
		if (changed_value) {
			/* Also next transition will not change value */
			rotary_encoder_update(&rotary, next_input->in1, next_input->in2);
			test_assert_int_eq(rotary.value, expect_value);
			rotary_encoder_update(&rotary, input->in1, input->in2);
			test_assert_int_eq(rotary.value, expect_value);
		}
	}
	test_assert_int_eq(rotary.value, rotary.detent_cnt - 1);
}

static void test_rotary_wrap(void) {
	const int expect_increment = 4;
	struct rotary_encoder_t rotary = {
		.detent_cnt = 13,
		.wrap_around = true,
	};
	int q = 0;
	int expect_value = 0;
	test_assert_int_eq(rotary.value, expect_value);
	for (; q <= rotary.detent_cnt * 10; q++) {
		const struct rotary_input_t *input = &inputs[q % 4];
		bool changed_value = rotary_encoder_update(&rotary, input->in1, input->in2);
		if (changed_value) {
			expect_value = (expect_value + expect_increment) % rotary.detent_cnt;
			test_assert_int_eq(rotary.value, expect_value);
		}
	}
	for (; q >= 0; q--) {
		const struct rotary_input_t *input = &inputs[q % 4];
		bool changed_value = rotary_encoder_update(&rotary, input->in1, input->in2);
		if (changed_value) {
			expect_value = (expect_value - expect_increment + rotary.detent_cnt) % rotary.detent_cnt;
			test_assert_int_eq(rotary.value, expect_value);
		}
	}
}

int main(int argc, char **argv) {
	test_start(argc, argv);
	test_rotary_normal();
	test_rotary_glitch();
	test_rotary_wrap();
	test_success();
	return 0;
}

