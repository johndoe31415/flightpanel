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

#ifndef __ROTARY_H__
#define __ROTARY_H__

#include <stdint.h>
#include <stdbool.h>

struct rotary_encoder_t {
	/* These are configuration variables */
	uint16_t value;
	uint16_t max_value;
	bool wrap_around;

	/* These are internal */
	uint8_t time_since_last_change;
	bool origin;
	bool armed;
	bool direction;
};

/*************** AUTO GENERATED SECTION FOLLOWS ***************/
bool rotary_encoder_update(struct rotary_encoder_t *rotary, bool value1, bool value2);
/***************  AUTO GENERATED SECTION ENDS   ***************/

#endif
