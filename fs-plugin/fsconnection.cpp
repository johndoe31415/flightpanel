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
#include <string.h>
#include "fsconnection.hpp"

static const char *xpdr_state_to_str(enum xpdr_mode_t xpdr_mode) {
	switch (xpdr_mode) {
		case XPDR_OFF: return "off";
		case XPDR_STANDBY: return "standby";
		case XPDR_CHARLY: return "charly";
	}
	return "?";
}

static bool dump_instrument_data_line(int line, char buffer[256], const struct instrument_data_t &data) {
	char *buf = buffer;

	switch (line) {
		case 0:
			buf += sprintf(buf, "COM1: %3d.%03d (%3d.%03d)     ", data.com1_active_freq_khz() / 1000, data.com1_active_freq_khz() % 1000, data.com1_standby_freq_khz() / 1000, data.com1_standby_freq_khz() % 1000);
			buf += sprintf(buf, "COM2: %3d.%03d (%3d.%03d)", data.com2_active_freq_khz() / 1000, data.com2_active_freq_khz() % 1000, data.com2_standby_freq_khz() / 1000, data.com2_standby_freq_khz() % 1000);
			break;

		case 1:
			buf += sprintf(buf, "NAV1: %3d.%03d (%3d.%03d)     ", data.nav1_active_freq_khz() / 1000, data.nav1_active_freq_khz() % 1000, data.nav1_standby_freq_khz() / 1000, data.nav1_standby_freq_khz() % 1000);
			buf += sprintf(buf, "NAV2: %3d.%03d (%3d.%03d)", data.nav2_active_freq_khz() / 1000, data.nav2_active_freq_khz() % 1000, data.nav2_standby_freq_khz() / 1000, data.nav2_standby_freq_khz() % 1000);
			break;

		case 2:
			buf += sprintf(buf, "OBS: NAV1 %3d deg, NAV2 %3d deg", data.external.nav1.obs, data.external.nav2.obs);
			break;

		case 3:
			buf += sprintf(buf, "ADF %4d kHz", data.external.adf.frequency_khz);
			break;

		case 4:
			buf += sprintf(buf, "DME %-3s dist %5.1f nm, speed %3d kt", data.internal.dme.available ? "yes" : "no", (double)data.internal.dme.distance_tenth_nm / 10, data.internal.dme.velocity);
			break;

		case 5:
			buf += sprintf(buf, "Squawk %04d, XPDR %s", data.external.xpdr.squawk, xpdr_state_to_str((enum xpdr_mode_t)data.external.xpdr.state));
			break;

		case 6:
			buf += sprintf(buf, "Radios: %1s%4s  %1s%4s  Audio: %4s %4s %3s %3s",
					(data.external.tx_radio_id == 1) ? ">" : "", (data.external.radio_panel & RADIO_COM1) ? "COM1" : "",
					(data.external.tx_radio_id == 2) ? ">" : "", (data.external.radio_panel & RADIO_COM2) ? "COM2" : "",
					(data.external.radio_panel & RADIO_NAV1) ? "NAV1" : "",
					(data.external.radio_panel & RADIO_NAV2) ? "NAV2" : "",
					(data.external.radio_panel & RADIO_DME) ? "DME" : "",
					(data.external.radio_panel & RADIO_ADF) ? "ADF" : "");
			break;

		case 7:
			buf += sprintf(buf, "Switches: %3s %4s %4s %3s %4s %6s",
					(data.external.flip_switches & SWITCH_BCN) ? "BCN" : "",
					(data.external.flip_switches & SWITCH_LAND) ? "LAND" : "",
					(data.external.flip_switches & SWITCH_TAXI) ? "TAXI" : "",
					(data.external.flip_switches & SWITCH_NAV) ? "NAV" : "",
					(data.external.flip_switches & SWITCH_STRB) ? "STRB" : "",
					(data.external.flip_switches & SWITCH_MASTER) ? "MASTER" : ""
				);
			break;

		case 8:
			buf += sprintf(buf, "AP: %3s, %5u ft, %4d ft/min, %3u', %3u kts",
					data.external.ap.state & AP_STATE_ACTIVE ? "ON" : "off",
					data.external.ap.altitude,
					data.external.ap.climbrate,
					data.external.ap.heading,
					data.external.ap.ias
			);
			break;

		case 9:
			buf += sprintf(buf, "AP: [%3s %3s %3s %2s %3s %3s %2s]",
					data.external.ap.state & AP_ALTITUDE_ARMED ? "alt" : (data.external.ap.state & AP_ALTITUDE_HOLD ? "ALT" : ""),
					data.external.ap.state & AP_HEADING_ARMED ? "hdg" : (data.external.ap.state & AP_HEADING_HOLD ? "HDG" : ""),
					data.external.ap.state & AP_IAS_ARMED ? "ias" : (data.external.ap.state & AP_IAS_HOLD ? "IAS" : ""),
					data.external.ap.state & AP_VERTICALSPEED_ARMED ? "vs" : (data.external.ap.state & AP_VERTICALSPEED_HOLD ? "VS" : ""),
					data.external.ap.state & AP_NAVIGATION_ARMED ? "nav" : (data.external.ap.state & AP_NAVIGATION_HOLD ? "NAV" : ""),
					data.external.ap.state & AP_LOCALIZER_ARMED ? "loc" : (data.external.ap.state & AP_LOCALIZER_HOLD ? "LOC" : ""),
					data.external.ap.state & AP_GLIDESLOPE_ARMED ? "gs" : (data.external.ap.state & AP_GLIDESLOPE_HOLD ? "GS" : "")
			);
			break;

		case 10:
			buf += sprintf(buf, "NAV source: %s   QNH %4d", data.external.navigate_by_gps ? "GPS" : "NAV", data.external.qnh);
			break;

		case 11:
			buf += sprintf(buf, "IDENTs: NAV1 %4s, NAV2 %4s, ADF %4s", data.internal.ident.nav1, data.internal.ident.nav2, data.internal.ident.adf);
			break;

		default:
			return false;
	}
	return true;
}

