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
	uint16_t threshold;				// Threshold for invocation of "short" press
	uint16_t long_threshold;		// Threshold for invocation of "long" press
	uint16_t deadtime;				// Deadtime after which button becomes responsive again

	/* These are for internal use */
	uint16_t counter;
	bool last_state;
	bool fired;
	enum btnaction_t armed;
};

/*************** AUTO GENERATED SECTION FOLLOWS ***************/
enum btnaction_t button_debounce(struct button_t *button, bool pressed);
/***************  AUTO GENERATED SECTION ENDS   ***************/

#endif
