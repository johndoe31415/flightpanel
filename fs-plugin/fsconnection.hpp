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

#if 0
struct com_data_t {
	uint32_t freq_active_khz;
	uint32_t freq_standby_khz;
	bool rx, tx;
};

struct nav_data_t {
	uint32_t freq_active_khz;
	uint32_t freq_standby_khz;
	uint16_t obs;
	char ident[8];
	bool sound;
};

struct adf_data_t {
	uint32_t freq_hz;
	char ident[8];
	uint16_t compass_rose;
	bool sound;
};

struct dme_data_t {
	bool available;
	uint8_t nav_id;
	uint16_t distance_nm_tenths;
	uint16_t speed_kt;
	bool sound;
};

struct light_data_t {
	bool beacon;
	bool landing;
	bool taxi;
	bool nav;
	bool strobe;
};

struct ap_data_t {
	bool active;
	uint32_t altitude_ft;
	int16_t climbrate_ft_per_min;
	uint16_t heading;
	uint16_t speed_kts;
	bool hdg_hold, nav_hold, alt_hold, rev_hold, apr_hold, ias_hold;
};

struct transponder_data_t {
	uint16_t squawk;
};

struct misc_data_t {
	uint16_t ias_kt;
	uint16_t indicated_alt_ft;
	uint16_t qnh_millibar;
	bool guide_gps;
};

struct instrument_data_t {
	struct com_data_t com1, com2;
	struct nav_data_t nav1, nav2;
	struct adf_data_t adf;
	struct dme_data_t dme;
	struct light_data_t lights;
	struct ap_data_t ap;
	struct transponder_data_t xpdr;
	struct misc_data_t misc;

	template<typename T> void set_value(int at_offset, const T &new_value) const {
		T* ptr = (T*)(((const uint8_t*)this) + at_offset);
		*ptr = new_value;
	}

	template<typename T> T get_value(int at_offset) const {
		const T* ptr = (const T*)(((const uint8_t*)this) + at_offset);
		return *ptr;
	}
};

struct component_selection_t {
	bool com1_active;
	bool com1_standby;
	bool com2_active;
	bool com2_standby;
	bool com_rxtx;

	bool nav1_active;
	bool nav1_standby;
	bool nav2_active;
	bool nav2_standby;

	bool is_any_set() const {
		return com1_active || com1_standby || com2_active || com2_standby || com_rxtx
				|| nav1_active || nav1_standby || nav2_active || nav2_standby;
	}

	void set_flag(int at_offset) {
		bool *ptr = (bool*)(((const uint8_t*)this) + at_offset);
		*ptr = true;
	}
};
#endif

struct instrument_data_t {
	struct hid_report_t external;
	struct {
		bool foo;
	} internal;

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
		return frequency_index_to_khz((const enum com_nav_range_t)external.com_divisions, external.nav1.freq.active_index);
	}
	uint32_t nav1_standby_freq_khz() const {
		return frequency_index_to_khz((const enum com_nav_range_t)external.com_divisions, external.nav1.freq.standby_index);
	}
	uint32_t nav2_active_freq_khz() const {
		return frequency_index_to_khz((const enum com_nav_range_t)external.com_divisions, external.nav2.freq.active_index);
	}
	uint32_t nav2_standby_freq_khz() const {
		return frequency_index_to_khz((const enum com_nav_range_t)external.com_divisions, external.nav2.freq.standby_index);
	}
};

class FSConnection {
	public:
		FSConnection() {
		}
		virtual void get_data(struct instrument_data_t *data) = 0;
		virtual void put_data(const struct instrument_data_t *data, const struct component_selection_t *selection) = 0;
		virtual ~FSConnection() {
		}
};


/*************** AUTO GENERATED SECTION FOLLOWS ***************/
void dump_instrument_data(const struct instrument_data_t *data);
void fsconnection_incoming_data(const struct instrument_data_t *data);
/***************  AUTO GENERATED SECTION ENDS   ***************/

#endif
