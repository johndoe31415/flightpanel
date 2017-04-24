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
#include "debounce.h"
#include "iomux.h"
#include "instruments.h"
#include "frequencies.h"

static struct instrument_state instrument_state;

static struct rotary_encoder_with_button_t rotary_com = {
	.rotary = {
		.value = 0,
		.detent_cnt = VHF_DIVISIONS,
		.wrap_around = true,
	},
	.button = {
		.threshold = 50,
		.deadtime = 50,
	}
};

static struct rotary_encoder_with_button_t rotary_nav = {
	.rotary = {
		.value = 0,
		.detent_cnt = NAV_DIVISIONS,
		.wrap_around = true,
	},
	.button = {
		.threshold = 50,
		.deadtime = 50,
	}
};

static const struct rotary_input_t rotary_inputs[] = {
	{
		.target = &rotary_com.rotary,
		.pin1 = 55,
		.pin2 = 48,
	},
	{
		.target = &rotary_nav.rotary,
		.pin1 = 111,
		.pin2 = 111,
	},
};

static const struct button_input_t button_inputs[] = {
	{
		.target = &rotary_com.button,
		.pin = 49,
	},
	{
		.target = &rotary_nav.button,
		.pin = 111,
	},
};

/* Called every 8 ms */
void hid_tick(void) {
}

static void swap_uint16(uint16_t *a, uint16_t *b) {
	uint16_t tmp = *a;
	*a = *b;
	*b = tmp;
}

void instruments_handle_inputs(void) {
	for (int i = 0; i < sizeof(rotary_inputs) / sizeof(struct rotary_input_t); i++) {
		rotary_encoder_update(rotary_inputs[i].target, iomux_get_input(rotary_inputs[i].pin1), iomux_get_input(rotary_inputs[i].pin2));
	}

	for (int i = 0; i < sizeof(button_inputs) / sizeof(struct button_input_t); i++) {
		button_debounce(button_inputs[i].target, !iomux_get_input(button_inputs[i].pin));
	}
}

static void redraw_frequency(int surface_index, int frequency_khz) {
	const struct surface_t *surface = displays_get_surface(surface_index);
	char text[16];
	int mhz = frequency_khz / 1000;
	int khz = frequency_khz % 1000;
	sprintf(text, "%3d.%03d", mhz, khz);
	struct cursor_t cursor = { 0, 35 };
	surface_clear(surface);
	blit_string_to_cursor(&font_vcr_osd_mono_30, text, surface, &cursor);
	display_mark_surface_dirty(surface_index);
	printf("%d: %s\n", surface_index, text);
}

void instruments_idle_loop(void) {
	bool redraw_com1_active = true;
	bool redraw_com1_standby = true;

	while (true) {
		if (rotary_com.rotary.changed) {
			rotary_com.rotary.changed = false;
			redraw_com1_standby = true;
		}

		if (rotary_com.button.lastpress != BUTTON_NOACTION) {
			rotary_com.button.lastpress = BUTTON_NOACTION;
			swap_uint16(&rotary_com.rotary.value, &instrument_state.com1_active_index);
			redraw_com1_active = true;
			redraw_com1_standby = true;
		}

		if (redraw_com1_active) {
			redraw_frequency(0, vhf_index_to_frequency_khz(instrument_state.com1_active_index));
			redraw_com1_active = false;
		}

		if (redraw_com1_standby) {
			redraw_frequency(1, vhf_index_to_frequency_khz(rotary_com.rotary.value));
			redraw_com1_standby = false;
		}
	}
}
