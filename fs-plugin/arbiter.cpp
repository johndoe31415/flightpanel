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
#include <unistd.h>

#include "arbiter.hpp"

Arbiter::Arbiter(FSConnection *fs_connection, FPConnection *fp_connection) {
	_fs_connection = fs_connection;
	_fp_connection = fp_connection;
}

void Arbiter::run() {
	fprintf(stderr, "Arbiter started FS %p, FP %p\n", _fs_connection, _fp_connection);
	while (true) {
		struct instrument_data_t fs_data;
		_fs_connection->get_data(&fs_data);

		struct instrument_data_t fp_data;
		_fp_connection->get_data(&fp_data);

//		_fp_connection->put_data(&fs_data);

		sleep(1);
	}
}
