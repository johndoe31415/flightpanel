#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>

#include "simconnect.hpp"

int main(void) {
	void *fs_connection = simconnect_init();
	simconnect_event_loop(fs_connection);
	simconnect_close(fs_connection);
	return 0;
}
