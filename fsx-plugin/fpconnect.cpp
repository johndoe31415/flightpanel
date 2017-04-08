#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>

#include "fsconnection.hpp"

#if defined(VARIANT_WINDOWS)
#include "simconnect.hpp"
#define FlightSimConnection		SimConnectConnection

#elif defined(VARIANT_LINUX_EMU)
#include "emulator.hpp"
#define FlightSimConnection		EmulatedConnection

#elif defined(VARIANT_LINUX_XPLANE)
#include "emulator.hpp"
#define FlightSimConnection		EmulatedConnection

#else

#error "FIXME: Undefined variant."

#endif

int main(void) {
	FSConnection *fs_connection = new FlightSimConnection();
	if ((!fs_connection) || (!fs_connection->connected())) {
		exit(EXIT_FAILURE);
	}
	fs_connection->event_loop();
	delete fs_connection;
	return 0;
}
