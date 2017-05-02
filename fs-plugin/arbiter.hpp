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

#ifndef __ARBITER_HPP__
#define __ARBITER_HPP__

#include "fsconnection.hpp"
#include "fpconnection.hpp"

class Arbiter {
	private:
		FSConnection *_fs_connection;
		FPConnection *_fp_connection;
		struct instrument_data_t _last_fs_data, _last_fp_data;

		struct instrument_data_t _put_fs_data, _put_fp_data;
		struct component_selection_t _put_fs_selection, _put_fp_selection;

		void arbitrate(const struct instrument_data_t &new_fs_data, const struct instrument_data_t &new_fp_data);
		template<typename T> void arbitrate_value(const struct instrument_data_t &new_fs_data, const struct instrument_data_t &new_fp_data, const struct arbitration_t &entry);
	public:
		Arbiter(FSConnection *fs_connection, FPConnection *fp_connection);
		void run();
};

/*************** AUTO GENERATED SECTION FOLLOWS ***************/
/***************  AUTO GENERATED SECTION ENDS   ***************/

#endif
