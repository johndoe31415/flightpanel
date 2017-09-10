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

#ifndef __IOMUX_H__
#define __IOMUX_H__

#include <stdint.h>
#include <stdbool.h>

#define IOMUX_BYTECOUNT			9
#define IOMUX_OUTPUTS			(IOMUX_BYTECOUNT * 8)

/*************** AUTO GENERATED SECTION FOLLOWS ***************/
const uint8_t *iomux_input_array(void);
void iomux_disable(void);
void iomux_clamp_all_outputs(bool do_clamp);
void iomux_trigger(void);
void dsr_iomux_dma_finished(void);
bool iomux_get_input_from(const uint8_t *iomux_array, int pin_id);
bool iomux_get_input(int pin_id);
void iomux_output_setall(uint8_t byte_value);
void iomux_output_toggle(int pin_id);
void iomux_output_set(int pin_id, bool value);
/***************  AUTO GENERATED SECTION ENDS   ***************/

#endif
