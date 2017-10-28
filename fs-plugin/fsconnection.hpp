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

#ifndef __FSCONNECTION_HPP__
#define __FSCONNECTION_HPP__

#include <stdint.h>
#include <stdbool.h>

#include "usb_hidreport.h"
#include "frequencies.h"
#include "arbiter_elements.hpp"
#include "thread.hpp"

enum ignore_value_t {
	IGNORE_IAS = (1 << 0),
	IGNORE_CLIMBRATE = (1 << 1),
};

struct internal_state_t {
	struct {
		char nav1[5], nav2[5], adf[5];
	} ident;
	struct dme_info_t dme;
	struct {
		uint8_t ignore_values;
	} ap;
};

struct instrument_data_t {
	struct hid_report_t external;
	struct internal_state_t internal;

	uint32_t com1_active_freq_khz() const {
		return frequency_index_to_khz((const enum com_nav_range_t)external.com_divisions, external.com1.freq.active_index);
	}
	uint32_t com1_standby_freq_khz() const {
		return frequency_index_to_khz((const enum com_nav_range_t)external.com_divisions, external.com1.freq.standby_index);
	}
	uint32_t com2_active_freq_khz() const {
		return frequency_index_to_khz((const enum com_nav_range_t)external.com_divisions, external.com2.freq.active_index);
	}
	uint32_t com2_standby_freq_khz() const {
		return frequency_index_to_khz((const enum com_nav_range_t)external.com_divisions, external.com2.freq.standby_index);
	}
	uint32_t nav1_active_freq_khz() const {
		return frequency_index_to_khz((const enum com_nav_range_t)external.nav_divisions, external.nav1.freq.active_index);
	}
	uint32_t nav1_standby_freq_khz() const {
		return frequency_index_to_khz((const enum com_nav_range_t)external.nav_divisions, external.nav1.freq.standby_index);
	}
	uint32_t nav2_active_freq_khz() const {
		return frequency_index_to_khz((const enum com_nav_range_t)external.nav_divisions, external.nav2.freq.active_index);
	}
	uint32_t nav2_standby_freq_khz() const {
		return frequency_index_to_khz((const enum com_nav_range_t)external.nav_divisions, external.nav2.freq.standby_index);
	}
};

class FSConnection {
	protected:
		Event _data_fresh;

	public:
		FSConnection() {
		}
		virtual void get_data(struct instrument_data_t *data) = 0;
		virtual void put_data(const struct instrument_data_t &data, const struct arbiter_elements_t &elements) = 0;
		Event &data_fresh() {
			return _data_fresh;
		}
		virtual ~FSConnection() {
		}
};


/*************** AUTO GENERATED SECTION FOLLOWS ***************/
void dump_instrument_data(FILE *f, const char *desc, const struct instrument_data_t &data);
void diff_instrument_data(FILE *f, const char *desc, const struct instrument_data_t &data1, const struct instrument_data_t &data2);
/***************  AUTO GENERATED SECTION ENDS   ***************/

#endif
