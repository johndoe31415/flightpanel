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
#include <stdlib.h>
#include "fsconnection.hpp"

static const char *xpdr_state_to_str(enum xpdr_mode_t xpdr_mode) {
	switch (xpdr_mode) {
		case XPDR_OFF: return "off";
		case XPDR_STANDBY: return "standby";
		case XPDR_CHARLY: return "charly";
	}
	return "?";
}

void dump_instrument_data(FILE *f, const struct instrument_data_t *data) {
	fprintf(f, "COM1: %3d.%03d (%3d.%03d)     ", data->com1_active_freq_khz() / 1000, data->com1_active_freq_khz() % 1000, data->com1_standby_freq_khz() / 1000, data->com1_standby_freq_khz() % 1000);
	fprintf(f, "COM2: %3d.%03d (%3d.%03d)\n", data->com2_active_freq_khz() / 1000, data->com2_active_freq_khz() % 1000, data->com2_standby_freq_khz() / 1000, data->com2_standby_freq_khz() % 1000);
	fprintf(f, "NAV1: %3d.%03d (%3d.%03d)     ", data->nav1_active_freq_khz() / 1000, data->nav1_active_freq_khz() % 1000, data->nav1_standby_freq_khz() / 1000, data->nav1_standby_freq_khz() % 1000);
	fprintf(f, "NAV2: %3d.%03d (%3d.%03d)\n", data->nav2_active_freq_khz() / 1000, data->nav2_active_freq_khz() % 1000, data->nav2_standby_freq_khz() / 1000, data->nav2_standby_freq_khz() % 1000);
	fprintf(f, "OBS: NAV1 %3d deg, NAV2 %3d deg\n", data->external.nav1.obs, data->external.nav2.obs);
	fprintf(f, "ADF %4d kHz\n", data->external.adf.frequency_khz);
	fprintf(f, "DME NAV %d\n", data->external.dme_nav_source);
	fprintf(f, "Squawk %04d, XPDR %s\n", data->external.xpdr.squawk, xpdr_state_to_str((enum xpdr_mode_t)data->external.xpdr.state));
	fprintf(f, "Radios: %1s%4s  %1s%4s  Audio: %4s %4s %3s %3s\n",
			(data->external.tx_radio_id == 1) ? ">" : "", (data->external.radio_panel & RADIO_COM1) ? "COM1" : "",
			(data->external.tx_radio_id == 2) ? ">" : "", (data->external.radio_panel & RADIO_COM2) ? "COM2" : "",
			(data->external.radio_panel & RADIO_NAV1) ? "NAV1" : "",
			(data->external.radio_panel & RADIO_NAV2) ? "NAV2" : "",
			(data->external.radio_panel & RADIO_DME) ? "DME" : "",
			(data->external.radio_panel & RADIO_ADF) ? "ADF" : "");
	fprintf(f, "Switches: %3s %4s %4s %3s %4s %6s\n",
			(data->external.flip_switches & SWITCH_BCN) ? "BCN" : "",
			(data->external.flip_switches & SWITCH_LAND) ? "LAND" : "",
			(data->external.flip_switches & SWITCH_TAXI) ? "TAXI" : "",
			(data->external.flip_switches & SWITCH_NAV) ? "NAV" : "",
			(data->external.flip_switches & SWITCH_STRB) ? "STRB" : "",
			(data->external.flip_switches & SWITCH_MASTER) ? "MASTER" : ""
		);
	fprintf(f, "AP: %3s, %u ft, %d ft/min, %u deg, %u kts [%3s %3s %3s %3s %3s %3s]\n",
			data->external.ap.state & AP_ACTIVE ? "ON" : "off",
			data->external.ap.altitude,
			data->external.ap.climbrate,
			data->external.ap.heading,
			data->external.ap.ias,
			data->external.ap.state & AP_HOLD_ALTITUDE ? "ALT" : "",
			data->external.ap.state & AP_HOLD_HEADING ? "HDG" : "",
			data->external.ap.state & AP_HOLD_IAS ? "IAS" : "",
			data->external.ap.state & AP_HOLD_NAVIGATION ? "NAV" : "",
			data->external.ap.state & AP_HOLD_APPROACH ? "APR" : "",
			data->external.ap.state & AP_HOLD_REVERSE ? "REV" : ""
	);
}

