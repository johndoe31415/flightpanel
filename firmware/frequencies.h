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

#define VHF_MIN_FREQUENCY_KHZ		118000
#define VHF_BANDWIDTH_MHZ			19
#define VHF_DIVISIONS_PER_100_KHZ	16
#define VHF_DIVISIONS_PER_MHZ		(VHF_DIVISIONS_PER_100_KHZ * 10)
#define VHF_DIVISIONS				(VHF_BANDWIDTH_MHZ * VHF_DIVISIONS_PER_MHZ)

#define NAV_MIN_FREQUENCY_KHZ		108000
#define NAV_BANDWIDTH_MHZ			10

/*************** AUTO GENERATED SECTION FOLLOWS ***************/
uint32_t vhf_index_to_frequency_khz(int vhf_index);
uint32_t nav_index_to_frequency_khz(int nav_index);
/***************  AUTO GENERATED SECTION ENDS   ***************/

#endif
