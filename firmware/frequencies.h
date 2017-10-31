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

#ifndef __FREQUENCIES_H__
#define __FREQUENCIES_H__

#include <stdint.h>
#include "usb_hidreport.h"

struct com_nav_range_definition_t {
	unsigned int base_frequency_khz;
	unsigned int division_count;
	unsigned int division_value;
	unsigned int subdivision_count;
	const uint8_t *subdivisions;
};

#ifdef __cplusplus
extern "C" {
#endif
/*************** AUTO GENERATED SECTION FOLLOWS ***************/
uint32_t frequency_index_to_khz(const enum com_nav_range_t range, const int index);
uint32_t frequency_khz_to_index(const enum com_nav_range_t range, const uint32_t frequency_khz);
int frequency_detent_count(const enum com_nav_range_t range);
/***************  AUTO GENERATED SECTION ENDS   ***************/
#ifdef __cplusplus
}
#endif

#endif
