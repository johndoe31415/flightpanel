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

#ifndef __DEBUGCONSOLE_H__
#define __DEBUGCONSOLE_H__

#include <stdint.h>

enum debugmode_t {
	DEBUG_DISABLED = 0,
	DEBUG_RS232_ISR,
	DEBUG_RS232_ECHO,
	DEBUG_GPIO_OUTPUTS,
	DEBUG_IOMUX_INPUTS,
	DEBUG_IOMUX_OUTPUTS,
	DEBUG_IOMUX_OUTPUTS_BLINK,
	DEBUG_IOMUX_OUTPUT_PATTERN,
	DEBUG_DISPLAY,
	DEBUG_DELAY,
	DEBUG_ADC_TELEMETRY,
};

/*************** AUTO GENERATED SECTION FOLLOWS ***************/
void debugmode_set(enum debugmode_t new_mode);
void debugconsole_tick(void);
void dsr_execute_debug_command(void);
void debugconsole_rxchar(uint8_t rxchar);
/***************  AUTO GENERATED SECTION ENDS   ***************/

#endif
