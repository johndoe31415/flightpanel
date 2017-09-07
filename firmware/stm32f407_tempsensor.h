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

#ifndef __STM32F407_TEMPSENSOR_H__
#define __STM32F407_TEMPSENSOR_H__

#include <stdint.h>
#include "stm32f407_adc.h"

/*************** AUTO GENERATED SECTION FOLLOWS ***************/
void calculate_telemetry(struct adc_telemetry_t *telemetry, const uint16_t adu_30_deg_c, const uint16_t adu_110_deg_c, const uint16_t adu_vrefint, const uint16_t adu_tempsensor);
/***************  AUTO GENERATED SECTION ENDS   ***************/

#endif
