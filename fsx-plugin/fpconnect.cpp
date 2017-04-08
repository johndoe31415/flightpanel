#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>

#include "fsconnection.hpp"

#if defined(VARIANT_WINDOWS)
#include "simconnect.hpp"
#define DefaultConnection		SimConnectConnection

#elif defined(VARIANT_LINUX)
#include "emulator.hpp"
#define DefaultConnection		EmulatedConnection
#endif

int main(void) {
	FSConnection *fs_connection = new DefaultConnection();
	if ((!fs_connection) || (!fs_connection->connected())) {
		exit(EXIT_FAILURE);
	}
	fs_connection->event_loop();
	delete fs_connection;
	return 0;
}
