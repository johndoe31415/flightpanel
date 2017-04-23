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

#ifndef __INSTRUMENTS_H__
#define __INSTRUMENTS_H__

#define VHF_BASE_FREQUENCY			118000
#define VHF_DETENT					25
#define VHF_BANDWIDTH_DETENTS		(19 * 40)

#define NAV_BASE_FREQUENCY			108000
#define NAV_DETENT					50
#define NAV_BANDWIDTH_DETENTS		(10 * 20)

/*************** AUTO GENERATED SECTION FOLLOWS ***************/
void hid_tick(void);
void instruments_handle_inputs(void);
void input_callback_rotary_button(int rotary_id, bool value);
void input_callback_rotary(int rotary_id, bool value1, bool value2);
void instruments_idle_loop(void);
/***************  AUTO GENERATED SECTION ENDS   ***************/

#endif
