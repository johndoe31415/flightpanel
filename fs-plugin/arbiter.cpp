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
#include <typeinfo>
#include <cstring>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stddef.h>

#include "arbiter.hpp"

Arbiter::Arbiter(FSConnection *fs_connection, FPConnection *fp_connection) : Thread(100) {
	_first_sync = true;
	_fs_connection = fs_connection;
	_fp_connection = fp_connection;
	std::memset(&_last_authoritative_data, 0, sizeof(struct instrument_data_t));
}

template<typename T> void Arbiter::arbitrate_value(bool *update_fs, bool *update_fp, const T &old_authoritative_data, const T &new_fs_data, const T &new_fp_data, T *authoritative_data) {
	bool fs_changed = memcmp(&old_authoritative_data, &new_fs_data, sizeof(T)) != 0;
	bool fp_changed = memcmp(&old_authoritative_data, &new_fp_data, sizeof(T)) != 0;
	if (fs_changed) {
		*authoritative_data = new_fs_data;
		*update_fp = true;
	} else if (fp_changed) {
		*authoritative_data = new_fp_data;
		*update_fs = true;
	} else {
		*authoritative_data = new_fs_data;
	}
}

template<typename T> void Arbiter::arbitrate_value_unidirectional(bool *update, const T *old_data, const T *new_data, unsigned int data_size, T *authoritative_data) {
	bool changed = memcmp(old_data, new_data, data_size) != 0;
	memcpy(authoritative_data, new_data, data_size);
	if (changed) {
		*update = true;
	}
}



struct arbiter_result_t Arbiter::arbitrate(const struct instrument_data_t &new_fs_data, const struct instrument_data_t &new_fp_data) {
	struct arbiter_result_t result;
	memset(&result, 0, sizeof(result));

	struct instrument_data_t authoritative_data;
	memset(&authoritative_data, 0, sizeof(authoritative_data));

	arbitrate_value(&result.fs.radio_panel, &result.fp.radio_panel, _last_authoritative_data.external.radio_panel, new_fs_data.external.radio_panel, new_fp_data.external.radio_panel, &authoritative_data.external.radio_panel);
	arbitrate_value(&result.fs.divisions, &result.fp.divisions, _last_authoritative_data.external.com_divisions, new_fs_data.external.com_divisions, new_fp_data.external.com_divisions, &authoritative_data.external.com_divisions);
	arbitrate_value(&result.fs.divisions, &result.fp.divisions, _last_authoritative_data.external.nav_divisions, new_fs_data.external.nav_divisions, new_fp_data.external.nav_divisions, &authoritative_data.external.nav_divisions);
	arbitrate_value(&result.fs.tx_radio_id, &result.fp.tx_radio_id, _last_authoritative_data.external.tx_radio_id, new_fs_data.external.tx_radio_id, new_fp_data.external.tx_radio_id, &authoritative_data.external.tx_radio_id);
	arbitrate_value(&result.fs.com1, &result.fp.com1, _last_authoritative_data.external.com1, new_fs_data.external.com1, new_fp_data.external.com1, &authoritative_data.external.com1);
	arbitrate_value(&result.fs.com2, &result.fp.com2, _last_authoritative_data.external.com2, new_fs_data.external.com2, new_fp_data.external.com2, &authoritative_data.external.com2);
	arbitrate_value(&result.fs.nav1, &result.fp.nav1, _last_authoritative_data.external.nav1, new_fs_data.external.nav1, new_fp_data.external.nav1, &authoritative_data.external.nav1);
	arbitrate_value(&result.fs.nav2, &result.fp.nav2, _last_authoritative_data.external.nav2, new_fs_data.external.nav2, new_fp_data.external.nav2, &authoritative_data.external.nav2);
	arbitrate_value(&result.fs.xpdr, &result.fp.xpdr, _last_authoritative_data.external.xpdr, new_fs_data.external.xpdr, new_fp_data.external.xpdr, &authoritative_data.external.xpdr);
	arbitrate_value(&result.fs.adf, &result.fp.adf, _last_authoritative_data.external.adf, new_fs_data.external.adf, new_fp_data.external.adf, &authoritative_data.external.adf);

