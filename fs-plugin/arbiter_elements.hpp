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

#ifndef __ARBITER_ELEMENTS_HPP__
#define __ARBITER_ELEMENTS_HPP__

struct arbiter_elements_t {
	bool radio_panel;
	bool divisions;
	bool tx_radio_id;
	bool com1, com2;
	bool nav1, nav2;
	bool xpdr;
	bool adf;
	bool ap;
	bool qnh;
	bool navigate_by_gps;
	bool flip_switches;
	bool ident_values;
	bool dme_values;

	bool fp_send_report_02() const {
		return radio_panel || divisions || tx_radio_id || com1 || com2 || nav1 || nav2 || xpdr || adf || ap || qnh || navigate_by_gps;
	}

	bool fp_send_report_03() const {
		return ident_values || dme_values;
	}
};

struct arbiter_result_t {
	struct arbiter_elements_t fp;
	struct arbiter_elements_t fs;
};

#endif
