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

#include <pthread.h>
#include "fsconnection.hpp"
#include "fpconnection.hpp"
#include "thread.hpp"

class Arbiter : public Thread {
	private:
		bool _first_sync;
		FSConnection *_fs_connection;
		FPConnection *_fp_connection;
		struct instrument_data_t _last_authoritative_data;
		struct arbiter_result_t arbitrate(const struct instrument_data_t &new_fs_data, const struct instrument_data_t &new_fp_data);
		template<typename T> void arbitrate_value(bool *update_fs, bool *update_fp, const T &old_authoritative_data, const T &new_fs_data, const T &new_fp_data, T *authoritative_data);
		template<typename T> void arbitrate_value_unidirectional(bool *update, const T *old_data, const T *new_data, unsigned int data_size, T *authoritative_data);
		void thread_action();
	public:
		Arbiter(FSConnection *fs_connection, FPConnection *fp_connection);
};

#endif

