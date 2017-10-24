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
#include "debug.h"

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
					const uint16_t new_value = rotary->value - change_value;
					debug("Rotary dec %d - %d [detent %d] -> %d\n", rotary->value, change_value, rotary->detent_cnt, new_value);
					rotary->value = new_value;
					changed = true;
				} else {
					if (rotary->wrap_around) {
						const uint16_t new_value = rotary->detent_cnt + rotary->value - change_value;
						debug("Rotary dec wrap %d - %d [detent %d] -> %d\n", rotary->value, change_value, rotary->detent_cnt, new_value);
						rotary->value = new_value;
						changed = true;
					} else if (rotary->value != 0) {
						const uint16_t new_value = 0;
						debug("Rotary dec clamp %d - %d [detent %d] -> %d\n", rotary->value, change_value, rotary->detent_cnt, new_value);
						rotary->value = new_value;
						changed = true;
					} else {
						debug("Rotary dec stalled %d - %d [detent %d]\n", rotary->value, change_value, rotary->detent_cnt);
						rotary->time_since_last_change = 0;
					}
				}
			} else {
				/* Increment */
				if (rotary->value < (rotary->detent_cnt - change_value)) {
					const uint16_t new_value = rotary->value + change_value;
					debug("Rotary inc %d + %d [detent %d] -> %d\n", rotary->value, change_value, rotary->detent_cnt, new_value);
					rotary->value = new_value;
					changed = true;
				} else {
					if (rotary->wrap_around) {
						const uint16_t new_value = rotary->value + change_value - rotary->detent_cnt;
						debug("Rotary inc wrap %d + %d [detent %d] -> %d\n", rotary->value, change_value, rotary->detent_cnt, new_value);
						rotary->value = new_value;
						changed = true;
					} else if (rotary->value != rotary->detent_cnt - 1) {
						const uint16_t new_value = rotary->detent_cnt - 1;
						debug("Rotary inc clamp %d + %d [detent %d] -> %d\n", rotary->value, change_value, rotary->detent_cnt, new_value);
						rotary->value = new_value;
						changed = true;
					} else {
						debug("Rotary inc stalled %d + %d [detent %d]\n", rotary->value, change_value, rotary->detent_cnt);
						rotary->time_since_last_change = 0;
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
		rotary->changed = true;
	} else {
		if (rotary->time_since_last_change != 0xffff) {
			rotary->time_since_last_change++;
		}
	}

	return changed;
}

static void rotary_setvalue_raw(struct rotary_encoder_t *rotary, int32_t value) {
	if (rotary->wrap_around) {
		value = value % rotary->detent_cnt;
		if (value < 0) {
			value += rotary->detent_cnt;
		}
		rotary->value = value;
	} else {
		if (value < 0) {
			rotary->value = 0;
		} else if (value >= rotary->detent_cnt) {
			rotary->value = rotary->detent_cnt - 1;
		} else {
			rotary->value = value;
		}
	}
}

int32_t rotary_getvalue(const struct rotary_encoder_t *rotary) {
	if (!rotary->mapping) {
		return rotary->value;
	} else {
		return (rotary->value + rotary->mapping->offset) * rotary->mapping->multiplier;
	}
}

void rotary_setvalue(struct rotary_encoder_t *rotary, int32_t value) {
	if (!rotary->mapping) {
		rotary_setvalue_raw(rotary, value);
	} else {
		rotary_setvalue_raw(rotary, (value / rotary->mapping->multiplier) - rotary->mapping->offset);
	}

}
