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

#include <stdio.h>
#include "stm32f4xx_devid.h"
#include "stm32f4xx_stackpaint.h"
#include "debug.h"

void debug_show_version(void) {
	printf("Compiled from: " BUILD_REVISION "\n");
}

void debug_show_device_id(void) {
	char ascii_device_id[32];
	stm32fxx_get_ascii_devid(ascii_device_id);
	printf("STM32 Device ID: %s\n", ascii_device_id);
}

void debug_show_compiler(void) {
	printf("Compiled with: gcc " __VERSION__ " newlib " _NEWLIB_VERSION " on " BUILD_TIMESTAMP_UTC " UTC\n");
}

void debug_show_memory(void) {
	struct stackpaint_result_t stackpaint = stm32fxx_get_stackpaint();
	printf("%d kiB of RAM total: %d bytes of heap used, %d bytes of stack used max (%d bytes currently). %d bytes free.\n", stackpaint.total_ram_bytes / 1024, stackpaint.heap_used_bytes, stackpaint.max_stack_used_bytes, stackpaint.stack_used_bytes, stackpaint.total_ram_bytes - stackpaint.heap_used_bytes - stackpaint.max_stack_used_bytes);
}

void debug_show_all(void) {
	debug_show_version();
	debug_show_device_id();
	debug_show_compiler();
	debug_show_memory();
}