	//arbitrate_autopilot(&result.fs.ap, &result.fp.ap, _last_authoritative_data.external.ap, new_fs_data.external.ap, new_fp_data.external.ap, &authoritative_data.external.ap);
	arbitrate_value(&result.fs.ap, &result.fp.ap, _last_authoritative_data.external.ap.state, new_fs_data.external.ap.state, new_fp_data.external.ap.state, &authoritative_data.external.ap.state);
	arbitrate_value(&result.fs.ap, &result.fp.ap, _last_authoritative_data.external.ap.altitude, new_fs_data.external.ap.altitude, new_fp_data.external.ap.altitude, &authoritative_data.external.ap.altitude);
	arbitrate_value(&result.fs.ap, &result.fp.ap, _last_authoritative_data.external.ap.heading, new_fs_data.external.ap.heading, new_fp_data.external.ap.heading, &authoritative_data.external.ap.heading);
	if ((new_fs_data.internal.ap.ignore_values & IGNORE_IAS) == 0) {
		arbitrate_value(&result.fs.ap, &result.fp.ap, _last_authoritative_data.external.ap.ias, new_fs_data.external.ap.ias, new_fp_data.external.ap.ias, &authoritative_data.external.ap.ias);
	} else {
		/* Ignore values read-back from FS, always prefer FP */
		arbitrate_value_unidirectional(&result.fs.ap, &_last_authoritative_data.external.ap.ias, &new_fp_data.external.ap.ias, sizeof(new_fp_data.external.ap.ias), &authoritative_data.external.ap.ias);
	}
	if ((new_fs_data.internal.ap.ignore_values & IGNORE_IAS) == 0) {
		arbitrate_value(&result.fs.ap, &result.fp.ap, _last_authoritative_data.external.ap.climbrate, new_fs_data.external.ap.climbrate, new_fp_data.external.ap.climbrate, &authoritative_data.external.ap.climbrate);
	} else {
		/* Ignore values read-back from FS, always prefer FP */
		arbitrate_value_unidirectional(&result.fs.ap, &_last_authoritative_data.external.ap.climbrate, &new_fp_data.external.ap.climbrate, sizeof(new_fp_data.external.ap.climbrate), &authoritative_data.external.ap.climbrate);
	}


	arbitrate_value(&result.fs.qnh, &result.fp.qnh, _last_authoritative_data.external.qnh, new_fs_data.external.qnh, new_fp_data.external.qnh, &authoritative_data.external.qnh);
	arbitrate_value(&result.fs.navigate_by_gps, &result.fp.navigate_by_gps, _last_authoritative_data.external.navigate_by_gps, new_fs_data.external.navigate_by_gps, new_fp_data.external.navigate_by_gps, &authoritative_data.external.navigate_by_gps);

	/* IDENT values can only be written to flightpanel */
	arbitrate_value_unidirectional(&result.fp.ident_values, _last_authoritative_data.internal.ident.nav1, new_fs_data.internal.ident.nav1, IDENT_LENGTH_BYTES, authoritative_data.internal.ident.nav1);
	arbitrate_value_unidirectional(&result.fp.ident_values, _last_authoritative_data.internal.ident.nav2, new_fs_data.internal.ident.nav2, IDENT_LENGTH_BYTES, authoritative_data.internal.ident.nav2);
	arbitrate_value_unidirectional(&result.fp.ident_values, _last_authoritative_data.internal.ident.adf, new_fs_data.internal.ident.adf, IDENT_LENGTH_BYTES, authoritative_data.internal.ident.adf);

	/* DME values as well */
	arbitrate_value_unidirectional(&result.fp.dme_values, &_last_authoritative_data.internal.dme, &new_fs_data.internal.dme, sizeof(new_fs_data.internal.dme), &authoritative_data.internal.dme);

	/* Flip switch values can only be read from flightpanel */
	arbitrate_value_unidirectional(&result.fs.flip_switches, &_last_authoritative_data.external.flip_switches, &new_fp_data.external.flip_switches, sizeof(new_fp_data.external.flip_switches), &authoritative_data.external.flip_switches);


	/* Synchronize switches in any case when connecting initially */
	if (_first_sync) {
		result.fs.flip_switches = true;
	}

	/* Sync both instruments with master data */
	_fp_connection->put_data(authoritative_data, result.fp);
	_fs_connection->put_data(authoritative_data, result.fs);

	diff_instrument_data(stderr, "Flightpanel", _last_authoritative_data, new_fp_data);
	diff_instrument_data(stderr, "Flightsim", _last_authoritative_data, new_fs_data);
	dump_instrument_data(stderr, "Authoritative", authoritative_data);
	fprintf(stderr, "======================================================================\n");

	_last_authoritative_data = authoritative_data;
	return result;
}

void Arbiter::thread_action() {
	if (!_fp_connection->connected()) {
		/* Do not arbitrate if no FP connection present! */
		_first_sync = true;
		return;
	}

	if (!_fs_connection->data_fresh().wait()) {
		fprintf(stderr, "FS is not fresh, timed out.\n");
		return;
	}
	if (!_fp_connection->data_fresh().wait()) {
		fprintf(stderr, "FP is not fresh, timed out.\n");
		return;
	}
	struct instrument_data_t new_fs_data;
	_fs_connection->get_data(&new_fs_data);

	if (!_fp_connection->data_initialized()) {
		struct arbiter_elements_t dummy;
		_fp_connection->put_data(new_fs_data, dummy, true);
		_fp_connection->wait_for_ack();
	}

	struct instrument_data_t new_fp_data;
	_fp_connection->get_data(&new_fp_data);

	arbitrate(new_fs_data, new_fp_data);
	_first_sync = false;
}

