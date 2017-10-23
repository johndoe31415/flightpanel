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

#include <iostream>
#include <cstring>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stddef.h>

#include "arbiter.hpp"

enum arbitration_type_t {
	ARBITRATION_UINT32_T
};

struct arbitration_t {
	const char *name;
	enum arbitration_type_t arbitration_type;
	int instrument_offset;
	int selection_offset;
};

static const struct arbitration_t arbitration[] = {
	/*
	{ "COM1 active frequency", ARBITRATION_UINT32_T, offsetof(struct instrument_data_t, com1.freq_active_khz), offsetof(struct component_selection_t, com1_active) },
	{ "COM1 standby frequency", ARBITRATION_UINT32_T, offsetof(struct instrument_data_t, com1.freq_standby_khz), offsetof(struct component_selection_t, com1_standby) },
	{ "COM2 active frequency", ARBITRATION_UINT32_T, offsetof(struct instrument_data_t, com2.freq_active_khz), offsetof(struct component_selection_t, com2_active) },
	{ "COM2 standby frequency", ARBITRATION_UINT32_T, offsetof(struct instrument_data_t, com2.freq_standby_khz), offsetof(struct component_selection_t, com2_standby) },
	{ "NAV1 active frequency", ARBITRATION_UINT32_T, offsetof(struct instrument_data_t, nav1.freq_active_khz), offsetof(struct component_selection_t, nav1_active) },
	{ "NAV1 standby frequency", ARBITRATION_UINT32_T, offsetof(struct instrument_data_t, nav1.freq_standby_khz), offsetof(struct component_selection_t, nav1_standby) },
	{ "NAV2 active frequency", ARBITRATION_UINT32_T, offsetof(struct instrument_data_t, nav2.freq_active_khz), offsetof(struct component_selection_t, nav2_active) },
	{ "NAV2 standby frequency", ARBITRATION_UINT32_T, offsetof(struct instrument_data_t, nav2.freq_standby_khz), offsetof(struct component_selection_t, nav2_standby) },
	*/
};

Arbiter::Arbiter(FSConnection *fs_connection, FPConnection *fp_connection) {
	_fs_connection = fs_connection;
	_fp_connection = fp_connection;
	std::memset(&_last_fs_data, 0, sizeof(struct instrument_data_t));

//	std::memset(&_last_fp_data, 0, sizeof(struct instrument_data_t));
}

#if 0
template<typename T> void Arbiter::arbitrate_value(const struct instrument_data_t &new_fs_data, const struct instrument_data_t &new_fp_data, const struct arbitration_t &entry) {
	T fs_value = new_fs_data.get_value<T>(entry.instrument_offset);
	T fp_value = new_fp_data.get_value<T>(entry.instrument_offset);
	T last_fs_value = _last_fs_data.get_value<T>(entry.instrument_offset);
	T last_fp_value = _last_fp_data.get_value<T>(entry.instrument_offset);

	if (fs_value != fp_value) {
		/* Arbitration needed, instruments differ */
		bool fs_changed = (fs_value != last_fs_value);
		bool fp_changed = (fp_value != last_fp_value);
		bool nothing_changed = !fs_changed && !fp_changed;
		if (nothing_changed || fs_changed) {
			/* If flight simulator, both instruments or no instruments changed,
			 * sync Flightsim -> Flightpanel */
			std::cerr << "Arbitrating FS->FP: " << entry.name << " " << fp_value << " ~> " << fs_value << std::endl;
			_put_fp_data.set_value(entry.instrument_offset, fs_value);
			_put_fp_selection.set_flag(entry.selection_offset);
		} else {
			/* Otherwise sync Flightpanel -> Flightsim (i.e., only when
			 * Flightpanel has changed) */
			std::cerr << "Arbitrating FP->FS: " << entry.name << " " << fs_value << " ~> " << fp_value << std::endl;
			_put_fs_data.set_value(entry.instrument_offset, fp_value);
			_put_fs_selection.set_flag(entry.selection_offset);
		}
	}
}
#endif

