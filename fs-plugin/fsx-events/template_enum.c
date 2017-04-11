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
#include "simconnect-events.h"

static const struct simconnect_event_enum_data_t event_enum_data[] = {
%for event in events:
	[${event.enum_name}] = {
		.name = "${event.symbol}",
		.flags = ${event.flags},
	},
%endfor
};

const struct simconnect_event_enum_data_t *get_simconnect_event_enum_data(enum simconnect_event_t value) {
	if ((value >= 0) && (value < EVENT_LAST)) {
		return &event_enum_data[value];
	} else {
		return NULL;
	}
}

const char *get_simconnect_event_enum_name(enum simconnect_event_t value) {
	const struct simconnect_event_enum_data_t *data = get_simconnect_event_enum_data(value);
	if (data) {
		return data->name;
	} else {
		return NULL;
	}
}
