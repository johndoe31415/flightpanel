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

#include "frequencies.h"
#include "debug.h"

static const uint8_t com_frequency_100khz_divisions[COM_DIVISIONS_PER_100_KHZ] = {
	0, 5, 10, 15, 25, 30, 35, 40, 50, 55, 60, 65, 75, 80, 85, 90
};

uint32_t com_index_to_frequency_khz(int com_index) {
	com_index = com_index % COM_DIVISIONS;
	int mhz = com_index / COM_DIVISIONS_PER_MHZ;
	int division = com_index % COM_DIVISIONS_PER_MHZ;
	int khz100 = division / COM_DIVISIONS_PER_100_KHZ;
	int khz_index = division % COM_DIVISIONS_PER_100_KHZ;
	uint32_t result = COM_MIN_FREQUENCY_KHZ + (1000 * mhz) + (100 * khz100) + com_frequency_100khz_divisions[khz_index];
	debug("%d -> %d %d %d -> %u\n", com_index, mhz, khz100, khz_index, result);
	return result;
}

uint32_t nav_index_to_frequency_khz(int nav_index) {
	return NAV_MIN_FREQUENCY_KHZ + (nav_index * 50);
}
