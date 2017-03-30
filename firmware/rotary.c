#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "rotary.h"

bool rotary_encoder_update(struct rotary_encoder_t *rotary, bool value1, bool value2) {
	bool changed = false;

	uint8_t change_value = 1;
	if (rotary->time_since_last_change <= 5) {
		change_value = 4;
	} else if (rotary->time_since_last_change < 10) {
		change_value = 2;
	}

	if (value1 == value2) {
		/* Steady state */
		if (rotary->armed && (rotary->origin != value1)) {
			if (!rotary->direction) {
				/* Decrement */
				if (rotary->value >= change_value) {
					changed = true;
					rotary->value -= change_value;
				} else {
					if (rotary->wrap_around) {
						rotary->value = rotary->max_value + 1 - change_value;
						changed = true;
					}
				}
			} else {
				/* Increment */
				if (rotary->value <= (rotary->max_value - change_value)) {
					changed = true;
					rotary->value += change_value;
				} else {
					if (rotary->wrap_around) {
						rotary->value = change_value - 1;
						changed = true;
					}
				}
			}
		}
		rotary->armed = false;
		rotary->origin = value1;
	} else {
		rotary->direction = value2 == rotary->origin;
		rotary->armed = true;
	}

	if (changed) {
		//printf("%d\n", rotary->time_since_last_change);
		rotary->time_since_last_change = 0;
	} else {
		if (rotary->time_since_last_change != 0xffff) {
			rotary->time_since_last_change++;
		}
	}

	return changed;
}