template<typename T> void Arbiter::arbitrate_value(bool *changed, const T &old_fs_data, const T &new_fs_data, const T &old_fp_data, const T &new_fp_data, T *authoritative_data) {
	bool fs_changed = memcmp(&old_fs_data, &new_fs_data, sizeof(T)) != 0;
	bool fp_changed = memcmp(&old_fp_data, &new_fp_data, sizeof(T)) != 0;
	if (fs_changed) {
		*authoritative_data = new_fs_data;
		*changed = true;
	} else if (fp_changed) {
		*authoritative_data = new_fp_data;
		*changed = true;
	} else {
		*authoritative_data = new_fs_data;
	}
}

void Arbiter::arbitrate(const struct instrument_data_t &new_fs_data, const struct instrument_data_t &new_fp_data) {
	bool send_report_01, send_report_02;
	struct instrument_data_t authoritative_data;
	memset(&authoritative_data, 0, sizeof(authoritative_data));

	send_report_01 = false;
	arbitrate_value(&send_report_01, _last_fs_data.external.radio_panel, new_fs_data.external.radio_panel, _last_fp_data.external.radio_panel, new_fp_data.external.radio_panel, &authoritative_data.external.radio_panel);
	arbitrate_value(&send_report_01, _last_fs_data.external.com_divisions, new_fs_data.external.com_divisions, _last_fp_data.external.com_divisions, new_fp_data.external.com_divisions, &authoritative_data.external.com_divisions);
	arbitrate_value(&send_report_01, _last_fs_data.external.nav_divisions, new_fs_data.external.nav_divisions, _last_fp_data.external.nav_divisions, new_fp_data.external.nav_divisions, &authoritative_data.external.nav_divisions);
	arbitrate_value(&send_report_01, _last_fs_data.external.com1, new_fs_data.external.com1, _last_fp_data.external.com1, new_fp_data.external.com1, &authoritative_data.external.com1);
	arbitrate_value(&send_report_01, _last_fs_data.external.com2, new_fs_data.external.com2, _last_fp_data.external.com2, new_fp_data.external.com2, &authoritative_data.external.com2);
	arbitrate_value(&send_report_01, _last_fs_data.external.nav1, new_fs_data.external.nav1, _last_fp_data.external.nav1, new_fp_data.external.nav1, &authoritative_data.external.nav1);
	arbitrate_value(&send_report_01, _last_fs_data.external.nav2, new_fs_data.external.nav2, _last_fp_data.external.nav2, new_fp_data.external.nav2, &authoritative_data.external.nav2);
	arbitrate_value(&send_report_01, _last_fs_data.external.xpdr, new_fs_data.external.xpdr, _last_fp_data.external.xpdr, new_fp_data.external.xpdr, &authoritative_data.external.xpdr);
	arbitrate_value(&send_report_01, _last_fs_data.external.adf, new_fs_data.external.adf, _last_fp_data.external.adf, new_fp_data.external.adf, &authoritative_data.external.adf);
	arbitrate_value(&send_report_01, _last_fs_data.external.ap, new_fs_data.external.ap, _last_fp_data.external.ap, new_fp_data.external.ap, &authoritative_data.external.ap);
	arbitrate_value(&send_report_01, _last_fs_data.external.navigate_by_gps, new_fs_data.external.navigate_by_gps, _last_fp_data.external.navigate_by_gps, new_fp_data.external.navigate_by_gps, &authoritative_data.external.navigate_by_gps);

	send_report_02 = false;

	if (send_report_01) {
		dump_instrument_data(stderr, &authoritative_data);
		_fp_connection->put_data(&authoritative_data, NULL);
	}


}

void Arbiter::run() {
	bool first = true;
	while (true) {
		fprintf(stderr, "arbiter!\n");
		struct instrument_data_t new_fs_data;
		_fs_connection->get_data(&new_fs_data);

		struct instrument_data_t new_fp_data;
		_fp_connection->get_data(&new_fp_data);

		if (first) {
			_fp_connection->put_data(&new_fs_data, NULL);
			first = false;
		} else {
			arbitrate(new_fs_data, new_fp_data);
		}

		_last_fs_data = new_fs_data;
		_last_fp_data = new_fp_data;
		sleep(1);
	}
}

