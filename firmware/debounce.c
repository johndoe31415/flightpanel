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

#include "debounce.h"

enum btnaction_t button_debounce(struct button_t *button, bool pressed) {
	enum btnaction_t action = BUTTON_NOACTION;

	debug("Button [In %d] Ctr %d, Thresholds %d / %d, Armed %d, Fired %d", pressed, button->counter, button->threshold, button->long_threshold, button->armed, button->fired);

	if (pressed) {
		if (button->counter < 0xffff) {
			button->counter++;
		}
	} else {
		if (button->counter) {
			button->counter--;
			if (button->counter == 0) {
				button->fired = false;
			}
		}
	}
	if (button->fired && (button->counter > button->deadtime)) {
		button->counter = button->deadtime;
	}

	bool do_fire = false;
	if (!button->fired) {
		if (button->long_threshold && (button->counter >= button->long_threshold)) {
			button->armed = BUTTON_LONG_PRESS;
			do_fire = true;
		} else if (button->counter >= button->threshold) {
			button->armed = BUTTON_PRESS;
			do_fire = (button->long_threshold == 0);
		}

		if ((button->armed != BUTTON_NOACTION) && (!pressed)) {
			do_fire = true;
		}
	}

	if (do_fire) {
		button->fired = true;
		action = button->armed;
		button->lastpress = button->armed;
		button->armed = BUTTON_NOACTION;
		button->counter = button->deadtime;
	}

	debug(" -> %d\n", action);
	return action;
}
