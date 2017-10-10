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
#include "morse.h"

static const char *morse_codes[] = {
	[ 0] = ".-",	// A
	[ 1] = "-...",	// B
	[ 2] = "-.-.",	// C
	[ 3] = "-..",	// D
	[ 4] = ".",		// E
	[ 5] = "..-.",	// F
	[ 6] = "--.",	// G
	[ 7] = "....",	// H
	[ 8] = "..",	// I
	[ 9] = ".---",	// J
	[10] = "-.-",	// K
	[11] = ".-..",	// L
	[12] = "--",	// M
	[13] = "-.",	// N
	[14] = "---",	// O
	[15] = ".--.",	// P
	[16] = "--.-",	// Q
	[17] = ".-.",	// R
	[18] = "...",	// S
	[19] = "-",		// T
	[20] = "..-",	// U
	[21] = "...-",	// V
	[22] = ".--",	// W
	[23] = "-..-",	// X
	[24] = "-.--",	// Y
	[25] = "--..",	// Z
};

const char *morse_get_code(const char letter) {
	int index = -1;
	if ((letter >= 'A') && (letter <= 'Z')) {
		index = letter - 'A';
	} else if ((letter >= 'a') && (letter <= 'z')) {
		index = letter - 'a';
	}
	if (index != -1) {
		return morse_codes[index];
	} else {
		return NULL;
	}
}
