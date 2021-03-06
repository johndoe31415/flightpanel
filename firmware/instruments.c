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
#include <string.h>

#include "instruments.h"
#include "rotary.h"
#include "displays.h"
#include "font.h"
#include "vcr-osd-mono-20.h"
#include "inconsolata-30.h"
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
static bool led_state_changed;
extern struct configuration active_configuration;
static bool report_via_usb = false;

static struct rotary_encoder_with_button_t rotary_com1 = {
	.rotary = {
		.wrap_around = true,
	},
	.button = {
		.threshold = 50,
		.deadtime = 50,
	}
};

static struct rotary_encoder_with_button_t rotary_com2 = {
	.rotary = {
		.wrap_around = true,
	},
	.button = {
		.threshold = 50,
		.deadtime = 50,
	}
};

static struct rotary_encoder_with_button_t rotary_nav1 = {
	.rotary = {
		.wrap_around = true,
	},
	.button = {
		.threshold = 50,
		.long_threshold = 700,
		.deadtime = 50,
	}
};

static struct rotary_encoder_with_button_t rotary_nav2 = {
	.rotary = {
		.wrap_around = true,
	},
	.button = {
		.threshold = 50,
		.deadtime = 50,
	}
};

static struct rotary_encoder_with_button_t rotary_atm = {
	.rotary = {
		.value = 0,
		.detent_cnt = 201,
		.wrap_around = false,
		.mapping = &(struct linear_mapping_t) {
			.multiplier = 1,
			.offset = 900,
		},
	},
	.button = {
		.threshold = 50,
		.long_threshold = 700,
		.deadtime = 50,
	}
};

static struct rotary_encoder_with_button_t rotary_obs = {
	.rotary = {
		.value = 0,
		.detent_cnt = 360,
		.wrap_around = true,
	},
	.button = {
		.threshold = 50,
		.long_threshold = 700,
		.deadtime = 50,
	}
};

static struct rotary_encoder_with_button_t rotary_dme_adf = {
	.rotary = {
		.value = 0,
		.detent_cnt = 1500,
		.wrap_around = false,
	},
	.button = {
		.threshold = 50,
		.deadtime = 50,
	}
};

static struct rotary_encoder_with_button_t rotary_ap_alt = {
	.rotary = {
		.value = 0,
		.detent_cnt = 401,
		.wrap_around = false,
		.mapping = &(struct linear_mapping_t) {
			.multiplier = 100,
			.offset = 0,
		},
	},
	.button = {
		.threshold = 50,
		.deadtime = 50,
	}
};

static struct rotary_encoder_with_button_t rotary_ap_hdg = {
	.rotary = {
		.value = 0,
		.detent_cnt = 360,
		.wrap_around = true,
	},
	.button = {
		.threshold = 50,
		.long_threshold = 700,
		.deadtime = 50,
	}
};

static struct rotary_encoder_with_button_t rotary_ap_ias = {
	.rotary = {
		.value = 0,
		.detent_cnt = 300,
		.wrap_around = false,
	},
	.button = {
		.threshold = 50,
		.deadtime = 50,
	}
};

static struct rotary_encoder_with_button_t rotary_ap_rate = {
	.rotary = {
		.value = 30,
		.detent_cnt = 61,
		.wrap_around = false,
		.mapping = &(struct linear_mapping_t) {
			.multiplier = 100,
			.offset = -3000,
		},
	},
	.button = {
		.threshold = 50,
		.deadtime = 50,
	}
};

/* Radio panel buttons */
static struct button_t radio_com1_button = {
	.threshold = 50,
	.deadtime = 50,
	.long_threshold = 700,
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
	.long_threshold = 700,
};
static struct button_t radio_nav2_button = {
	.threshold = 50,
	.deadtime = 50,
};
static struct button_t radio_adf_button = {
	.threshold = 50,
	.deadtime = 50,
};

/* AP buttons */
static struct button_t ap_master_button = {
	.threshold = 50,
	.deadtime = 50,
};
static struct button_t ap_nav_button = {
	.threshold = 50,
	.deadtime = 50,
};
static struct button_t ap_apr_button = {
	.threshold = 50,
	.deadtime = 50,
};
static struct button_t ap_rev_button = {
	.threshold = 50,
	.deadtime = 50,
};

