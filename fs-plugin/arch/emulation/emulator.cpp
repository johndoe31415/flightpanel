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
#include <stdbool.h>
#include <stdint.h>
#include <math.h>
#include <stddef.h>
#include <unistd.h>
#include <string.h>
#include "emulator.hpp"

void EmulatedConnection::get_data(struct instrument_data_t *data) {
	memcpy(data, &_instrument_data, sizeof(struct instrument_data_t));
}

void EmulatedConnection::put_data(const struct instrument_data_t &data, const arbiter_elements_t &elements) {
	_instrument_data = data;
}

static int rand_value(int maxvalue) {
	return rand() % maxvalue;
}

static int rand_value(int minvalue, int maxvalue) {
	return rand_value(maxvalue - minvalue) + minvalue;
}

static double rand_double(double scalar) {
	return scalar * (double)rand_value(2147483647) / 2147483647;
}

static bool rand_bool(void) {
	return rand_value(2);
}

static bool rand_percentage(double percentage) {
	return rand_double(100) <= percentage;
}

void EmulatedConnection::randomize(struct com_state_t *com) {
	com->freq.active_index = rand_value(19 * 40);
	com->freq.standby_index = rand_value(19 * 40);
}

void EmulatedConnection::randomize(struct nav_state_t *nav) {
	nav->freq.active_index = rand_value(19 * 40);
	nav->freq.standby_index = rand_value(19 * 40);
	nav->obs = rand_value(360);
}

void EmulatedConnection::randomize(struct adf_state_t *adf) {
	adf->frequency_khz = rand_value(100, 1200);
}

void EmulatedConnection::randomize(struct ap_state_t *ap) {
	ap->state = 0;
	if (rand_percentage(75)) {
		ap->state |= AP_STATE_ACTIVE;
	}
	if (rand_percentage(25)) {
		ap->state |= AP_ALTITUDE_HOLD;
	}
	if (rand_percentage(25)) {
		ap->state |= AP_IAS_HOLD;
	}
	if (rand_percentage(25)) {
		if (rand_percentage(25)) {
			ap->state |= AP_HEADING_HOLD;
		} else if (rand_percentage(25)) {
			ap->state |= AP_LOCALIZER_HOLD;
		} else if (rand_percentage(25)) {
			ap->state |= AP_GLIDESLOPE_HOLD;
		}
	}
	ap->climbrate = (rand_value(5) * 100) + 200;
	ap->heading = rand_value(360);
	ap->ias = rand_value(8, 15) * 10;
	ap->altitude = 1200 + 100 * rand_value(60);
}

void EmulatedConnection::randomize(struct xpdr_state_t *xpdr) {
	if (rand_percentage(50)) {
		xpdr->state = XPDR_STANDBY;
	} else {
		xpdr->state = XPDR_CHARLY;
	}
	xpdr->squawk = 0;
	for (int i = 0; i < 4; i++) {
		xpdr->squawk = (10 * xpdr->squawk) + rand_value(8);
	}
}

void EmulatedConnection::randomize(struct hid_report_t *misc) {
	misc->tx_radio_id = rand_value(1, 2);
	misc->radio_panel = 0;
	misc->qnh = rand_value(970, 1030);
	misc->navigate_by_gps = rand_bool();
}


void EmulatedConnection::randomize(char ident[IDENT_LENGTH_BYTES]) {
	if (rand_percentage(70)) {
		for (int i = 0; i < rand_value(3, IDENT_LENGTH_BYTES - 1); i++) {
			ident[i] = 'A' + rand_value(26);
		}
	}
}

void EmulatedConnection::randomize(struct internal_state_t *internal) {
	randomize(internal->ident.nav1);
	randomize(internal->ident.nav2);
	randomize(internal->ident.adf);
	internal->dme.available = rand_bool();
	internal->dme.distance_tenth_nm = rand_value(700);
	internal->dme.velocity = rand_value(120);
}

void EmulatedConnection::poke() {
	_instrument_data.external.nav1.obs++;
}

EmulatedConnection::EmulatedConnection() {
	time_t now;
	time(&now);
	srand(now);

	memset(&_instrument_data, 0, sizeof(_instrument_data));
	randomize(&_instrument_data.internal);
	randomize(&_instrument_data.external);
	randomize(&_instrument_data.external.com1);
	randomize(&_instrument_data.external.com2);
	randomize(&_instrument_data.external.nav1);
	randomize(&_instrument_data.external.nav2);
	randomize(&_instrument_data.external.xpdr);
	randomize(&_instrument_data.external.adf);
	randomize(&_instrument_data.external.ap);

	_data_fresh.set();
}

EmulatedConnection::~EmulatedConnection() {
}

