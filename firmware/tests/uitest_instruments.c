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
#include <instrument_visuals.h>
#include "gtk_helper.h"
#include "testbed.h"

static void test_instruments(void) {
	subtest_start();
	const struct surface_t surfaces[] = {
		{ .width = 128, .height = 64, .data = (uint8_t[128 * 64 / 8]) { }, },
		{ .width = 128, .height = 64, .data = (uint8_t[128 * 64 / 8]) { }, },
		{ .width = 128, .height = 64, .data = (uint8_t[128 * 64 / 8]) { }, },
		{ .width = 128, .height = 64, .data = (uint8_t[128 * 64 / 8]) { }, },
		{ .width = 128, .height = 64, .data = (uint8_t[128 * 64 / 8]) { }, },
		{ .width = 128, .height = 64, .data = (uint8_t[128 * 64 / 8]) { }, },
		{ .width = 128, .height = 64, .data = (uint8_t[128 * 64 / 8]) { }, },
		{ .width = 128, .height = 64, .data = (uint8_t[128 * 64 / 8]) { }, },
		{ .width = 128, .height = 64, .data = (uint8_t[128 * 64 / 8]) { }, },
		{ .width = 128, .height = 64, .data = (uint8_t[128 * 64 / 8]) { }, },
		{ .width = 128, .height = 64, .data = (uint8_t[128 * 64 / 8]) { }, },
		{ .width = 128, .height = 64, .data = (uint8_t[128 * 64 / 8]) { }, },
	};
	struct instrument_state_t istate = {
		.external = {
			.com1 = {
				.active_index = 100,
				.standby_index = 200,
			},
			.com2 = {
				.active_index = 300,
				.standby_index = 400,
			},
			.xpdr = {
				.state = XPDR_CHARLY | XPDR_MODE_IDENTING,
				.squawk = 7001,
			},
			.ap = {
				.state = AP_ACTIVE | AP_HOLD_ALTITUDE | AP_HOLD_IAS | AP_HOLD_HEADING,
				.altitude = 6900,
				.climbrate = 750,
				.ias = 125,
				.heading = 180,
			},
		},
		.internal = {
			.xpdr = {
				.edit_char = 4,
			},
		},
	};

	redraw_display(&surfaces[0], &istate, DISPLAY_COM1);
	redraw_display(&surfaces[1], &istate, DISPLAY_COM1_STBY);
	redraw_display(&surfaces[2], &istate, DISPLAY_COM2);
	redraw_display(&surfaces[3], &istate, DISPLAY_COM2_STBY);
	redraw_display(&surfaces[4], &istate, DISPLAY_NAV1);
	redraw_display(&surfaces[5], &istate, DISPLAY_NAV1_STBY);
	redraw_display(&surfaces[6], &istate, DISPLAY_NAV2);
	redraw_display(&surfaces[7], &istate, DISPLAY_NAV2_STBY);
	redraw_display(&surfaces[8], &istate, DISPLAY_DME);
	redraw_display(&surfaces[9], &istate, DISPLAY_ADF);
	redraw_display(&surfaces[10], &istate, DISPLAY_AP);
	redraw_display(&surfaces[11], &istate, DISPLAY_XPDR);

	start_gtk_application(surfaces, 12);
	subtest_finished();
}

int main(int argc, char **argv) {
	test_start(argc, argv);
	test_instruments();
	test_finished();
	return 0;
}
