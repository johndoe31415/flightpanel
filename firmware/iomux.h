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

/*************** AUTO GENERATED SECTION FOLLOWS ***************/
void iomux_trigger(void);
void iomux_dma_finished(void);
bool iomux_get_input(int pin_id);
void iomux_output_set(int pin_id, bool value);
void iomux_dump_iochange(void);
/***************  AUTO GENERATED SECTION ENDS   ***************/

#endif
