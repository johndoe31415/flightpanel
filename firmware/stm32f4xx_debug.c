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
#include <misc.h>
#include "stm32f4xx_debug.h"

uint32_t overhead_cycles;

void __attribute__ ((noinline)) timing_start(void) {
	DWT->CYCCNT = 0;
}

uint32_t __attribute__ ((noinline)) timing_end(void) {
	uint32_t cycles = DWT->CYCCNT;
	if (cycles < overhead_cycles) {
		return 0;
	} else {
		return cycles - overhead_cycles;
	}
}

void timing_init(void) {
	/* Unlock "Lock Access Register" inside ITM */
	ITM->LAR = 0xC5ACCE55;

	/* Enable core debugging */
	CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;

	/* Enable cycle counting */
	DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;

	/* Now calibrate */
	uint32_t min_cycles = 0xffffffff;
	overhead_cycles = 0;
	for (int i = 0; i < 10; i++) {
		timing_start();
		uint32_t cycles = timing_end();
		if (cycles < min_cycles) {
			min_cycles = cycles;
		}
	}
	overhead_cycles = min_cycles;
	printf("Calibrated to %lu cycles.\n", overhead_cycles);
}
