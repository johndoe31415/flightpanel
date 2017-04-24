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
#include <debounce.h>

static void test_noaction(void) {
	subtest_start();
	struct button_t button = {
		.threshold = 20,
		.deadtime = 50,
	};
	for (int i = 1; i <= 100; i++) {
		test_assert_int_eq(button_debounce(&button, false), BUTTON_NOACTION);
	}
}

static void test_short_action(void) {
	subtest_start();
	struct button_t button = {
		.threshold = 20,
		.deadtime = 50,
	};
	for (int j = 0; j < 10; j++) {
		for (int i = 1; i <= 100; i++) {
			test_assert_int_eq(button_debounce(&button, i <= 50), (i == 20) ? BUTTON_PRESS : BUTTON_NOACTION);
		}
	}
}

static void test_long_action(void) {
	subtest_start();
	struct button_t button = {
		.threshold = 20,
		.long_threshold = 40,
		.deadtime = 50,
	};
	for (int i = 1; i <= 100; i++) {
		test_assert_int_eq(button_debounce(&button, true), (i == 40) ? BUTTON_LONG_PRESS : BUTTON_NOACTION);
	}
}

static void test_short_action_longbutton(void) {
	subtest_start();
	struct button_t button = {
		.threshold = 20,
		.long_threshold = 40,
		.deadtime = 50,
	};
	for (int i = 1; i <= 100; i++) {
		test_assert_int_eq(button_debounce(&button, (i < 25) ? true : false), (i == 25) ? BUTTON_PRESS : BUTTON_NOACTION);
	}
}

static void test_flaky_button(void) {
	subtest_start();
	struct button_t button = {
		.threshold = 20,
		.deadtime = 50,
	};
	for (int i = 1; i <= 100; i++) {
		test_assert_int_eq(button_debounce(&button, ((i % 5) == 0) ? false : true), (i == 32) ? BUTTON_PRESS : BUTTON_NOACTION);
	}
}

static void test_flaky_long_button(void) {
	subtest_start();
	/* Long button, if flaky, will mistakenly be detected as short button press. */
	struct button_t button = {
		.threshold = 20,
		.long_threshold = 30,
		.deadtime = 50,
	};
	for (int i = 1; i <= 100; i++) {
		test_assert_int_eq(button_debounce(&button, ((i % 5) == 0) ? false : true), (i == 35) ? BUTTON_PRESS : BUTTON_NOACTION);
	}
}

static void test_deadtime(void) {
	subtest_start();
	/* Long button, if flaky, will mistakenly be detected as short button press. */
	struct button_t button = {
		.threshold = 5,
		.deadtime = 3,
	};

	for (int deadtime_wait = 1; deadtime_wait <= 10; deadtime_wait++) {
		// Wait without pressing
		for (int i = 1; i <= 100; i++) {
			test_assert_int_eq(button_debounce(&button, false), BUTTON_NOACTION);
		}

		// Issue press
		for (int i = 1; i <= 10; i++) {
			test_assert_int_eq(button_debounce(&button, true), (i == button.threshold) ? BUTTON_PRESS : BUTTON_NOACTION);
		}

		// Release
		for (int i = 1; i <= deadtime_wait; i++) {
			test_assert_int_eq(button_debounce(&button, false), BUTTON_NOACTION);
		}

		// Retrigger
		for (int i = 1; i <= 10; i++) {
			test_assert_int_eq(button_debounce(&button, true), (i == button.threshold) ? ((deadtime_wait >= button.deadtime) ? BUTTON_PRESS : BUTTON_NOACTION) : BUTTON_NOACTION);
		}
	}
}

int main(int argc, char **argv) {
	test_start(argc, argv);
	test_noaction();
	test_short_action();
	test_long_action();
	test_short_action_longbutton();
	test_flaky_button();
	test_flaky_long_button();
	test_deadtime();
	test_success();
	return 0;
}

