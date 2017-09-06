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
#include "stm32f4xx_devid.h"

#define STM32Fxx_DEVICE_ID_REGISTER		((uint32_t*)0x1fff7a10)

void stm32fxx_get_devid(uint8_t bin_device_id[static 12]) {
	for (int i = 0; i < 3; i++) {
		((uint32_t*)bin_device_id)[i] = STM32Fxx_DEVICE_ID_REGISTER[i];
	}
}

void stm32fxx_get_ascii_devid(char *device_id) {
	uint8_t bin_device_id[12];
	stm32fxx_get_devid(bin_device_id);
	memcpy(device_id, bin_device_id + 5, 7);
	sprintf(device_id + 7, "-%02x-%02x-%02x", bin_device_id[0], bin_device_id[2], bin_device_id[4]);
}
