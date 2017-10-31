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
#include "frequencies.h"
#include "debug.h"

static const struct com_nav_range_definition_t frequency_definitions[] = {
	[COM_RANGE_5KHZ] = {
		.base_frequency_khz = 118000,
		.division_count = 10 * 19,
		.division_value = 100,
		.subdivision_count = 16,
		.subdivisions = (const uint8_t[]) { 0, 5, 10, 15, 25, 30, 35, 40, 50, 55, 60, 65, 75, 80, 85, 90 },
	},
	[COM_RANGE_25KHZ] = {
		.base_frequency_khz = 118000,
		.division_count = 40 * 19,
		.division_value = 25,
		.subdivision_count = 0,
		.subdivisions = NULL,
	},
	[NAV_RANGE] = {
		.base_frequency_khz = 108000,
		.division_count = 10 * 20,
		.division_value = 50,
		.subdivision_count = 0,
		.subdivisions = NULL,
	},
};

uint32_t frequency_index_to_khz(const enum com_nav_range_t range, const int index) {
	const struct com_nav_range_definition_t *def = (const struct com_nav_range_definition_t*)(frequency_definitions + range);
	if (def->subdivision_count) {
		const int division = index / def->subdivision_count;
		const int subdivision = index % def->subdivision_count;
		return def->base_frequency_khz + (division * def->division_value) + def->subdivisions[subdivision];
	} else {
		return def->base_frequency_khz + (index * def->division_value);
	}
}

uint32_t frequency_khz_to_index(const enum com_nav_range_t range, const uint32_t frequency_khz) {
	const struct com_nav_range_definition_t *def = (const struct com_nav_range_definition_t*)(frequency_definitions + range);
	const uint32_t top_frequency_khz = frequency_khz - def->base_frequency_khz;

	if (def->subdivision_count) {
		const int division_index = top_frequency_khz / def->division_value * def->subdivision_count;
		const int subdivision_value = top_frequency_khz % def->division_value;
		int subdivision_index = def->subdivision_count;
		for (int i = 0; i < def->subdivision_count; i++) {
			if (def->subdivisions[i] >= subdivision_value) {
				subdivision_index = i;
				break;
			}
		}
		return division_index + subdivision_index;
	} else {
		return top_frequency_khz / def->division_value;
	}
}

int frequency_detent_count(const enum com_nav_range_t range) {
	const struct com_nav_range_definition_t *def = (const struct com_nav_range_definition_t*)(frequency_definitions + range);
	return def->division_count * (def->subdivision_count ? def->subdivision_count : 1);
}
