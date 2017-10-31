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

#ifndef __SIMCONNECT_DATADEFS_HPP__
#define __SIMCONNECT_DATADEFS_HPP__

#include <stdint.h>

struct simconnect_datatype_information_t {
	char title[256];
} __attribute__((packed));
static_assert(sizeof(struct simconnect_datatype_information_t) == 256, "struct simconnect_datatype_information_t incorrectly packed.");
#define SIMCONNECT_DATATYPE_INFORMATION_DATUM_COUNT			1

struct simconnect_datatype_instruments_t {
	char nav1_ident[8];
	double nav1_obs_deg;
	char nav2_ident[8];
	double nav2_obs_deg;
	char adf_ident[8];
	double adf_compass_rose_deg;
	double dme1_speed;
	double dme1_distance;
	double dme2_speed;
	double dme2_distance;
	double ap_heading_deg;
	double ap_altitude_ft;
	double ap_climbrate_ft_per_min;
	double ias;
	double indicated_alt_ft;
	double qnh_millibar;
	int32_t com1_freq_active;
	int32_t com1_freq_standby;
	int32_t com1_tx;
	int32_t com2_freq_active;
	int32_t com2_freq_standby;
	int32_t com2_tx;
	int32_t com_all_rx;
	int32_t nav1_freq_active;
	int32_t nav1_freq_standby;
	int32_t nav1_sound;
	int32_t nav2_freq_active;
	int32_t nav2_freq_standby;
	int32_t nav2_sound;
	int32_t adf_freq_active;
	int32_t adf_sound;
	int32_t dme_sound;
	int32_t dme_selected;
	int32_t dme_nav1;
	int32_t dme_nav2;
	int32_t xpdr_squawk;
	int32_t light_states;
	int32_t ap_master;
	int32_t ap_hdg_hold;
	int32_t ap_nav_hold;
	int32_t ap_alt_hold;
	int32_t ap_rev_hold;
	int32_t ap_apr_hold;
	int32_t ap_ias_hold;
	int32_t gps_drives_nav;
} __attribute__((packed));
static_assert(sizeof(struct simconnect_datatype_instruments_t) == 244, "struct simconnect_datatype_instruments_t incorrectly packed.");
#define SIMCONNECT_DATATYPE_INSTRUMENTS_DATUM_COUNT			45


void simconnect_datadefs_register_information(void *hSimConnect, int group_id);
void simconnect_datadefs_register_instruments(void *hSimConnect, int group_id);

#endif
