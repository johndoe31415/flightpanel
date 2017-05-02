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

#define COM_MIN_FREQUENCY_KHZ		118000
#define COM_BANDWIDTH_MHZ			19
#define COM_DIVISIONS_PER_100_KHZ	16
#define COM_DIVISIONS_PER_MHZ		(COM_DIVISIONS_PER_100_KHZ * 10)
#define COM_DIVISIONS				(COM_BANDWIDTH_MHZ * COM_DIVISIONS_PER_MHZ)

#define NAV_MIN_FREQUENCY_KHZ		108000
#define NAV_BANDWIDTH_MHZ			10
#define NAV_DIVISIONS_PER_MHZ		20
#define NAV_KHZ_PER_DIVISION		(1000 / NAV_DIVISIONS_PER_MHZ)
#define NAV_DIVISIONS				(NAV_BANDWIDTH_MHZ * NAV_DIVISIONS_PER_MHZ)

#ifdef __cplusplus
extern "C" {
#endif
/*************** AUTO GENERATED SECTION FOLLOWS ***************/
uint32_t com_index_to_frequency_khz(int com_index);
uint16_t com_frequency_khz_to_index(uint32_t frequency_khz);
uint32_t nav_index_to_frequency_khz(int nav_index);
uint16_t nav_frequency_khz_to_index(uint32_t frequency_khz);
/***************  AUTO GENERATED SECTION ENDS   ***************/
#ifdef __cplusplus
}
#endif

#endif
