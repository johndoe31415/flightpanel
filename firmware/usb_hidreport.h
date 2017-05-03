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

#ifndef __USB_HIDREPORT_H__
#define __USB_HIDREPORT_H__

#include <stdint.h>

struct hid_report_t {
	uint16_t com1_active, com1_standby;
	uint16_t com2_active, com2_standby;
	uint16_t nav1_active, nav1_standby;
	uint16_t nav2_active, nav2_standby;
	uint16_t squawk;
	uint16_t buttons;
} __attribute__ ((packed));

struct hid_set_report_t {
	uint8_t report_id;
	uint16_t com1_active, com1_standby;
	uint16_t com2_active, com2_standby;
	uint16_t nav1_active, nav1_standby;
	uint16_t nav2_active, nav2_standby;
} __attribute__ ((packed));

#endif