/* Transponder buttons */
static struct button_t xpdr_mode_button = {
	.threshold = 50,
	.long_threshold = 700,
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

/* Misc buttons */
static struct button_t navsrc_button = {
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
		.target = &rotary_atm.rotary,
		.pin1 = IOMUX_IN_Rotary_ATM_A,
		.pin2 = IOMUX_IN_Rotary_ATM_B,
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
	{
		.target = &rotary_obs.rotary,
		.pin1 = IOMUX_IN_Rotary_OBS_A,
		.pin2 = IOMUX_IN_Rotary_OBS_B,
	},
	{
		.target = &rotary_dme_adf.rotary,
		.pin1 = IOMUX_IN_Rotary_DME_ADF_A,
		.pin2 = IOMUX_IN_Rotary_DME_ADF_B,
	},
	{
		.target = &rotary_ap_alt.rotary,
		.pin1 = IOMUX_IN_Rotary_AP_ALT_A,
		.pin2 = IOMUX_IN_Rotary_AP_ALT_B,
	},
	{
		.target = &rotary_ap_hdg.rotary,
		.pin1 = IOMUX_IN_Rotary_AP_HDG_A,
		.pin2 = IOMUX_IN_Rotary_AP_HDG_B,
	},
	{
		.target = &rotary_ap_ias.rotary,
		.pin1 = IOMUX_IN_Rotary_AP_IAS_A,
		.pin2 = IOMUX_IN_Rotary_AP_IAS_B,
	},
	{
		.target = &rotary_ap_rate.rotary,
		.pin1 = IOMUX_IN_Rotary_AP_RATE_A,
		.pin2 = IOMUX_IN_Rotary_AP_RATE_B,
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
		.target = &rotary_atm.button,
		.pin = IOMUX_IN_Rotary_ATM_Button,
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
		.target = &rotary_obs.button,
		.pin = IOMUX_IN_Rotary_OBS_Button,
	},
	{
		.target = &rotary_dme_adf.button,
		.pin = IOMUX_IN_Rotary_DME_ADF_Button,
	},
	{
		.target = &rotary_ap_alt.button,
		.pin = IOMUX_IN_Rotary_AP_ALT_Button,
	},
	{
		.target = &rotary_ap_hdg.button,
		.pin = IOMUX_IN_Rotary_AP_HDG_Button,
	},
	{
		.target = &rotary_ap_ias.button,
		.pin = IOMUX_IN_Rotary_AP_IAS_Button,
	},
	{
		.target = &rotary_ap_rate.button,
		.pin = IOMUX_IN_Rotary_AP_RATE_Button,
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
	{
		.target = &ap_master_button,
		.pin = IOMUX_IN_Button_AP,
	},
	{
		.target = &ap_nav_button,
		.pin = IOMUX_IN_Button_AP_NAV,
	},
	{
		.target = &ap_apr_button,
		.pin = IOMUX_IN_Button_AP_APR,
	},
	{
		.target = &ap_rev_button,
		.pin = IOMUX_IN_Button_AP_REV,
	},
	{
		.target = &navsrc_button,
		.pin = IOMUX_IN_Button_NavSrc,
	},
};

static void redraw_all_displays(void) {
	for (int i = 0; i < DISPLAY_COUNT; i++) {
		display_data_changed[i] = true;
	}
}

static void display_mode_set_active(void) {
	instrument_state.internal.display_mode.state = ACTIVE;
	instrument_state.internal.display_mode.timer = active_configuration.misc.time_to_blank_milliseconds;
	redraw_all_displays();
}

static void unblank(void) {
	if (instrument_state.internal.display_mode.state == BLANKED) {
		display_mode_set_active();
	} else if (instrument_state.internal.display_mode.state == ACTIVE) {
		instrument_state.internal.display_mode.timer = active_configuration.misc.time_to_blank_milliseconds;
	}
}

static void instruments_send_usb_hid_report(void) {
	usb_report_time_tick = USB_REPORT_INTERVAL_MS;
	if (report_via_usb) {
		usb_submit_report(&instrument_state.external);
	}
}

void hid_tick(void) {
	if (!usb_report_time_tick) {
		instruments_send_usb_hid_report();
	} else {
		usb_report_time_tick--;
	}
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
		unblank();
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
		unblank();
		button->lastpress = BUTTON_NOACTION;

		instrument_state.internal.xpdr.edit_timeout = active_configuration.xpdr.edit_timeout_milliseconds;
		if (!instrument_state.internal.xpdr.edit_char) {
			instrument_state.internal.xpdr.edit_char++;
		}

		uint16_t scalar = 1;
		switch (instrument_state.internal.xpdr.edit_char) {
			case 1: scalar = 1000; break;
			case 2: scalar = 100; break;
			case 3: scalar = 10; break;
		}

		uint8_t current_digit = (instrument_state.external.xpdr.squawk / scalar) % 10;
		instrument_state.external.xpdr.squawk += (digit - current_digit) * scalar;

		instrument_state.internal.xpdr.edit_char++;
		if (instrument_state.internal.xpdr.edit_char == 5) {
			instrument_state.internal.xpdr.edit_char = 1;
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

static void update_leds(void) {
	if (instrument_state.external.flip_switches & SWITCH_MASTER) {
		/* Radio panel */
		iomux_output_set(IOMUX_OUT_Radio_COM1, (instrument_state.external.radio_panel & RADIO_COM1) != 0);
		iomux_output_set(IOMUX_OUT_Radio_NAV1, (instrument_state.external.radio_panel & RADIO_NAV1) != 0);
		iomux_output_set(IOMUX_OUT_Radio_DME, (instrument_state.external.radio_panel & RADIO_DME) != 0);
		iomux_output_set(IOMUX_OUT_Radio_COM2, (instrument_state.external.radio_panel & RADIO_COM2) != 0);
		iomux_output_set(IOMUX_OUT_Radio_NAV2, (instrument_state.external.radio_panel & RADIO_NAV2) != 0);
		iomux_output_set(IOMUX_OUT_Radio_ADF, (instrument_state.external.radio_panel & RADIO_ADF) != 0);

		/* AP */
		iomux_output_set(IOMUX_OUT_AP_MASTER, (instrument_state.external.ap.state & AP_STATE_ACTIVE) != 0);
		iomux_output_set(IOMUX_OUT_AP_ALT_Green, (instrument_state.external.ap.state & AP_ALTITUDE_ARMED) != 0);
		iomux_output_set(IOMUX_OUT_AP_ALT_Red, (instrument_state.external.ap.state & AP_ALTITUDE_HOLD) != 0);
		iomux_output_set(IOMUX_OUT_AP_HDG_Green, (instrument_state.external.ap.state & AP_HEADING_ARMED) != 0);
		iomux_output_set(IOMUX_OUT_AP_HDG_Red, (instrument_state.external.ap.state & AP_HEADING_HOLD) != 0);
		iomux_output_set(IOMUX_OUT_AP_IAS_Green, (instrument_state.external.ap.state & AP_IAS_ARMED) != 0);
		iomux_output_set(IOMUX_OUT_AP_IAS_Red, (instrument_state.external.ap.state & AP_IAS_HOLD) != 0);
		iomux_output_set(IOMUX_OUT_AP_RATE_Green, (instrument_state.external.ap.state & AP_VERTICALSPEED_ARMED) != 0);
		iomux_output_set(IOMUX_OUT_AP_RATE_Red, (instrument_state.external.ap.state & AP_VERTICALSPEED_HOLD) != 0);

		/* Navigational source */
		iomux_output_set(IOMUX_OUT_NavSrc_GPS, instrument_state.external.navigate_by_gps);
		iomux_output_set(IOMUX_OUT_NavSrc_NAV, !instrument_state.external.navigate_by_gps);

		/* XPDR */
		const uint8_t xpdr_mode = instrument_state.external.xpdr.state & XPDR_MODE_MASK;
		iomux_output_set(IOMUX_OUT_XPDR_C, xpdr_mode == XPDR_CHARLY);
		iomux_output_set(IOMUX_OUT_XPDR_STBY, xpdr_mode == XPDR_STANDBY);
	} else {
		iomux_output_setall(0);
	}
}

static void handle_radiopanel_inputs(void) {
	if (radio_com1_button.lastpress != BUTTON_NOACTION) {
		unblank();
		if (radio_com1_button.lastpress == BUTTON_PRESS) {
			/* Short press */
			led_state_changed = true;
			instrument_state.external.radio_panel ^= RADIO_COM1;
		} else {
			/* Long press */
			instrument_state.external.tx_radio_id = 1;
			display_data_changed[DISPLAY_COM1] = true;
			display_data_changed[DISPLAY_COM2] = true;
		}
		radio_com1_button.lastpress = BUTTON_NOACTION;
	}
	if (button_pressed(&radio_nav1_button)) {
		led_state_changed = true;
		instrument_state.external.radio_panel ^= RADIO_NAV1;
	}
	if (button_pressed(&radio_dme_button)) {
		led_state_changed = true;
		instrument_state.external.radio_panel ^= RADIO_DME;
	}
	if (radio_com2_button.lastpress != BUTTON_NOACTION) {
		unblank();
		if (radio_com2_button.lastpress == BUTTON_PRESS) {
			/* Short press */
			led_state_changed = true;
			instrument_state.external.radio_panel ^= RADIO_COM2;
		} else {
			/* Long press */
			instrument_state.external.tx_radio_id = 2;
			display_data_changed[DISPLAY_COM1] = true;
			display_data_changed[DISPLAY_COM2] = true;
		}
		radio_com2_button.lastpress = BUTTON_NOACTION;
	}
	if (button_pressed(&radio_nav2_button)) {
		led_state_changed = true;
		instrument_state.external.radio_panel ^= RADIO_NAV2;
	}
	if (button_pressed(&radio_adf_button)) {
		led_state_changed = true;
		instrument_state.external.radio_panel ^= RADIO_ADF;
	}
}

static void swap_uint16(uint16_t *value1, uint16_t *value2) {
	uint16_t tmp = *value1;
	*value1 = *value2;
	*value2 = tmp;
}

static void handle_comnav_inputs(struct xcom_state_t *comnav, struct rotary_encoder_with_button_t *rotary, const enum display_t active_display, const enum display_t standby_display, uint16_t *ident_inhibit_timeout)  {
	if (rotary_changed(&rotary->rotary)) {
		comnav->standby_index = rotary_getvalue(&rotary->rotary);
		display_data_changed[standby_display] = true;
	}

	if (rotary->button.lastpress != BUTTON_NOACTION) {
		if (rotary->button.lastpress == BUTTON_PRESS) {
			/* Short button press, exchange ACTIVE <-> STBY */
			if (comnav->active_index != comnav->standby_index) {
				if (ident_inhibit_timeout) {
					/* Reset IDENT value when changing frequencies */
					*ident_inhibit_timeout = active_configuration.instruments.nav_ident_inhibit_timeout;
				}
				swap_uint16(&comnav->active_index, &comnav->standby_index);
				rotary_setvalue(&rotary->rotary, comnav->standby_index);
				display_data_changed[active_display] = true;
				display_data_changed[standby_display] = true;
			}
		} else {
			/* Long button press, exchange NAV1 <-> NAV2 */
			// TODO
		}
		rotary->button.lastpress = BUTTON_NOACTION;
	}
}

static bool invert_ap_state(enum ap_mode_t arm_mode, enum ap_mode_t hold_mode, uint16_t cancel_modes) {
	const bool currently_armed = (instrument_state.external.ap.state & arm_mode) != 0;
	const bool currently_holding = (instrument_state.external.ap.state & hold_mode) != 0;
	const bool currently_active = currently_armed || currently_holding;
	instrument_state.external.ap.state = (instrument_state.external.ap.state & ~cancel_modes) | (currently_active ? 0 : arm_mode);
	return !currently_active;
}

static void handle_ap_inputs(void) {
	if (rotary_changed(&rotary_ap_alt.rotary)) {
		instrument_state.external.ap.altitude = rotary_getvalue(&rotary_ap_alt.rotary);
		display_data_changed[DISPLAY_AP] = true;
	}
	if (rotary_changed(&rotary_ap_hdg.rotary)) {
		instrument_state.external.ap.heading = rotary_getvalue(&rotary_ap_hdg.rotary);
		display_data_changed[DISPLAY_AP] = true;
	}
	if (rotary_changed(&rotary_ap_ias.rotary)) {
		instrument_state.external.ap.ias = rotary_getvalue(&rotary_ap_ias.rotary);
		display_data_changed[DISPLAY_AP] = true;
	}
	if (rotary_changed(&rotary_ap_rate.rotary)) {
		instrument_state.external.ap.climbrate = rotary_getvalue(&rotary_ap_rate.rotary);
		display_data_changed[DISPLAY_AP] = true;
	}

	if (button_pressed(&rotary_ap_alt.button)) {
		invert_ap_state(AP_ALTITUDE_ARMED, AP_ALTITUDE_HOLD, AP_VERTICAL_MODES);
		led_state_changed = true;
		display_data_changed[DISPLAY_AP] = true;
	}
	if (rotary_ap_hdg.button.lastpress != BUTTON_NOACTION) {
		if (rotary_ap_hdg.button.lastpress == BUTTON_PRESS) {
			invert_ap_state(AP_HEADING_ARMED, AP_HEADING_HOLD, AP_HORIZONTAL_MODES);
			led_state_changed = true;
		} else {
			instrument_state.external.ap.heading = (instrument_state.external.ap.heading + 180) % 360;
			rotary_setvalue(&rotary_ap_hdg.rotary, instrument_state.external.ap.heading);
		}
		rotary_ap_hdg.button.lastpress = BUTTON_NOACTION;
		display_data_changed[DISPLAY_AP] = true;
	}
	if (button_pressed(&rotary_ap_ias.button)) {
		invert_ap_state(AP_IAS_ARMED, AP_IAS_HOLD, AP_IAS_ARMED | AP_IAS_HOLD);
		led_state_changed = true;
		display_data_changed[DISPLAY_AP] = true;
	}
	if (button_pressed(&rotary_ap_rate.button)) {
		invert_ap_state(AP_VERTICALSPEED_ARMED, AP_VERTICALSPEED_HOLD, AP_VERTICAL_MODES);
		led_state_changed = true;
		display_data_changed[DISPLAY_AP] = true;
	}

	if (button_pressed(&ap_master_button)) {
		instrument_state.external.ap.state ^= AP_STATE_ACTIVE;
		led_state_changed = true;
		display_data_changed[DISPLAY_AP] = true;
	}
	if (button_pressed(&ap_nav_button)) {
		invert_ap_state(AP_NAVIGATION_ARMED, AP_NAVIGATION_HOLD, AP_HORIZONTAL_MODES);
		led_state_changed = true;
		display_data_changed[DISPLAY_AP] = true;
	}
	if (button_pressed(&ap_apr_button)) {
		invert_ap_state(AP_GLIDESLOPE_ARMED, AP_GLIDESLOPE_HOLD, AP_VERTICAL_MODES);
		led_state_changed = true;
		display_data_changed[DISPLAY_AP] = true;
	}
	if (button_pressed(&ap_rev_button)) {
		instrument_state.external.ap.state ^= AP_STATE_BACKCOURSE;
		led_state_changed = true;
		display_data_changed[DISPLAY_AP] = true;
	}
}

static void handle_xpdr_inputs(void) {
	if (xpdr_mode_button.lastpress != BUTTON_NOACTION) {
		unblank();
		uint8_t mode = instrument_state.external.xpdr.state & XPDR_MODE_MASK;

		if (xpdr_mode_button.lastpress == BUTTON_PRESS) {
			/* Short press */
			if (mode == XPDR_STANDBY) {
				mode = XPDR_CHARLY;
			} else {
				mode = XPDR_STANDBY;
			}
		} else {
			/* Long press */
			if (mode == XPDR_OFF) {
				mode = XPDR_STANDBY;
			} else {
				mode = XPDR_OFF;
			}
		}
		instrument_state.external.xpdr.state = (instrument_state.external.xpdr.state & ~XPDR_MODE_MASK) | mode;
		led_state_changed = true;
		xpdr_mode_button.lastpress = BUTTON_NOACTION;
	}
	if (button_pressed(&xpdr_IDENT_button)) {
		instrument_state.external.xpdr.state |= XPDR_MODE_IDENTING;
		instrument_state.internal.xpdr.ident_timeout = active_configuration.xpdr.ident_timeout_milliseconds;
		display_data_changed[DISPLAY_XPDR] = true;
	}
	if (button_pressed(&xpdr_VFR_button)) {
		instrument_state.external.xpdr.squawk = active_configuration.xpdr.vfr_squawk;
		instrument_state.internal.xpdr.edit_char = 0;
		instrument_state.internal.xpdr.edit_timeout = 0;
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

	if (timeout(&instrument_state.internal.xpdr.edit_timeout)) {
		/* Entering timeout. */
		instrument_state.internal.xpdr.edit_char = 0;
		display_data_changed[DISPLAY_XPDR] = true;
	}

	if (timeout(&instrument_state.internal.xpdr.ident_timeout)) {
		/* IDENT over */
		instrument_state.external.xpdr.state &= ~XPDR_MODE_IDENTING;
		display_data_changed[DISPLAY_XPDR] = true;
	}
}

static void handle_adf_inputs(void) {
	if (rotary_changed(&rotary_dme_adf.rotary)) {
		instrument_state.external.adf.frequency_khz = rotary_getvalue(&rotary_dme_adf.rotary);
		display_data_changed[DISPLAY_ADF] = true;
	}
	if (button_pressed(&rotary_dme_adf.button)) {
		instrument_state.external.dme_nav_id = 3 - instrument_state.external.dme_nav_id;
		display_data_changed[DISPLAY_NAV1_STBY] = true;
		display_data_changed[DISPLAY_NAV2_STBY] = true;
	}
}

static void handle_nav_src_inputs(void) {
	if (button_pressed(&navsrc_button)) {
		instrument_state.external.navigate_by_gps = !instrument_state.external.navigate_by_gps;
		led_state_changed = true;
	}
}

static void set_qnh(uint16_t new_qnh_value) {
	instrument_state.external.qnh = new_qnh_value;
	instrument_state.internal.screen_mplex.qnh = QNH;
	instrument_state.internal.screen_mplex.qnh_timeout = 2000;
	display_data_changed[DISPLAY_QNH] = true;
}

static void handle_qnh_inputs(void) {
	if (rotary_changed(&rotary_atm.rotary)) {
		set_qnh(rotary_getvalue(&rotary_atm.rotary));
	}
	if (rotary_atm.button.lastpress != BUTTON_NOACTION) {
		if (rotary_atm.button.lastpress == BUTTON_LONG_PRESS) {
			/* Long press on ATM rotary -> standard QNH 1013 mBar */
			rotary_setvalue(&rotary_atm.rotary, 1013);
			set_qnh(1013);
		}
		rotary_atm.button.lastpress = BUTTON_NOACTION;
	}
	if (timeout(&instrument_state.internal.screen_mplex.qnh_timeout)) {
		instrument_state.internal.screen_mplex.qnh = DEFAULT;
		display_data_changed[DISPLAY_QNH] = true;
	}
}

static void handle_obs_inputs(void) {
	if (rotary_changed(&rotary_obs.rotary)) {
		uint16_t *obs = (instrument_state.internal.active_obs == 0) ? &instrument_state.external.nav1.obs : &instrument_state.external.nav2.obs;
		*obs = rotary_getvalue(&rotary_obs.rotary);
		display_data_changed[(instrument_state.internal.active_obs == 0) ? DISPLAY_NAV1_STBY : DISPLAY_NAV2_STBY] = true;
	}
	if (rotary_obs.button.lastpress != BUTTON_NOACTION) {
		if (rotary_obs.button.lastpress == BUTTON_PRESS) {
			/* Short press, change which NAV is selected */
			instrument_state.internal.active_obs = !instrument_state.internal.active_obs;
			rotary_setvalue(&rotary_obs.rotary, (instrument_state.internal.active_obs == 0) ? instrument_state.external.nav1.obs : instrument_state.external.nav2.obs);
			display_data_changed[DISPLAY_NAV1_STBY] = true;
			display_data_changed[DISPLAY_NAV2_STBY] = true;
		} else {
			/* Long press, select back course */
			uint16_t *obs = (instrument_state.internal.active_obs == 0) ? &instrument_state.external.nav1.obs : &instrument_state.external.nav2.obs;
			rotary_setvalue(&rotary_obs.rotary, rotary_getvalue(&rotary_obs.rotary) + 180);
			*obs = rotary_getvalue(&rotary_obs.rotary);
			display_data_changed[(instrument_state.internal.active_obs == 0) ? DISPLAY_NAV1_STBY : DISPLAY_NAV2_STBY] = true;
		}
		rotary_obs.button.lastpress = BUTTON_NOACTION;
	}
}

static void handle_switches(void) {
	uint8_t new_state = 0;
	new_state |= iomux_get_input(IOMUX_IN_Switch_MASTER) ? 0 : SWITCH_MASTER;
	new_state |= iomux_get_input(IOMUX_IN_Switch_BCN) ? 0 : SWITCH_BCN;
	new_state |= iomux_get_input(IOMUX_IN_Switch_LAND) ? 0 : SWITCH_LAND;
	new_state |= iomux_get_input(IOMUX_IN_Switch_TAXI) ? 0 : SWITCH_TAXI;
	new_state |= iomux_get_input(IOMUX_IN_Switch_NAV) ? 0 : SWITCH_NAV;
	new_state |= iomux_get_input(IOMUX_IN_Switch_STRB) ? 0 : SWITCH_STRB;

	uint8_t changed_switches = instrument_state.external.flip_switches ^ new_state;
	instrument_state.external.flip_switches = new_state;
	if (changed_switches & SWITCH_MASTER) {
		/* MASTER flip switch has been updated. */
		unblank();
		update_leds();
		redraw_all_displays();
	}
}

static void handle_nav_ident_inhibit_timeouts(void) {
	if (timeout(&instrument_state.internal.ident.nav1_ident_inhibit_timeout)) {
		display_data_changed[DISPLAY_NAV1] = true;
	}
	if (timeout(&instrument_state.internal.ident.nav2_ident_inhibit_timeout)) {
		display_data_changed[DISPLAY_NAV2] = true;
	}
}

static void handle_display_mode_transitions(void) {
	if (instrument_state.internal.display_mode.timer > 0) {
		instrument_state.internal.display_mode.timer--;
		if (!instrument_state.internal.display_mode.timer) {
			if (instrument_state.internal.display_mode.state == ACTIVE) {
				/* Blank now */
				instrument_state.internal.display_mode.state = BLANKED;
				for (int did = 0; did < DISPLAY_COUNT; did++) {
					display_data_changed[did] = false;
					const struct surface_t *surface = displays_get_surface(did);
					redraw_display(surface, &instrument_state, did);
					display_mark_surface_dirty(did);
				}
			} else if (instrument_state.internal.display_mode.state == BOOTING) {
				/* Set active now */
				display_mode_set_active();
			}
		}
	}
}

static void handle_shutoff(void) {
	static int shutoff_blink_led_period = 0;
	if ((instrument_state.external.flip_switches & SWITCH_MASTER) == 0) {
		iomux_output_set(IOMUX_OUT_XPDR_STBY, (shutoff_blink_led_period <= 0));
		shutoff_blink_led_period++;
		if (shutoff_blink_led_period > 2000) {
			shutoff_blink_led_period = 0;
		}
	}
}

void dsr_idle_task(void) {
	handle_radiopanel_inputs();
	handle_comnav_inputs(&instrument_state.external.com1.freq, &rotary_com1, DISPLAY_COM1, DISPLAY_COM1_STBY, NULL);
	handle_comnav_inputs(&instrument_state.external.com2.freq, &rotary_com2, DISPLAY_COM2, DISPLAY_COM2_STBY, NULL);
	handle_comnav_inputs(&instrument_state.external.nav1.freq, &rotary_nav1, DISPLAY_NAV1, DISPLAY_NAV1_STBY, &instrument_state.internal.ident.nav1_ident_inhibit_timeout);
	handle_comnav_inputs(&instrument_state.external.nav2.freq, &rotary_nav2, DISPLAY_NAV2, DISPLAY_NAV2_STBY, &instrument_state.internal.ident.nav2_ident_inhibit_timeout);
	handle_adf_inputs();
	handle_ap_inputs();
	handle_xpdr_inputs();
	handle_nav_src_inputs();
	handle_nav_ident_inhibit_timeouts();
	handle_qnh_inputs();
	handle_obs_inputs();
	handle_switches();
	handle_display_mode_transitions();
	handle_shutoff();

	if (led_state_changed) {
		led_state_changed = false;
		update_leds();
	}

	bool display_change = false;
	for (int did = 0; did < DISPLAY_COUNT; did++) {
		if (display_data_changed[did]) {
			display_change = true;
			// Get the surface for this display index
			display_data_changed[did] = false;
			const struct surface_t *surface = displays_get_surface(did);
			redraw_display(surface, &instrument_state, did);
			display_mark_surface_dirty(did);
		}
	}
	if (display_change) {
		unblank();
	}
}

static bool copy_if_changed(void *dst, const void *src, const unsigned int length) {
	bool changed = false;
	if (memcmp(dst, src, length)) {
		memcpy(dst, src, length);
		changed = true;
	}
	return changed;
}

static void instruments_set_by_host_report02(const struct hid_set_report_02_t *report) {
	led_state_changed = copy_if_changed(&instrument_state.external.radio_panel, &report->radio_panel, sizeof(instrument_state.external.radio_panel)) || led_state_changed;
	bool divisions_changed = copy_if_changed(&instrument_state.external.com_divisions, &report->com_divisions, sizeof(instrument_state.external.com_divisions)) || led_state_changed;
	divisions_changed = copy_if_changed(&instrument_state.external.nav_divisions, &report->nav_divisions, sizeof(instrument_state.external.nav_divisions)) || divisions_changed;
	bool active_radio_changed = false;
	if ((report->tx_radio_id == 1) || (report->tx_radio_id == 2)) {
		active_radio_changed = copy_if_changed(&instrument_state.external.tx_radio_id, &report->tx_radio_id, sizeof(instrument_state.external.tx_radio_id));
	}
	display_data_changed[DISPLAY_COM1] = copy_if_changed(&instrument_state.external.com1.freq.active_index, &report->com1.freq.active_index, sizeof(instrument_state.external.com1.freq.active_index)) || display_data_changed[DISPLAY_COM1] || divisions_changed || active_radio_changed;
	display_data_changed[DISPLAY_COM1_STBY] = copy_if_changed(&instrument_state.external.com1.freq.standby_index, &report->com1.freq.standby_index, sizeof(instrument_state.external.com1.freq.standby_index)) || display_data_changed[DISPLAY_COM1_STBY] || divisions_changed;
	display_data_changed[DISPLAY_COM2] = copy_if_changed(&instrument_state.external.com2.freq.active_index, &report->com2.freq.active_index, sizeof(instrument_state.external.com2.freq.active_index)) || display_data_changed[DISPLAY_COM2] || divisions_changed || active_radio_changed;
	display_data_changed[DISPLAY_COM2_STBY] = copy_if_changed(&instrument_state.external.com2.freq.standby_index, &report->com2.freq.standby_index, sizeof(instrument_state.external.com2.freq.standby_index)) || display_data_changed[DISPLAY_COM2_STBY] || divisions_changed;

	display_data_changed[DISPLAY_NAV1] = copy_if_changed(&instrument_state.external.nav1.freq.active_index, &report->nav1.freq.active_index, sizeof(instrument_state.external.nav1.freq.active_index)) || display_data_changed[DISPLAY_NAV1] || divisions_changed;
	display_data_changed[DISPLAY_NAV1_STBY] = copy_if_changed(&instrument_state.external.nav1.freq.standby_index, &report->nav1.freq.standby_index, sizeof(instrument_state.external.nav1.freq.standby_index)) || display_data_changed[DISPLAY_NAV1_STBY] || divisions_changed;
	display_data_changed[DISPLAY_NAV1_STBY] = copy_if_changed(&instrument_state.external.nav1.obs, &report->nav1.obs, sizeof(instrument_state.external.nav1.obs)) || display_data_changed[DISPLAY_NAV1_STBY] || divisions_changed;
	display_data_changed[DISPLAY_NAV2] = copy_if_changed(&instrument_state.external.nav2.freq.active_index, &report->nav2.freq.active_index, sizeof(instrument_state.external.nav2.freq.active_index)) || display_data_changed[DISPLAY_NAV2] || divisions_changed;
	display_data_changed[DISPLAY_NAV2_STBY] = copy_if_changed(&instrument_state.external.nav2.freq.standby_index, &report->nav2.freq.standby_index, sizeof(instrument_state.external.nav2.freq.standby_index)) || display_data_changed[DISPLAY_NAV2_STBY] || divisions_changed;
	display_data_changed[DISPLAY_NAV2_STBY] = copy_if_changed(&instrument_state.external.nav2.obs, &report->nav2.obs, sizeof(instrument_state.external.nav2.obs)) || display_data_changed[DISPLAY_NAV2_STBY] || divisions_changed;
	if ((report->dme_nav_id == 1) || (report->dme_nav_id == 2)) {
		bool dme_nav_source_changed = copy_if_changed(&instrument_state.external.dme_nav_id, &report->dme_nav_id, sizeof(instrument_state.external.dme_nav_id));
		if (dme_nav_source_changed) {
			display_data_changed[DISPLAY_NAV1_STBY] = true;
			display_data_changed[DISPLAY_NAV2_STBY] = true;
		}
	}

	bool xpdr_state_changed = copy_if_changed(&instrument_state.external.xpdr, &report->xpdr, sizeof(instrument_state.external.xpdr));
	display_data_changed[DISPLAY_XPDR] = xpdr_state_changed || display_data_changed[DISPLAY_XPDR];
	led_state_changed = xpdr_state_changed || led_state_changed;
	display_data_changed[DISPLAY_ADF] = copy_if_changed(&instrument_state.external.adf, &report->adf, sizeof(instrument_state.external.adf)) || display_data_changed[DISPLAY_ADF];
	led_state_changed = (instrument_state.external.ap.state != report->ap.state) || led_state_changed;
	display_data_changed[DISPLAY_AP] = copy_if_changed(&instrument_state.external.ap, &report->ap, sizeof(instrument_state.external.ap)) || display_data_changed[DISPLAY_AP];
	instrument_state.external.qnh = report->qnh;
	led_state_changed = copy_if_changed(&instrument_state.external.navigate_by_gps, &report->navigate_by_gps, sizeof(instrument_state.external.navigate_by_gps)) || led_state_changed;

	rotary_setvalue(&rotary_com1.rotary, instrument_state.external.com1.freq.standby_index);
	rotary_setvalue(&rotary_com2.rotary, instrument_state.external.com2.freq.standby_index);
	rotary_setvalue(&rotary_atm.rotary, instrument_state.external.qnh);
	rotary_setvalue(&rotary_nav1.rotary, instrument_state.external.nav1.freq.standby_index);
	rotary_setvalue(&rotary_nav2.rotary, instrument_state.external.nav2.freq.standby_index);
	rotary_setvalue(&rotary_obs.rotary, (instrument_state.internal.active_obs == 0) ? instrument_state.external.nav1.obs : instrument_state.external.nav2.obs);
	rotary_setvalue(&rotary_dme_adf.rotary, instrument_state.external.adf.frequency_khz);
	rotary_setvalue(&rotary_ap_alt.rotary, instrument_state.external.ap.altitude);
	rotary_setvalue(&rotary_ap_hdg.rotary, instrument_state.external.ap.heading);
	rotary_setvalue(&rotary_ap_ias.rotary, instrument_state.external.ap.ias);
	rotary_setvalue(&rotary_ap_rate.rotary, instrument_state.external.ap.climbrate);
	instrument_state.external.seqno = report->seqno;
}

static void instruments_set_by_host_report03(const struct hid_set_report_03_t *report) {
	display_data_changed[DISPLAY_NAV1] = copy_if_changed(&instrument_state.internal.ident.nav1, &report->ident.nav1, sizeof(instrument_state.internal.ident.nav1)) || display_data_changed[DISPLAY_NAV1];
	display_data_changed[DISPLAY_NAV2] = copy_if_changed(&instrument_state.internal.ident.nav2, &report->ident.nav2, sizeof(instrument_state.internal.ident.nav2)) || display_data_changed[DISPLAY_NAV2];
	display_data_changed[DISPLAY_ADF] = copy_if_changed(&instrument_state.internal.ident.adf, &report->ident.adf, sizeof(instrument_state.internal.ident.adf)) || display_data_changed[DISPLAY_ADF];
	display_data_changed[DISPLAY_DME] = copy_if_changed(&instrument_state.internal.dme, &report->dme, sizeof(instrument_state.internal.dme)) || display_data_changed[DISPLAY_DME];
	instrument_state.internal.ident.nav1[IDENT_LENGTH_BYTES - 1] = 0;
	instrument_state.internal.ident.nav2[IDENT_LENGTH_BYTES - 1] = 0;
	instrument_state.internal.ident.adf[IDENT_LENGTH_BYTES - 1] = 0;
	instrument_state.external.seqno = report->seqno;
}

void instruments_set_by_host(const union hid_set_report_t *report) {
	if (report->generic.report_id == 0x02) {
		instruments_set_by_host_report02(&report->r02);
	} else if (report->generic.report_id == 0x03) {
		instruments_set_by_host_report03(&report->r03);
	} else {
		printf("Unknown report 0x%02x received.\n", report->generic.report_id);
	}
}

void instruments_init(void) {
	unblank();
	instrument_state.external.com_divisions = active_configuration.instruments.com_frequency_divisions;
	instrument_state.external.nav_divisions = active_configuration.instruments.nav_frequency_divisions;

	instrument_state.external.ap.ias = active_configuration.instruments.ap.ias;
	instrument_state.external.ap.altitude = active_configuration.instruments.ap.altitude;
	instrument_state.external.ap.climbrate = active_configuration.instruments.ap.climbrate;
	instrument_state.external.xpdr.squawk = active_configuration.instruments.squawk;

	instrument_state.external.com1.freq.active_index = frequency_khz_to_index(instrument_state.external.com_divisions, active_configuration.instruments.com1.active_frequency_khz);
	instrument_state.external.com1.freq.standby_index = frequency_khz_to_index(instrument_state.external.com_divisions, active_configuration.instruments.com1.standby_frequency_khz);
	instrument_state.external.com2.freq.active_index = frequency_khz_to_index(instrument_state.external.com_divisions, active_configuration.instruments.com2.active_frequency_khz);
	instrument_state.external.com2.freq.standby_index = frequency_khz_to_index(instrument_state.external.com_divisions, active_configuration.instruments.com2.standby_frequency_khz);
	instrument_state.external.nav1.freq.active_index = frequency_khz_to_index(instrument_state.external.nav_divisions, active_configuration.instruments.nav1.active_frequency_khz);
	instrument_state.external.nav1.freq.standby_index = frequency_khz_to_index(instrument_state.external.nav_divisions, active_configuration.instruments.nav1.standby_frequency_khz);
	instrument_state.external.nav2.freq.active_index = frequency_khz_to_index(instrument_state.external.nav_divisions, active_configuration.instruments.nav2.active_frequency_khz);
	instrument_state.external.nav2.freq.standby_index = frequency_khz_to_index(instrument_state.external.nav_divisions, active_configuration.instruments.nav2.standby_frequency_khz);

	rotary_com1.rotary.detent_cnt = frequency_detent_count(instrument_state.external.com_divisions);
	rotary_com2.rotary.detent_cnt = frequency_detent_count(instrument_state.external.com_divisions);
	rotary_nav1.rotary.detent_cnt = frequency_detent_count(instrument_state.external.nav_divisions);
	rotary_nav2.rotary.detent_cnt = frequency_detent_count(instrument_state.external.nav_divisions);
	rotary_setvalue(&rotary_ap_alt.rotary, instrument_state.external.ap.altitude);
	rotary_setvalue(&rotary_ap_ias.rotary, instrument_state.external.ap.ias);
	rotary_setvalue(&rotary_ap_rate.rotary, instrument_state.external.ap.climbrate);

	instrument_state.external.adf.frequency_khz = active_configuration.instruments.adf_frequency_khz;

	instrument_state.external.qnh = active_configuration.instruments.qnh;
	rotary_setvalue(&rotary_atm.rotary, instrument_state.external.qnh);

	instrument_state.external.tx_radio_id = active_configuration.instruments.tx_radio_id;
	instrument_state.external.dme_nav_id = 1;

	instrument_state.internal.display_mode.state = BOOTING;
	instrument_state.internal.display_mode.timer = 3000;

	led_state_changed = true;
	redraw_all_displays();
	report_via_usb = true;
}
