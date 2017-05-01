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

#include "fsconnection.hpp"
#include "fpconnection.hpp"
#include "arbiter.hpp"

#if defined(VARIANT_WINDOWS_FSX)
#include "simconnect.hpp"
#define FlightSimConnection		SimConnectConnection

#elif defined(VARIANT_LINUX_EMU)
#include "emulator.hpp"
#define FlightSimConnection		EmulatedConnection

#elif defined(VARIANT_LINUX_XPLANE)
#include "xsquawkbox.hpp"
#define FlightSimConnection		XSquawkBoxConnection

#else

#error "FIXME: Undefined variant."

#endif

int main(void) {
	FSConnection *fs_connection = new FlightSimConnection();
	FPConnection *fp_connection = new FPConnection();

	//fs_connection->event_loop();
	//fp_connection->event_loop();

	Arbiter arbiter(fs_connection, fp_connection);
	arbiter.run();

	delete fs_connection;
	return 0;
}
