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
	{ "COM1 active frequency", ARBITRATION_UINT32_T, offsetof(struct instrument_data_t, com1.freq_active_khz), offsetof(struct component_selection_t, com1_active) },
	{ "COM1 standby frequency", ARBITRATION_UINT32_T, offsetof(struct instrument_data_t, com1.freq_standby_khz), offsetof(struct component_selection_t, com1_standby) },
//	{ "COM2 active frequency", ARBITRATION_UINT32_T, offsetof(struct instrument_data_t, com2.freq_active_khz), offsetof(struct component_selection_t, com2_active) },
//	{ "COM2 standby frequency", ARBITRATION_UINT32_T, offsetof(struct instrument_data_t, com2.freq_standby_khz), offsetof(struct component_selection_t, com2_standby) },
};

Arbiter::Arbiter(FSConnection *fs_connection, FPConnection *fp_connection) {
	_fs_connection = fs_connection;
	_fp_connection = fp_connection;
	memset(&_last_fs_data, 0, sizeof(struct instrument_data_t));
	memset(&_last_fp_data, 0, sizeof(struct instrument_data_t));
}

template<typename T> void Arbiter::arbitrate_value(const struct instrument_data_t &new_fs_data, const struct instrument_data_t &new_fp_data, const struct arbitration_t &entry) {
	T fs_value, fp_value;
	T last_fs_value, last_fp_value;
	memcpy(&fs_value, ((uint8_t*)&new_fs_data) + entry.instrument_offset, sizeof(T));
	memcpy(&fp_value, ((uint8_t*)&new_fp_data) + entry.instrument_offset, sizeof(T));
	memcpy(&last_fs_value, ((uint8_t*)&_last_fs_data) + entry.instrument_offset, sizeof(T));
	memcpy(&last_fp_value, ((uint8_t*)&_last_fp_data) + entry.instrument_offset, sizeof(T));

	if (fs_value != fp_value) {
		/* Arbitration needed, instruments differ */
		bool fs_changed = (fs_value != last_fs_value);
		bool fp_changed = (fp_value != last_fp_value);
		bool anything_changed = fs_changed || fp_changed;
		bool *change_value_ptr;
		if (!anything_changed || fs_changed) {
			/* If flight simulator, both instruments or no instruments changed,
			 * sync Flightsim -> Flightpanel */
			std::cerr << "arbitrating " << entry.name << ": " << fs_value << " vs. " << fp_value << " FS -> FP" << std::endl;
			memcpy(((uint8_t*)&_put_fp_data) + entry.instrument_offset, &fs_value, sizeof(T));
			change_value_ptr = (bool*)(((uint8_t*)&_put_fp_selection) + entry.selection_offset);
		} else {
			/* Otherwise sync Flightpanel -> Flightsim (i.e., only when
			 * Flightpanel has changed) */
			std::cerr << "arbitrating " << entry.name << ": " << fs_value << " vs. " << fp_value << " FP -> FS" << std::endl;
			memcpy(((uint8_t*)&_put_fs_data) + entry.instrument_offset, &fp_value, sizeof(T));
			change_value_ptr = (bool*)(((uint8_t*)&_put_fs_selection) + entry.selection_offset);
		}
		*change_value_ptr = true;
	}
}

static bool anything_set(const struct component_selection_t &selection) {
	const uint8_t *rawdata = (const uint8_t*)&selection;
	for (unsigned int i = 0; i < sizeof(struct component_selection_t); i++) {
		if (rawdata[i] != 0) {
			return true;
		}
	}
	return false;
}

void Arbiter::arbitrate(const struct instrument_data_t &new_fs_data, const struct instrument_data_t &new_fp_data) {
	memset(&_put_fs_data, 0, sizeof(_put_fs_data));
	memset(&_put_fs_selection, 0, sizeof(_put_fs_selection));
	memset(&_put_fp_data, 0, sizeof(_put_fp_data));
	memset(&_put_fp_selection, 0, sizeof(_put_fp_selection));
	for (unsigned int i = 0; i < sizeof(arbitration) / sizeof(struct arbitration_t); i++) {
		if (arbitration[i].arbitration_type == ARBITRATION_UINT32_T) {
			arbitrate_value<uint32_t>(new_fs_data, new_fp_data, arbitration[i]);
		} else {
			fprintf(stderr, "Unable to arbitrate type %d at index %d.\n", arbitration[i].arbitration_type, i);
		}
	}
	if (anything_set(_put_fp_selection)) {
		_fp_connection->put_data(&_put_fp_data, &_put_fp_selection);
	}
	if (anything_set(_put_fs_selection)) {
		_fs_connection->put_data(&_put_fs_data, &_put_fs_selection);
	}
}

void Arbiter::run() {
	fprintf(stderr, "Arbiter started FS %p, FP %p\n", _fs_connection, _fp_connection);
	bool first = true;
	while (true) {
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
#if 0
		struct component_selection_t fs_update, fp_update;
		memset(&fs_update, 0, sizeof(fs_update));
		memset(&fp_update, 0, sizeof(fp_update));
		if (fs_data.com1.freq_active_khz != fp_data.com1.freq_active_khz) {
			if (_fs_data.com1.freq_active_khz != fs_data.com1.freq_active_khz) {
				fp_update.com1_active = true;
			} else {
				fs_update.com1_active = true;
			}
		}
#endif
		sleep(1);
		fprintf(stderr, "\n");
	}
}

