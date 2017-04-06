#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>

#include "simconnect.hpp"

int main(void) {
	struct flightsim_connection_t *fs_connection = simconnect_init();
	if (!fs_connection) {
		exit(EXIT_FAILURE);
	}
	simconnect_event_loop(fs_connection);
	simconnect_close(fs_connection);
	return 0;
}
