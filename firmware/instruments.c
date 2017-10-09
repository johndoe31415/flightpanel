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
#include "iomux_pinmap.h"
#include "dsr_tasks.h"

#define USB_REPORT_INTERVAL_MS		25

static struct instrument_state instrument_state;
static uint8_t usb_report_time_tick;
static bool redraw_com1_active = true;
static bool redraw_com1_standby = true;
static bool redraw_com2_active = true;
static bool redraw_com2_standby = true;
static bool redraw_nav1_active = true;
static bool redraw_nav1_standby = true;
static bool redraw_nav2_active = true;
static bool redraw_nav2_standby = true;

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

static struct rotary_encoder_with_button_t rotary_com2 = {
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

static struct rotary_encoder_with_button_t rotary_nav1 = {
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

static struct rotary_encoder_with_button_t rotary_nav2 = {
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

struct button_t radio_com1_button = {
	.threshold = 50,
	.deadtime = 50,
};
struct button_t radio_nav1_button = {
	.threshold = 50,
	.deadtime = 50,
};
struct button_t radio_dme_button = {
	.threshold = 50,
	.deadtime = 50,
};
struct button_t radio_com2_button = {
	.threshold = 50,
	.deadtime = 50,
};
struct button_t radio_nav2_button = {
	.threshold = 50,
	.deadtime = 50,
};
struct button_t radio_adf_button = {
	.threshold = 50,
	.deadtime = 50,
};

struct button_t xpdf_mode_button = {
	.threshold = 50,
	.deadtime = 50,
};
struct button_t navsrc_button = {
	.threshold = 50,
	.deadtime = 50,
};

struct button_t ap_master_button = {
	.threshold = 50,
	.deadtime = 50,
};

static const struct rotary_input_t rotary_inputs[] = {
	{
		.target = &rotary_com1.rotary,
		.pin1 = IOMUX_IN_Rotary_COM1_A,
		.pin2 = IOMUX_IN_Rotary_COM1_B,
	},
	{
		.target = &rotary_com2.rotary,
		.pin1 = IOMUX_IN_Rotary_COM2_A,
		.pin2 = IOMUX_IN_Rotary_COM2_B,
	},
	{
		.target = &rotary_nav1.rotary,
		.pin1 = IOMUX_IN_Rotary_NAV1_A,
		.pin2 = IOMUX_IN_Rotary_NAV1_B,
	},
	{
		.target = &rotary_nav2.rotary,
		.pin1 = IOMUX_IN_Rotary_NAV2_A,
		.pin2 = IOMUX_IN_Rotary_NAV2_B,
	},
};

static const struct button_input_t button_inputs[] = {
	{
		.target = &rotary_com1.button,
		.pin = IOMUX_IN_Rotary_COM1_Button,
	},
	{
		.target = &rotary_com2.button,
		.pin = IOMUX_IN_Rotary_COM2_Button,
	},
	{
		.target = &rotary_nav1.button,
		.pin = IOMUX_IN_Rotary_NAV1_Button,
	},
	{
		.target = &rotary_nav2.button,
		.pin = IOMUX_IN_Rotary_NAV2_Button,
	},
	{
		.target = &radio_com1_button,
		.pin = IOMUX_IN_Button_Radio_COM1,
	},
	{
		.target = &radio_nav1_button,
		.pin = IOMUX_IN_Button_Radio_NAV1,
	},
	{
		.target = &radio_dme_button,
		.pin = IOMUX_IN_Button_Radio_DME,
	},
	{
		.target = &radio_com2_button,
		.pin = IOMUX_IN_Button_Radio_COM2,
	},
	{
		.target = &radio_nav2_button,
		.pin = IOMUX_IN_Button_Radio_NAV2,
	},
	{
		.target = &radio_adf_button,
		.pin = IOMUX_IN_Button_Radio_ADF,
	},
	{
		.target = &xpdf_mode_button,
		.pin = IOMUX_IN_Button_XPDR_Mode,
	},
	{
		.target = &navsrc_button,
		.pin = IOMUX_IN_Button_NavSrc,
	},
	{
		.target = &ap_master_button,
		.pin = IOMUX_IN_Button_AP,
	},
};

static void instruments_send_usb_hid_report(void) {
	usb_report_time_tick = USB_REPORT_INTERVAL_MS;
	struct hid_report_t hid_report = {
		.com1_active = instrument_state.com1_active_index,
		.com1_standby = rotary_com1.rotary.value,
		.com2_active = instrument_state.com2_active_index,
		.com2_standby = rotary_com2.rotary.value,
		.nav1_active = instrument_state.nav1_active_index,
		.nav1_standby = rotary_nav1.rotary.value,
		.nav2_active = instrument_state.nav2_active_index,
		.nav2_standby = rotary_nav2.rotary.value,
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
		if ((rotary_inputs[i].pin1 != PIN_INVALID) && (rotary_inputs[i].pin2 != PIN_INVALID)) {
			rotary_encoder_update(rotary_inputs[i].target, iomux_get_input(rotary_inputs[i].pin1), iomux_get_input(rotary_inputs[i].pin2));
		}
	}

	for (int i = 0; i < sizeof(button_inputs) / sizeof(struct button_input_t); i++) {
		if (button_inputs[i].pin != PIN_INVALID) {
			button_debounce(button_inputs[i].target, !iomux_get_input(button_inputs[i].pin));
		}
	}
}

static void redraw_frequency_display(bool *do_redraw, int surface_index, uint32_t frequency_khz) {
	if (*do_redraw) {
		*do_redraw = false;
		const struct surface_t *surface = displays_get_surface(surface_index);
		char text[16];
		int mhz = frequency_khz / 1000;
		int khz = frequency_khz % 1000;
		sprintf(text, "%3d.%03d", mhz, khz);
		struct cursor_t cursor = { 0, 35 };
		surface_clear(surface);
		blit_string_to_cursor(&font_vcr_osd_mono_30, text, surface, &cursor);
		display_mark_surface_dirty(surface_index);
//		printf("%d: %s\n", surface_index, text);
	}
}

static bool button_pressed(struct button_t *button) {
	if (button->lastpress != BUTTON_NOACTION) {
		button->lastpress = BUTTON_NOACTION;
		return true;
	}
	return false;
}

static bool rotary_changed(struct rotary_encoder_t *rotary) {
	if (rotary->changed) {
		rotary->changed = false;
		return true;
	}
	return false;
}

void dsr_idle_task(void) {
	dsr_mark_pending(DSR_TASK_IDLE);
	iomux_clamp_all_outputs(iomux_get_input(IOMUX_IN_Switch_MASTER));
	if (rotary_changed(&rotary_com1.rotary)) {
		redraw_com1_standby = true;
	}
	if (rotary_changed(&rotary_com2.rotary)) {
		redraw_com2_standby = true;
	}
	if (rotary_changed(&rotary_nav1.rotary)) {
		redraw_nav1_standby = true;
	}
	if (rotary_changed(&rotary_nav2.rotary)) {
		redraw_nav2_standby = true;
	}

	if (button_pressed(&rotary_com1.button)) {
		swap_uint16(&rotary_com1.rotary.value, &instrument_state.com1_active_index);
		redraw_com1_active = true;
		redraw_com1_standby = true;
	}
	if (button_pressed(&rotary_com2.button)) {
		swap_uint16(&rotary_com2.rotary.value, &instrument_state.com2_active_index);
		redraw_com2_active = true;
		redraw_com2_standby = true;
	}
	if (button_pressed(&rotary_nav1.button)) {
		swap_uint16(&rotary_nav1.rotary.value, &instrument_state.nav1_active_index);
		redraw_nav1_active = true;
		redraw_nav1_standby = true;
	}
	if (button_pressed(&rotary_nav2.button)) {
		swap_uint16(&rotary_nav2.rotary.value, &instrument_state.nav2_active_index);
		redraw_nav2_active = true;
		redraw_nav2_standby = true;
	}
	if (button_pressed(&radio_com1_button)) {
		iomux_output_toggle(IOMUX_OUT_Radio_COM1);
	}
	if (button_pressed(&radio_nav1_button)) {
		iomux_output_toggle(IOMUX_OUT_Radio_NAV1);
	}
	if (button_pressed(&radio_dme_button)) {
		iomux_output_toggle(IOMUX_OUT_Radio_DME);
	}
	if (button_pressed(&radio_com2_button)) {
		iomux_output_toggle(IOMUX_OUT_Radio_COM2);
	}
	if (button_pressed(&radio_nav2_button)) {
		iomux_output_toggle(IOMUX_OUT_Radio_NAV2);
	}
	if (button_pressed(&radio_adf_button)) {
		iomux_output_toggle(IOMUX_OUT_Radio_ADF);
	}
	if (button_pressed(&xpdf_mode_button)) {
		iomux_output_toggle(IOMUX_OUT_XPDR_STBY);
		iomux_output_toggle(IOMUX_OUT_XPDR_C);
	}
	if (button_pressed(&navsrc_button)) {
		iomux_output_toggle(IOMUX_OUT_NavSrc_NAV);
		iomux_output_toggle(IOMUX_OUT_NavSrc_GPS);
	}
	if (button_pressed(&ap_master_button)) {
		iomux_output_toggle(IOMUX_OUT_AP_MASTER);
	}

	redraw_frequency_display(&redraw_com1_active, DISPLAY_COM1, com_index_to_frequency_khz(instrument_state.com1_active_index));
	redraw_frequency_display(&redraw_com1_standby, DISPLAY_COM1_STBY, com_index_to_frequency_khz(rotary_com1.rotary.value));
	redraw_frequency_display(&redraw_com2_active, DISPLAY_COM2, com_index_to_frequency_khz(instrument_state.com2_active_index));
	redraw_frequency_display(&redraw_com2_standby, DISPLAY_COM2_STBY, com_index_to_frequency_khz(rotary_com2.rotary.value));
	redraw_frequency_display(&redraw_nav1_active, DISPLAY_NAV1, nav_index_to_frequency_khz(instrument_state.nav1_active_index));
	redraw_frequency_display(&redraw_nav1_standby, DISPLAY_NAV1_STBY, nav_index_to_frequency_khz(rotary_nav1.rotary.value));
	redraw_frequency_display(&redraw_nav2_active, DISPLAY_NAV2, nav_index_to_frequency_khz(instrument_state.nav2_active_index));
	redraw_frequency_display(&redraw_nav2_standby, DISPLAY_NAV2_STBY, nav_index_to_frequency_khz(rotary_nav2.rotary.value));
}

void instruments_set_by_host(const struct hid_set_report_t *report) {
	printf("HostSet %d %d\n", report->com1_active, report->com1_standby);
	instrument_state.com1_active_index = report->com1_active;
	rotary_com1.rotary.value = report->com1_standby;
	instrument_state.com2_active_index = report->com2_active;
	rotary_com2.rotary.value = report->com2_standby;
	instrument_state.nav1_active_index = report->nav1_active;
	rotary_nav1.rotary.value = report->nav1_standby;
	instrument_state.nav2_active_index = report->nav2_active;
	rotary_nav2.rotary.value = report->nav2_standby;
	redraw_com1_active = true;
	redraw_com1_standby = true;
	redraw_com2_active = true;
	redraw_com2_standby = true;
	redraw_nav1_active = true;
	redraw_nav1_standby = true;
	redraw_nav2_active = true;
	redraw_nav2_standby = true;
}