void dump_instrument_data(FILE *f, const char *desc, const struct instrument_data_t &data) {
	int lineno = 0;
	char buffer[256];
	fprintf(f, "%s:\n", desc);
	while (dump_instrument_data_line(lineno, buffer, data)) {
		fprintf(f, "%s\n", buffer);
		lineno++;
	}
	fprintf(f, "\n");
}

void diff_instrument_data(FILE *f, const char *desc, const struct instrument_data_t &data1, const struct instrument_data_t &data2, bool show_everything) {
	int lineno = 0;
	char buffer1[256], buffer2[256];
	fprintf(f, "%s:\n", desc);
	bool difference = false;
	while (dump_instrument_data_line(lineno, buffer1, data1)) {
		dump_instrument_data_line(lineno, buffer2, data2);
		lineno++;

		if ((!show_everything) && (!strcmp(buffer1, buffer2))) {
			continue;
		}

		difference = true;
		int len1 = strlen(buffer1);
		int len2 = strlen(buffer2);
		int min_len = len1 < len2 ? len1 : len2;

		bool red = false;
		for (int i = 0; i < min_len; i++) {
			bool char_red = (buffer1[i] != buffer2[i]);
			if (char_red != red) {
				if (char_red) {
					fprintf(f, "\x1b[31m");
				} else {
					fprintf(f, "\x1b[0m");
				}
				red = char_red;
			}

			fprintf(f, "%c", buffer1[i]);
		}
		fprintf(f, "\x1b[0m");
		fprintf(f, "%s", buffer1 + min_len);
		fprintf(f, "%*s", 60 - len1, "");

		red = false;
		for (int i = 0; i < min_len; i++) {
			bool char_red = (buffer1[i] != buffer2[i]);
			if (char_red != red) {
				if (char_red) {
					fprintf(f, "\x1b[31m");
				} else {
					fprintf(f, "\x1b[0m");
				}
				red = char_red;
			}
			fprintf(f, "%c", buffer2[i]);
		}
		fprintf(f, "\x1b[0m");
		fprintf(f, "%s", buffer2 + min_len);
		fprintf(f, "\n");
	}
	if ((!show_everything) && (!difference)) {
		fprintf(f, "No differences.\n");
	}
	fprintf(f, "\n");

}
