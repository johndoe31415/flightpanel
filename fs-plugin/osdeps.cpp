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

#include "osdeps.hpp"

#ifdef __windows

#include <windows.h>
#include <pthread_time.h>

void sleep_millis(unsigned int millis) {
	Sleep(millis);
}

void now(struct timespec *ts) {
	clock_gettime(CLOCK_REALTIME, ts);
}

#else

#include <unistd.h>
#include <time.h>

void sleep_millis(unsigned int millis) {
	usleep(1000 * millis);
}

void now(struct timespec *ts) {
	clock_gettime(CLOCK_REALTIME, ts);
}

#endif
