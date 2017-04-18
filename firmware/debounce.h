#ifndef __DEBOUNCE_H__
#define __DEBOUNCE_H__

#include <stdbool.h>
#include <stdint.h>

enum btnaction_t {
	BUTTON_NOACTION,
	BUTTON_PRESS,
	BUTTON_LONG_PRESS
};

struct button_t {
	/* This is configuration */
	uint16_t threshold;
	uint16_t long_threshold;

	/* These are for internal use */
	uint16_t current_threshold;
	bool last_state;
	bool fired;
};

/*************** AUTO GENERATED SECTION FOLLOWS ***************/
enum btnaction_t button_debounce(struct button_t *button, bool pressed);
/***************  AUTO GENERATED SECTION ENDS   ***************/

#endif
