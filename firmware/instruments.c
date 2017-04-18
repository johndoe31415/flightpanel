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

#include "instruments.h"
#include "usb.h"
#include "rotary.h"
#include "displays.h"
#include "font.h"
#include "vcr-osd-mono-20.h"
#include "vcr-osd-mono-30.h"
#include "timer.h"
#include "pinmap.h"

struct instrument_state {
	uint16_t vhf1, vhf2;
	uint16_t nav1, nav2;
	uint16_t ap_altitude;
	uint16_t ap_climbrate;
	uint16_t ap_ias;
	uint16_t squawk;
	bool transponder_charly;
	uint16_t qnh;
	bool ap_active;
	bool ap_hold_altitude;
	bool ap_hold_navigation;
	bool ap_hold_reverse;
	bool ap_hold_approach;
	bool gps_nav;
};

struct flight_data {
	uint16_t ias;
	uint16_t altitude;
};

static struct rotary_encoder_t rotary1 = {
	.value = 0,
//	.detent_cnt = (19 * 40),		// VHF
	.detent_cnt = (10 * 20),		// NAV
	.wrap_around = true
};

static volatile bool change;

/* Called every 8 ms */
void hid_tick(void) {
	if (rotary_encoder_update(&rotary1, (GPIOD->IDR & GPIO_Pin_10), (GPIOD->IDR & GPIO_Pin_11))) {
		change = true;
	}
}

void instruments_idle_loop(void) {
	/* VHF */
//	const int raster = 25;
//	const int base_freq = 118000;

	/* NAV */
	const int raster = 50;
	const int base_freq = 108000;

	change = true;
	while (true) {
		if (change) {
			change = false;
			char text[16];
			int frequency = (rotary1.value * raster) + base_freq;
			int mhz = frequency / 1000;
			int khz = frequency % 1000;
			sprintf(text, "%3d.%03d", mhz, khz);
//			printf("%4d %+d %s\n", rotary1.value, last - rotary1.value, text);

			const struct surface_t* surface = displays_get_surface(0);
			struct cursor_t cursor = { 0, 35 };
			surface_clear(surface);
			blit_string_to_cursor(&font_vcr_osd_mono_30, text, surface, &cursor);
			display_mark_surface_dirty(0);
		}
	}
}
