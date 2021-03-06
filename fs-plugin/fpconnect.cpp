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
#include <pthread.h>
#include <unistd.h>
#include <stdexcept>

#include "logging.hpp"
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

static FlightSimConnection *fs_connection;
static FPConnection *fp_connection;
static Arbiter *arbiter;

void start_arbiter_thread() {
	fs_connection = new FlightSimConnection();
	fp_connection = new FPConnection();
	arbiter = new Arbiter(fs_connection, fp_connection);

	arbiter->start();
}

void stop_arbiter_thread() {
	delete arbiter;
	delete fs_connection;
	delete fp_connection;
}

#if defined(VARIANT_LINUX_EMU) || defined(VARIANT_WINDOWS_FSX)
static void start_arbiter(void) {
	start_arbiter_thread();
	logmsg(LLVL_INFO, "Press return to exit...");
	char buffer[256];
	while (true) {
		while (fgets(buffer, sizeof(buffer), stdin) == NULL);
#if defined(VARIANT_LINUX_EMU)
		fs_connection->poke();
#else
		logmsg(LLVL_INFO, "Interrupted by keypress.");
		break;
#endif
	}
	logmsg(LLVL_INFO, "Exiting!");
	stop_arbiter_thread();
}

int main(void) {
	try {
		start_arbiter();
	} catch (const std::runtime_error &exception) {
		logmsg(LLVL_FATAL, "Execution failed with exception: %s", exception.what());
		return 1;
	}
	return 0;
}
#endif
