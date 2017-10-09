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
#include "rotary.h"
#include "displays.h"
#include "font.h"
#include "vcr-osd-mono-20.h"
#include "vcr-osd-mono-30.h"
#include "timer.h"
#include "usb.h"
#include "pinmap.h"
#include "debounce.h"
#include "iomux.h"
#include "instruments.h"
#include "frequencies.h"
#include "iomux_pinmap.h"
#include "dsr_tasks.h"
#include "instrument_visuals.h"
#include "configuration.h"

#define USB_REPORT_INTERVAL_MS		25

static struct instrument_state_t instrument_state;
static uint8_t usb_report_time_tick;
static bool display_data_changed[DISPLAY_COUNT];
extern struct configuration active_configuration;

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

static struct button_t radio_com1_button = {
	.threshold = 50,
	.deadtime = 50,
};
static struct button_t radio_nav1_button = {
	.threshold = 50,
	.deadtime = 50,
};
static struct button_t radio_dme_button = {
	.threshold = 50,
	.deadtime = 50,
};
static struct button_t radio_com2_button = {
	.threshold = 50,
	.deadtime = 50,
};
static struct button_t radio_nav2_button = {
	.threshold = 50,
	.deadtime = 50,
};
static struct button_t radio_adf_button = {
	.threshold = 50,
	.deadtime = 50,
};

static struct button_t xpdr_mode_button = {
	.threshold = 50,
	.deadtime = 50,
};
static struct button_t navsrc_button = {
	.threshold = 50,
	.deadtime = 50,
};
static struct button_t ap_master_button = {
	.threshold = 50,
	.deadtime = 50,
};

