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

#ifndef __USB_HIDREPORT_H__
#define __USB_HIDREPORT_H__

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* IDENT is zero-terminated, i.e., only 4 characters available */
#define IDENT_LENGTH_BYTES			5

enum com_nav_range_t {
	COM_RANGE_5KHZ,
	COM_RANGE_25KHZ,
	NAV_RANGE,
};

enum ap_mode_t {
	AP_STATE_ACTIVE = (1 << 0),
	AP_STATE_BACKCOURSE = (1 << 1),
	AP_ALTITUDE_ARMED = (1 << 2),
	AP_ALTITUDE_HOLD = (1 << 3),
	AP_IAS_ARMED = (1 << 4),
	AP_IAS_HOLD = (1 << 5),
	AP_HEADING_ARMED = (1 << 6),
	AP_HEADING_HOLD = (1 << 7),
	AP_NAVIGATION_ARMED = (1 << 8),
	AP_NAVIGATION_HOLD = (1 << 9),
	AP_GLIDESLOPE_ARMED = (1 << 10),
	AP_GLIDESLOPE_HOLD = (1 << 11),
	AP_LOCALIZER_ARMED = (1 << 12),
	AP_LOCALIZER_HOLD = (1 << 13),
	AP_VERTICALSPEED_ARMED = (1 << 14),
	AP_VERTICALSPEED_HOLD = (1 << 15),
};
#define AP_VERTICAL_ARMED_MODES		(AP_ALTITUDE_ARMED | AP_GLIDESLOPE_ARMED | AP_VERTICALSPEED_ARMED)
#define AP_HORIZONTAL_ARMED_MODES	(AP_HEADING_ARMED | AP_NAVIGATION_ARMED |  AP_LOCALIZER_ARMED)
#define AP_ALL_ARMED_MODES			(AP_VERTICAL_ARMED_MODES | AP_HORIZONTAL_ARMED_MODES | AP_IAS_ARMED)

#define AP_VERTICAL_HOLD_MODES		(AP_ALTITUDE_HOLD | AP_GLIDESLOPE_HOLD | AP_VERTICALSPEED_HOLD)
#define AP_HORIZONTAL_HOLD_MODES	(AP_HEADING_HOLD | AP_NAVIGATION_HOLD |  AP_LOCALIZER_HOLD)
#define AP_ALL_HOLD_MODES			(AP_VERTICAL_HOLD_MODES | AP_HORIZONTAL_HOLD_MODES | AP_IAS_HOLD)

#define AP_VERTICAL_MODES			(AP_VERTICAL_ARMED_MODES | AP_VERTICAL_HOLD_MODES)
#define AP_HORIZONTAL_MODES			(AP_HORIZONTAL_ARMED_MODES | AP_HORIZONTAL_HOLD_MODES)
#define AP_ALL_MODES				(AP_ALL_ARMED_MODES | AP_ALL_HOLD_MODES)

enum radiopanel_button_t {
	RADIO_COM1 = (1 << 0),
	RADIO_NAV1 = (1 << 1),
	RADIO_DME = (1 << 2),
	RADIO_COM2 = (1 << 3),
	RADIO_NAV2 = (1 << 4),
	RADIO_ADF = (1 << 5),
};

enum xpdr_mode_t {
	XPDR_OFF = 0,
	XPDR_STANDBY = 1,
	XPDR_CHARLY = 2,
};

enum xpdr_state_element_t {
	XPDR_MODE_MASK = (3 << 0),
	XPDR_MODE_IDENTING = (1 << 2),
};

enum flipswitch_t {
	SWITCH_BCN = (1 << 0),
	SWITCH_LAND = (1 << 1),
	SWITCH_TAXI = (1 << 2),
	SWITCH_NAV	= (1 << 3),
	SWITCH_STRB = (1 << 4),
	SWITCH_MASTER = (1 << 5),
};

// Mingw __attribute__ ((packed)) is broken, need to use pragma instead
#pragma pack(1)

struct xcom_state_t {
	uint16_t active_index;
	uint16_t standby_index;
} __attribute__ ((packed));

struct com_state_t {
	struct xcom_state_t freq;
} __attribute__ ((packed));

struct nav_state_t {
	struct xcom_state_t freq;
	uint16_t obs;
} __attribute__ ((packed));

struct xpdr_state_t {
	uint8_t state;
	uint16_t squawk;
} __attribute__ ((packed));

struct adf_state_t {
	uint16_t frequency_khz;
} __attribute__ ((packed));

struct ap_state_t {
	uint16_t state;
	uint16_t altitude;
	uint16_t heading;
	uint16_t ias;
	int16_t climbrate;
} __attribute__ ((packed));

struct dme_info_t {
	bool available;
	uint16_t distance_tenth_nm;
	uint16_t velocity;
} __attribute__ ((packed));

struct hid_report_t {
	uint8_t report_id;
	uint8_t seqno;
	uint8_t radio_panel;
	uint8_t com_divisions;
	uint8_t nav_divisions;
	uint8_t tx_radio_id;
	uint8_t dme_nav_id;
	struct com_state_t com1, com2;
	struct nav_state_t nav1, nav2;
	struct xpdr_state_t xpdr;
	struct adf_state_t adf;
	struct ap_state_t ap;
	uint8_t flip_switches;
	uint16_t qnh;
	bool navigate_by_gps;
} __attribute__ ((packed));

//#define sassert(cond)			static_assert((cond), #cond)
//sassert(sizeof(struct hid_report_t) == 5 + (2 * 4) + (2 * 6) + 3 + 2 + 10 + 1 + 2 + 1);

struct hid_set_generic_report_t {
	uint8_t report_id;
	uint8_t seqno;
} __attribute__ ((packed));

struct hid_set_report_02_t {
	uint8_t report_id;
	uint8_t seqno;
	uint8_t radio_panel;
	uint8_t com_divisions;
	uint8_t nav_divisions;
	uint8_t tx_radio_id;
	uint8_t dme_nav_id;
	struct com_state_t com1, com2;
	struct nav_state_t nav1, nav2;
	struct xpdr_state_t xpdr;
	struct adf_state_t adf;
	struct ap_state_t ap;
	uint16_t qnh;
	bool navigate_by_gps;
} __attribute__ ((packed));
//sassert(sizeof(struct hid_set_report_02_t) == 5 + (2 * 4) + (2 * 6) + 3 + 2 + 10 + 2 + 1);

struct hid_set_report_03_t {
	uint8_t report_id;
	uint8_t seqno;
	struct {
		char nav1[IDENT_LENGTH_BYTES], nav2[IDENT_LENGTH_BYTES];
		char adf[IDENT_LENGTH_BYTES];
	} ident;
	struct dme_info_t dme;
	struct {
		uint16_t heading;
		uint16_t ias;
		uint16_t altitude;
	} plane_parameters;
} __attribute__ ((packed));

#pragma pack(0)

union hid_set_report_t {
	struct hid_set_generic_report_t generic;
	struct hid_set_report_02_t r02;
	struct hid_set_report_03_t r03;
};

#endif
