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
#include <stdint.h>
#include <string.h>
#include "stm32f4xx_stackpaint.h"

extern const uint8_t _sram, _eram;

struct stackpaint_result_t stm32fxx_get_stackpaint(void) {
	struct stackpaint_result_t result;
	memset(&result, 0, sizeof(result));

	/* Determine RAM size */
	result.total_ram_bytes = &_eram - &_sram;

	/* Determine heap usage */
	const unsigned int ram_word_size = result.total_ram_bytes / sizeof(uint32_t);
	const uint32_t *ram = (const uint32_t*)&_sram;
	for (int i = 0; i < ram_word_size; i++) {
		if (ram[i] == 0xdeadbeef) {
			break;
		}
		result.heap_used_bytes += 4;
	}

	/* Determine stack usage */
	for (int i = ram_word_size - 1; i >= 0; i--) {
		if (ram[i] == 0xdeadbeef) {
			break;
		}
		result.stack_used_bytes += 4;
	}

#if 0
	for (int i = 0; i < RAM_WORD_SIZE; i += 8) {
		const uint32_t *addr = ((const uint32_t*)RAM_BEGIN) + i;
		printf("%p: ", addr);
		for (int j = 0; j < 8; j++) {
			printf("%8lx ", addr[j]);
		}
		printf("\n");

	}
#endif

	return result;
}
