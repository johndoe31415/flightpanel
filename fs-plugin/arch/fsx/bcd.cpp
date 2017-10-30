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

#include <stdint.h>
#include "bcd.hpp"

uint32_t bcd_to_decimal(uint32_t bcd_value) {
	uint32_t result = 0;
	uint32_t value = 1;
	while (bcd_value) {
		uint8_t digit = bcd_value & 0xf;
		result += digit * value;
		value *= 10;
		bcd_value >>= 4;
	}
	return result;
}

uint32_t decimal_to_bcd(uint32_t decimal_value) {
	uint32_t result = 0;
	uint32_t divisor = 1000000000;
	while (divisor) {
		uint8_t digit = decimal_value / divisor;
		decimal_value = decimal_value % divisor;
		result = (result << 4) | digit;
		divisor /= 10;
	}
	return result;
}


#ifdef BCD_DEBUG
#include <stdio.h>

int main() {
	fprintf(stderr, "%d\n", bcd_to_decimal(0x123456));
	fprintf(stderr, "%x\n", decimal_to_bcd(123456));
	return 0;
}
#endif
