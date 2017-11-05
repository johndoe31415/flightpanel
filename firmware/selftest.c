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

#include <string.h>
#include <stdbool.h>
#include <stm32f4xx_crc.h>
#include "selftest.h"

static struct selftest_result_t selftest_result;
static bool selftest_performed;

extern uint8_t _sflash;
extern uint8_t _sdata;
extern uint8_t _edata;
extern uint8_t _sidata;

/* STM32F4 CRC32
 * 		Poly: 0x4c11db7		 Initial value: 0xffffffff
 *
 *			CRC_CalcBlockCRC("\xaa\xbb\xcc\xdd", 0);						// 0xffffffff
 *			CRC_CalcBlockCRC("\xaa\xbb\xcc\xdd", 1);						// 0x19354eb8
 *			CRC_CalcBlockCRC("\xaa\xbb\xcc\xdd\x11\x22\x33\x44", 2);		// 0x06c986cc
 *			CRC_CalcBlockCRC("\xaa\xaa\xaa\xaa", 1);						// 0x42fc4b29
 *
 */

static struct selftest_result_t perform_selftest(void) {
	struct selftest_result_t result;
	memset(&result, 0, sizeof(result));

	result.origin = (uint32_t)&_sflash;
	result.length = (uint32_t)&_sidata - (uint32_t)&_sflash + (uint32_t)&_edata - (uint32_t)&_sdata;
	CRC_ResetDR();
	result.crc32 = CRC_CalcBlockCRC((uint32_t*)result.origin, result.length / 4);

	return result;
}

const struct selftest_result_t *get_selftest_result(void) {
	if (!selftest_performed) {
		selftest_result = perform_selftest();
	}
	return &selftest_result;
}
