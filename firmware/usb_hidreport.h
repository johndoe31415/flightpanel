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

enum ap_mode_t {
	AP_ACTIVE = (1 << 0),
	AP_HOLD_ALTITUDE = (1 << 1),
	AP_HOLD_IAS = (1 << 2),
	AP_HOLD_HEADING = (1 << 3),
	AP_HOLD_NAVIGATION = (1 << 4),
	AP_HOLD_REVERSE = (1 << 5),
	AP_HOLD_APPROACH = (1 << 6),
};

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
	SWITCH_MASTER = (1 << 0),
	SWITCH_BCN = (1 << 1),
	SWITCH_LAND = (1 << 2),
	SWITCH_TAXI = (1 << 3),
	SWITCH_NAV	= (1 << 4),
	SWITCH_STRB = (1 << 5),
};

struct com_nav_state_t {
	uint16_t active_index;
	uint16_t standby_index;
};

struct xpdr_state_t {
	uint8_t state;
	uint16_t squawk;
};

struct adf_state_t {
	uint16_t frequency_khz;
};

struct ap_state_t {
	uint8_t state;
	uint16_t altitude;
	uint16_t climbrate;
	uint16_t ias;
	uint16_t heading;
};

struct hid_report_t {
	uint8_t radio_panel;
	uint8_t com_raster:4, nav_raster:4;
	struct com_nav_state_t com1, com2;
	struct com_nav_state_t nav1, nav2;
	struct xpdr_state_t xpdr;
	struct adf_state_t adf;
	struct ap_state_t ap;
	uint8_t flip_switches;
	uint16_t qnh;
	bool navigate_by_gps;
} __attribute__ ((packed));

struct hid_set_report_t {
	uint8_t report_id;
	uint16_t com1_active, com1_standby;
	uint16_t com2_active, com2_standby;
	uint16_t nav1_active, nav1_standby;
	uint16_t nav2_active, nav2_standby;
	uint16_t squawk;
	uint16_t ap_state;
} __attribute__ ((packed));


struct hid_set_report_01_t {
	uint8_t report_id;
	uint16_t ias;
	uint16_t altitude;
} __attribute__ ((packed));

#endif

