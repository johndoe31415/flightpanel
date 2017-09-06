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

#ifndef __DEBUG_H__
#define __DEBUG_H__

#if defined(__ARM_ARCH) || defined(DISABLE_FIRMWARE_DEBUG)

#define debug(msg, ...)

#define emit(value)			__asm__ __volatile__(".word " #value)

#define bkpt(value)			__asm__ __volatile__("bkpt #" #value)

#endif

/*************** AUTO GENERATED SECTION FOLLOWS ***************/
void debug_show_version(void);
void debug_show_device_id(void);
void debug_show_compiler(void);
void debug_show_memory(void);
void debug_show_all(void);
/***************  AUTO GENERATED SECTION ENDS   ***************/

#endif
