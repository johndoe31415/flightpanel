#include "testbed.h"
#include <debounce.h>

static void test_noaction(void) {
	struct button_t button = {
		.threshold = 20,
	};
	for (int i = 1; i <= 100; i++) {
		test_assert_int_eq(button_debounce(&button, false), BUTTON_NOACTION);
	}
}

static void test_short_action(void) {
	struct button_t button = {
		.threshold = 20,
	};
	for (int j = 0; j < 10; j++) {
		for (int i = 1; i <= 100; i++) {
			test_assert_int_eq(button_debounce(&button, i <= 50), (i == 20) ? BUTTON_PRESS : BUTTON_NOACTION);
		}
	}
}

static void test_long_action(void) {
	struct button_t button = {
		.threshold = 20,
		.long_threshold = 40,
	};
	for (int i = 1; i <= 100; i++) {
		test_assert_int_eq(button_debounce(&button, true), (i == 40) ? BUTTON_LONG_PRESS : BUTTON_NOACTION);
	}
}

static void test_short_action_longbutton(void) {
	struct button_t button = {
		.threshold = 20,
		.long_threshold = 40,
	};
	for (int i = 1; i <= 100; i++) {
		test_assert_int_eq(button_debounce(&button, (i < 25) ? true : false), (i == 25) ? BUTTON_PRESS : BUTTON_NOACTION);
	}
}

int main(int argc, char **argv) {
	test_start(argc, argv);
	test_noaction();
	test_short_action();
	test_long_action();
	test_short_action_longbutton();
	test_success();
	return 0;
}