static struct button_t xpdr_0_button = {
	.threshold = 50,
	.deadtime = 50,
};
static struct button_t xpdr_1_button = {
	.threshold = 50,
	.deadtime = 50,
};
static struct button_t xpdr_2_button = {
	.threshold = 50,
	.deadtime = 50,
};
static struct button_t xpdr_3_button = {
	.threshold = 50,
	.deadtime = 50,
};
static struct button_t xpdr_4_button = {
	.threshold = 50,
	.deadtime = 50,
};
static struct button_t xpdr_5_button = {
	.threshold = 50,
	.deadtime = 50,
};
static struct button_t xpdr_6_button = {
	.threshold = 50,
	.deadtime = 50,
};
static struct button_t xpdr_7_button = {
	.threshold = 50,
	.deadtime = 50,
};
static struct button_t xpdr_VFR_button = {
	.threshold = 50,
	.deadtime = 50,
};
static struct button_t xpdr_IDENT_button = {
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
		.target = &xpdr_mode_button,
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
	{
		.target = &xpdr_0_button,
		.pin = IOMUX_IN_Button_XPDR_0,
	},
	{
		.target = &xpdr_1_button,
		.pin = IOMUX_IN_Button_XPDR_1,
	},
	{
		.target = &xpdr_2_button,
		.pin = IOMUX_IN_Button_XPDR_2,
	},
	{
		.target = &xpdr_3_button,
		.pin = IOMUX_IN_Button_XPDR_3,
	},
	{
		.target = &xpdr_4_button,
		.pin = IOMUX_IN_Button_XPDR_4,
	},
	{
		.target = &xpdr_5_button,
		.pin = IOMUX_IN_Button_XPDR_5,
	},
	{
		.target = &xpdr_6_button,
		.pin = IOMUX_IN_Button_XPDR_6,
	},
	{
		.target = &xpdr_7_button,
		.pin = IOMUX_IN_Button_XPDR_7,
	},
	{
		.target = &xpdr_VFR_button,
		.pin = IOMUX_IN_Button_XPDR_VFR,
	},
	{
		.target = &xpdr_IDENT_button,
		.pin = IOMUX_IN_Button_XPDR_IDENT,
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

static void check_vfr_number_pressed(struct button_t *button, const uint8_t digit) {
	if (button->lastpress != BUTTON_NOACTION) {
		button->lastpress = BUTTON_NOACTION;

		instrument_state.xpdr.edit_timeout = active_configuration.xpdr.edit_timeout_milliseconds;
		if (!instrument_state.xpdr.edit_char) {
			instrument_state.xpdr.edit_char++;
		}

		uint16_t scalar = 1;
		switch (instrument_state.xpdr.edit_char) {
			case 1: scalar = 1000; break;
			case 2: scalar = 100; break;
			case 3: scalar = 10; break;
		}

		uint8_t current_digit = (instrument_state.xpdr.squawk / scalar) % 10;
		instrument_state.xpdr.squawk += (digit - current_digit) * scalar;

		instrument_state.xpdr.edit_char++;
		if (instrument_state.xpdr.edit_char == 5) {
			instrument_state.xpdr.edit_char = 1;
		}

		display_data_changed[DISPLAY_XPDR] = true;

	}
}

static bool timeout(uint16_t *value) {
	if (*value) {
		*value = *value - 1;
		if (!*value) {
			return true;
		}
	}
	return false;
}

void dsr_idle_task(void) {
	if (rotary_changed(&rotary_com1.rotary)) {
		instrument_state.com1_standby_index = rotary_com1.rotary.value;
		display_data_changed[DISPLAY_COM1_STBY] = true;
	}
	if (rotary_changed(&rotary_com2.rotary)) {
		instrument_state.com2_standby_index = rotary_com2.rotary.value;
		display_data_changed[DISPLAY_COM2_STBY] = true;
	}
	if (rotary_changed(&rotary_nav1.rotary)) {
		instrument_state.nav1_standby_index = rotary_nav1.rotary.value;
		display_data_changed[DISPLAY_NAV1_STBY] = true;
	}
	if (rotary_changed(&rotary_nav2.rotary)) {
		instrument_state.nav2_standby_index = rotary_nav2.rotary.value;
		display_data_changed[DISPLAY_NAV2_STBY] = true;
	}

	if (button_pressed(&rotary_com1.button)) {
		swap_uint16(&instrument_state.com1_standby_index, &instrument_state.com1_active_index);
		rotary_com1.rotary.value = instrument_state.com1_standby_index;
		display_data_changed[DISPLAY_COM1] = true;
		display_data_changed[DISPLAY_COM1_STBY] = true;
	}
	if (button_pressed(&rotary_com2.button)) {
		swap_uint16(&instrument_state.com2_standby_index, &instrument_state.com2_active_index);
		rotary_com2.rotary.value = instrument_state.com2_standby_index;
		display_data_changed[DISPLAY_COM2] = true;
		display_data_changed[DISPLAY_COM2_STBY] = true;
	}
	if (button_pressed(&rotary_nav1.button)) {
		swap_uint16(&instrument_state.nav1_standby_index, &instrument_state.nav1_active_index);
		rotary_nav1.rotary.value = instrument_state.nav1_standby_index;
		display_data_changed[DISPLAY_NAV1] = true;
		display_data_changed[DISPLAY_NAV1_STBY] = true;
	}
	if (button_pressed(&rotary_nav2.button)) {
		swap_uint16(&instrument_state.nav2_standby_index, &instrument_state.nav2_active_index);
		rotary_nav2.rotary.value = instrument_state.nav2_standby_index;
		display_data_changed[DISPLAY_NAV2] = true;
		display_data_changed[DISPLAY_NAV2_STBY] = true;
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
	if (button_pressed(&xpdr_mode_button)) {
		instrument_state.xpdr.mode_charly = !instrument_state.xpdr.mode_charly;
		iomux_output_set(IOMUX_OUT_XPDR_STBY, !instrument_state.xpdr.mode_charly);
		iomux_output_set(IOMUX_OUT_XPDR_C, instrument_state.xpdr.mode_charly);
	}
	if (button_pressed(&navsrc_button)) {
		instrument_state.navigate_by_gps = !instrument_state.navigate_by_gps;
		iomux_output_set(IOMUX_OUT_NavSrc_NAV, !instrument_state.navigate_by_gps);
		iomux_output_set(IOMUX_OUT_NavSrc_GPS, instrument_state.navigate_by_gps);
	}
	if (button_pressed(&ap_master_button)) {
		iomux_output_toggle(IOMUX_OUT_AP_MASTER);
	}
	if (button_pressed(&xpdr_IDENT_button)) {
		instrument_state.xpdr.identing = true;
		instrument_state.xpdr.ident_timeout = active_configuration.xpdr.ident_timeout_milliseconds;
		display_data_changed[DISPLAY_XPDR] = true;
	}
	if (button_pressed(&xpdr_VFR_button)) {
		instrument_state.xpdr.squawk = active_configuration.xpdr.vfr_squawk;
		instrument_state.xpdr.edit_char = 0;
		instrument_state.xpdr.edit_timeout = 0;
		display_data_changed[DISPLAY_XPDR] = true;
	}

	check_vfr_number_pressed(&xpdr_0_button, 0);
	check_vfr_number_pressed(&xpdr_1_button, 1);
	check_vfr_number_pressed(&xpdr_2_button, 2);
	check_vfr_number_pressed(&xpdr_3_button, 3);
	check_vfr_number_pressed(&xpdr_4_button, 4);
	check_vfr_number_pressed(&xpdr_5_button, 5);
	check_vfr_number_pressed(&xpdr_6_button, 6);
	check_vfr_number_pressed(&xpdr_7_button, 7);

	if (timeout(&instrument_state.xpdr.edit_timeout)) {
		/* Entering timeout. */
		instrument_state.xpdr.edit_char = 0;
		display_data_changed[DISPLAY_XPDR] = true;
	}

	if (timeout(&instrument_state.xpdr.ident_timeout)) {
		/* IDENT over */
		instrument_state.xpdr.identing = false;
		display_data_changed[DISPLAY_XPDR] = true;
	}

	for (int did = 0; did < DISPLAY_COUNT; did++) {
		if (display_data_changed[did]) {
			// Get the surface for this display index
			display_data_changed[did] = false;
			const struct surface_t *surface = displays_get_surface(did);
			redraw_display(surface, &instrument_state, did);
			display_mark_surface_dirty(did);
		}
	}
}

static void update_value_uint16(bool *changed, uint16_t *dest, const uint16_t src) {
	if (*dest != src) {
		*changed = true;
		*dest = src;
	}
}

void instruments_set_by_host(const struct hid_set_report_t *report) {
	// TODO: This is broken right now
	printf("HostSet %d %d\n", report->com1_active, report->com1_standby);
	update_value_uint16(&display_data_changed[DISPLAY_COM1], &instrument_state.com1_active_index, report->com1_active);
#if 0
	update_value_uint16(&redraw_com1_standby, &rotary_com1.rotary.value, report->com1_standby);
	update_value_uint16(&redraw_com2_active, &instrument_state.com2_active_index, report->com2_active);
	update_value_uint16(&redraw_com2_standby, &rotary_com2.rotary.value, report->com2_standby);
	update_value_uint16(&redraw_nav1_active, &instrument_state.nav1_active_index, report->nav1_active);
	update_value_uint16(&redraw_nav1_standby, &rotary_nav1.rotary.value, report->nav1_standby);
	update_value_uint16(&redraw_nav2_active, &instrument_state.nav2_active_index, report->nav2_active);
	update_value_uint16(&redraw_nav2_standby, &rotary_nav2.rotary.value, report->nav2_standby);
	update_value_uint16(&redraw_xpdr, &instrument_state.squawk, report->squawk);
#endif
}

void instruments_init(void) {
	for (int did = 0; did < DISPLAY_COUNT; did++) {
		display_data_changed[did] = true;
	}
}
