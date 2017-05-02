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

#define USB_REPORT_INTERVAL_MS		25

static struct instrument_state instrument_state;
static uint8_t usb_report_time_tick;

static struct rotary_encoder_with_button_t rotary_com1 = {
	.rotary = {
		.value = 0,
		.detent_cnt = COM_DIVISIONS,
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
		.target = &rotary_com1.rotary,
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
		.target = &rotary_com1.button,
		.pin = 49,
	},
	{
		.target = &rotary_nav.button,
		.pin = 111,
	},
};

static void instruments_send_usb_hid_report(void) {
	usb_report_time_tick = USB_REPORT_INTERVAL_MS;
	struct hid_report_t hid_report = {
		.com1_active = instrument_state.com1_active_index,
		.com1_standby = rotary_com1.rotary.value,
	};
	usb_submit_report(&hid_report);
}

void hid_tick(void) {
	if (!usb_report_time_tick) {
		instruments_send_usb_hid_report();
	} else {
		usb_report_time_tick--;
	}
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

static bool redraw_com1_active = true;
static bool redraw_com1_standby = true;

void instruments_idle_loop(void) {

	rotary_com1.rotary.changed = true;
	rotary_com1.rotary.value = 0x123;

	while (true) {
		if (rotary_com1.rotary.changed) {
			rotary_com1.rotary.changed = false;
			redraw_com1_standby = true;
		}

		if (rotary_com1.button.lastpress != BUTTON_NOACTION) {
			rotary_com1.button.lastpress = BUTTON_NOACTION;
			swap_uint16(&rotary_com1.rotary.value, &instrument_state.com1_active_index);
			redraw_com1_active = true;
			redraw_com1_standby = true;
		}

		if (redraw_com1_active) {
			redraw_frequency(0, com_index_to_frequency_khz(instrument_state.com1_active_index));
			redraw_com1_active = false;
		}

		if (redraw_com1_standby) {
			redraw_frequency(1, com_index_to_frequency_khz(rotary_com1.rotary.value));
			redraw_com1_standby = false;
		}
	}
}

void instruments_set_by_host(const struct hid_set_report_t *report) {
	printf("set %d %d\n", report->com1_active, report->com1_standby);
	instrument_state.com1_active_index = report->com1_active;
	rotary_com1.rotary.value = report->com1_standby;
	redraw_com1_active = true;
	redraw_com1_standby = true;

}
