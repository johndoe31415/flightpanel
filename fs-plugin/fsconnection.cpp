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
	fprintf(f, "COM1: %3d.%03d (%3d.%03d)     ", data->com1_active_freq_khz() / 1000, data->com1_active_freq_khz() % 1000, data->com1_standby_freq_khz() / 1000, data->com1_standby_freq_khz() % 1000);
	fprintf(f, "COM2: %3d.%03d (%3d.%03d)\n", data->com2_active_freq_khz() / 1000, data->com2_active_freq_khz() % 1000, data->com2_standby_freq_khz() / 1000, data->com2_standby_freq_khz() % 1000);
	fprintf(f, "NAV1: %3d.%03d (%3d.%03d)     ", data->nav1_active_freq_khz() / 1000, data->nav1_active_freq_khz() % 1000, data->nav1_standby_freq_khz() / 1000, data->nav1_standby_freq_khz() % 1000);
	fprintf(f, "NAV2: %3d.%03d (%3d.%03d)\n", data->nav2_active_freq_khz() / 1000, data->nav2_active_freq_khz() % 1000, data->nav2_standby_freq_khz() / 1000, data->nav2_standby_freq_khz() % 1000);
#if 0
	fprintf(f, "OBS: NAV1 %3d deg, NAV2 %3d deg\n", data->external.nav1.obs, data->external.nav2.obs);
//	fprintf(f, "ADF %4d kHz %3d deg\n", data->external.adf.freq_khz, data->adf.compass_rose);
	fprintf(f, "ADF %4d kHz ??? deg\n", data->external.adf.freq_khz);
	if (data->external.dme.available) {
		fprintf(f, "DME (NAV%d) distance %d.%d nm, %d kt\n", data->dme.nav_id, data->dme.distance_nm_tenths / 10, data->dme.distance_nm_tenths % 10, data->dme.speed_kt);
	} else {
		fprintf(f, "DME unavailable (NAV%d)\n", data->dme.nav_id);
	}
	fprintf(f, "Squawk %04d\n", data->xpdr.squawk);
	fprintf(f, "Radios: %1s%4s  %1s%4s  Audio: %4s %4s %3s %3s\n", data->com1.tx ? ">" : "", data->com1.rx ? "VHF1" : "", data->com2.tx ? ">" : "", data->com2.rx ? "VHF2" : "",
			data->external.nav1.sound ? "NAV1" : "", data->external.nav2.sound ? "NAV2" : "",
			data->dme.sound ? "DME" : "", data->adf.sound ? "ADF" : "");
	const char *dme_ident = (data->dme.nav_id == 1) ? data->external.nav1.ident : data->external.nav2.ident;
	fprintf(f, "Ident: NAV1 (%.8s), NAV2 (%.8s), ADF (%.8s), DME (%.8s)\n", data->external.nav1.ident, data->external.nav2.ident, data->adf.ident, dme_ident);
	fprintf(f, "Lights: %3s %4s %4s %3s %4s\n", data->lights.beacon ? "BCN" : "", data->lights.landing ? "LAND" : "", data->lights.taxi ? "TAXI" : "", data->lights.nav ? "NAV" : "", data->lights.strobe ? "STRB" : "");
	fprintf(f, "AP: %3s, %u ft, %d ft/min, %u deg, %u kts [%3s %3s %3s %3s %3s %3s]\n", data->ap.active ? "ON" : "off", data->ap.altitude_ft, data->ap.climbrate_ft_per_min, data->ap.heading, data->ap.speed_kts,
			data->ap.hdg_hold ? "HDG" : "", data->ap.nav_hold ? "NAV" : "", data->ap.alt_hold ? "ALT" : "", data->ap.rev_hold ? "REV" : "", data->ap.apr_hold ? "APR" : "", data->ap.ias_hold ? "IAS" : "");
	fprintf(f, "IAS %d kt, IALT %d ft, QNH %d mBar, Guide %s\n", data->misc.ias_kt, data->misc.indicated_alt_ft, data->misc.qnh_millibar, data->misc.guide_gps ? "GPS" : "NAV");
	fprintf(f, "\n");
#endif
}

