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

#ifndef __CONFIGURATION_H__
#define __CONFIGURATION_H__

#include <stdint.h>
#include <stdbool.h>

struct configuration {
	struct {
		uint16_t ident_timeout_milliseconds;
		uint16_t edit_timeout_milliseconds;
		uint16_t vfr_squawk;
	} xpdr;
	struct {
		uint8_t tx_radio_id;
		uint8_t com_frequency_divisions;
		uint8_t nav_frequency_divisions;
		struct {
			uint32_t active_frequency_khz;
			uint32_t standby_frequency_khz;
		} com1, com2, nav1, nav2;
		struct {
			uint16_t ias;
			uint16_t altitude;
			uint16_t climbrate;
		} ap;
		uint16_t adf_frequency_khz;
		uint16_t squawk;
		uint16_t qnh;
	} instruments;
	struct {
		uint32_t time_to_blank_milliseconds;
	} misc;
	uint32_t crc32;
};

/*************** AUTO GENERATED SECTION FOLLOWS ***************/
void write_configuration(struct configuration *config);
bool read_configuration(struct configuration *config);
void init_configuration(void);
/***************  AUTO GENERATED SECTION ENDS   ***************/

#endif
