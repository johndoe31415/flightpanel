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
					} else if (rotary->value != 0) {
						rotary->value = 0;
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
					} else if (rotary->value != rotary->max_value) {
						rotary->value = rotary->max_value;
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
		//debug("Change t = %d\n", rotary->time_since_last_change);
		rotary->time_since_last_change = 0;
	} else {
		if (rotary->time_since_last_change != 0xffff) {
			rotary->time_since_last_change++;
		}
	}

	return changed;
}
