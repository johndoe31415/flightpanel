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

#ifndef __XSQUAWKBOX_HPP__
#define __XSQUAWKBOX_HPP__

#include <stdbool.h>
#include <pthread.h>
#include <XPLMDataAccess.h>
#include "fsconnection.hpp"

struct vhf_dataref_t {
	XPLMDataRef frequency_active, frequency_standby;
	XPLMDataRef rx;
};

struct nav_dataref_t {
	XPLMDataRef frequency_active, frequency_standby;
	XPLMDataRef obs;
	XPLMDataRef ident;
	XPLMDataRef sound;
};

struct adf_dataref_t {
	XPLMDataRef frequency;
	XPLMDataRef compass_rose;
	XPLMDataRef ident;
	XPLMDataRef sound;
};

struct dme_dataref_t {
	XPLMDataRef available;
	XPLMDataRef speed;
	XPLMDataRef distance;
	XPLMDataRef sound;
};

struct ap_dataref_t {
	XPLMDataRef active;
	XPLMDataRef altitude;
	XPLMDataRef climbrate;
	XPLMDataRef heading;
	XPLMDataRef airspeed;
	XPLMDataRef hdg_hold, nav_hold, alt_hold, rev_hold, apr_hold, ias_hold;
};

struct lights_dataref_t {
	XPLMDataRef beacon;
	XPLMDataRef landing;
	XPLMDataRef taxi;
	XPLMDataRef nav;
	XPLMDataRef strobe;
};

struct xpdr_dataref_t {
	XPLMDataRef squawk;
};

struct misc_dataref_t {
	XPLMDataRef barometer_setting;
	XPLMDataRef altitude;
	XPLMDataRef ias;
	XPLMDataRef hsi_selector;
};

struct datarefs_t {
	struct vhf_dataref_t vhf1, vhf2;
	struct nav_dataref_t nav1, nav2;
	struct adf_dataref_t adf;
	struct dme_dataref_t dme;
	struct ap_dataref_t ap;
	struct xpdr_dataref_t xpdr;
	struct lights_dataref_t lights;
	struct misc_dataref_t misc;
};

class XSquawkBoxConnection : public FSConnection {
	private:
		bool _loop_running;
		pthread_t _periodic_query_thread;
		struct datarefs_t _datarefs;
	public:
		XSquawkBoxConnection();
		void event_loop();
		void poll_data();
		bool connected(void) const {
			return true;
		}
		void set_quit() {
			_loop_running = false;
		}
		bool is_loop_running() const {
			return _loop_running;
		}
		~XSquawkBoxConnection();
};

#endif
