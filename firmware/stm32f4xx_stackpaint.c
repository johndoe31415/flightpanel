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

struct stackpaint_result_t stm32fxx_get_stackpaint(void) {
	struct stackpaint_result_t result;
	memset(&result, 0, sizeof(result));

	/* Determine heap usage */
	const uint32_t *ram = (const uint32_t*)RAM_BEGIN;
	for (int i = 0; i < RAM_WORD_SIZE; i++) {
		if (ram[i] == 0xdeadbeef) {
			break;
		}
		result.heap_used += 4;
	}

	/* Determine stack usage */
	for (int i = RAM_WORD_SIZE - 1; i >= 0; i--) {
		if (ram[i] == 0xdeadbeef) {
			break;
		}
		result.stack_used += 4;
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
