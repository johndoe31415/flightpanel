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

void dump_instrument_data(FILE *f, const struct instrument_data_t *data) {
#if defined(VARIANT_WINDOWS)
	system("cls");
#elif defined(VARIANT_LINUX_EMU) || defined(VARIANT_LINUX_XPLANE)
	fprintf(f, "\x1b[3;J\x1b[H\x1b[2J");
#endif
	fprintf(f, "VHF1: %3d.%03d (%3d.%03d)     ", data->vhf1.freq_active_khz / 1000, data->vhf1.freq_active_khz % 1000, data->vhf1.freq_standby_khz / 1000, data->vhf1.freq_standby_khz % 1000);
	fprintf(f, "VHF2: %3d.%03d (%3d.%03d)\n", data->vhf2.freq_active_khz / 1000, data->vhf2.freq_active_khz % 1000, data->vhf2.freq_standby_khz / 1000, data->vhf2.freq_standby_khz % 1000);
	fprintf(f, "NAV1: %3d.%03d (%3d.%03d)     ", data->nav1.freq_active_khz / 1000, data->nav1.freq_active_khz % 1000, data->nav1.freq_standby_khz / 1000, data->nav1.freq_standby_khz % 1000);
	fprintf(f, "NAV2: %3d.%03d (%3d.%03d)\n", data->nav2.freq_active_khz / 1000, data->nav2.freq_active_khz % 1000, data->nav2.freq_standby_khz / 1000, data->nav2.freq_standby_khz % 1000);
	fprintf(f, "OBS: NAV1 %3d deg, NAV2 %3d deg\n", data->nav1.obs, data->nav2.obs);
	fprintf(f, "ADF %4d.%d kHz %3d deg\n", data->adf.freq_hz / 1000, data->adf.freq_hz % 1000 / 100, data->adf.compass_rose);
	if (data->dme.available) {
		fprintf(f, "DME (NAV%d) distance %d.%d nm, %d kt\n", data->dme.nav_id, data->dme.distance_nm_tenths / 10, data->dme.distance_nm_tenths % 10, data->dme.speed_kt);
	} else {
		fprintf(f, "DME unavailable (NAV%d)\n", data->dme.nav_id);
	}
	fprintf(f, "Squawk %04d\n", data->xpdr.squawk);
	fprintf(f, "Radios: %1s%4s  %1s%4s  Audio: %4s %4s %3s %3s\n", data->vhf1.tx ? ">" : "", data->vhf1.rx ? "VHF1" : "", data->vhf2.tx ? ">" : "", data->vhf2.rx ? "VHF2" : "",
			data->nav1.sound ? "NAV1" : "", data->nav2.sound ? "NAV2" : "",
			data->dme.sound ? "DME" : "", data->adf.sound ? "ADF" : "");
	const char *dme_ident = (data->dme.nav_id == 1) ? data->nav1.ident : data->nav2.ident;
	fprintf(f, "Ident: NAV1 (%.8s), NAV2 (%.8s), ADF (%.8s), DME (%.8s)\n", data->nav1.ident, data->nav2.ident, data->adf.ident, dme_ident);
	fprintf(f, "Lights: %3s %4s %4s %3s %4s\n", data->lights.beacon ? "BCN" : "", data->lights.landing ? "LAND" : "", data->lights.taxi ? "TAXI" : "", data->lights.nav ? "NAV" : "", data->lights.strobe ? "STRB" : "");
	fprintf(f, "AP: %3s, %u ft, %d ft/min, %u deg, %u kts [%3s %3s %3s %3s %3s %3s]\n", data->ap.active ? "ON" : "off", data->ap.altitude_ft, data->ap.climbrate_ft_per_min, data->ap.heading, data->ap.speed_kts,
			data->ap.hdg_hold ? "HDG" : "", data->ap.nav_hold ? "NAV" : "", data->ap.alt_hold ? "ALT" : "", data->ap.rev_hold ? "REV" : "", data->ap.apr_hold ? "APR" : "", data->ap.ias_hold ? "IAS" : "");
	fprintf(f, "IAS %d kt, IALT %d ft, QNH %d mBar, Guide %s\n", data->misc.ias_kt, data->misc.indicated_alt_ft, data->misc.qnh_millibar, data->misc.guide_gps ? "GPS" : "NAV");
}

void fsconnection_incoming_data(const struct instrument_data_t *data) {
	dump_instrument_data(stdout, data);
}
