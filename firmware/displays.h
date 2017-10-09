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

#ifndef __DISPLAYS_H__
#define __DISPLAYS_H__

#define DISPLAY_COUNT		12

enum display_t {
	DISPLAY_COM1 = 0,
	DISPLAY_COM1_STBY = 1,
	DISPLAY_COM2 = 3,
	DISPLAY_COM2_STBY = 2,
	DISPLAY_NAV1 = 4,
	DISPLAY_NAV1_STBY = 5,
	DISPLAY_DME = 6,
	DISPLAY_NAV2 = 9,
	DISPLAY_NAV2_STBY = 8,
	DISPLAY_ADF = 7,
	DISPLAY_AP = 10,
	DISPLAY_XPDR = 11,
};

/*************** AUTO GENERATED SECTION FOLLOWS ***************/
void isr_display_dma_finished(void);
void dsr_display_dma_finished(void);
void displays_check_dma_schedule(void);
void display_mark_surface_dirty(int display_index);
const struct surface_t* displays_get_surface(int display_index);
void init_displays(void);
/***************  AUTO GENERATED SECTION ENDS   ***************/

#endif
