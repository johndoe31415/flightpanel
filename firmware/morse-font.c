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
#include "font.h"
#include "morse-font.h"

/*
 * Rasterized font: "Helvetica" at 20.000 pt
 * Antialiasing used: CAIRO_ANTIALIAS_GRAY
 * Rasterization threshold: 96
 * Character set:  •-
 */

static int codepoint_to_charindex(const unsigned int codepoint) {
	if (codepoint == 32) {
		return codepoint - 32;
	} else if ((codepoint >= 45) && (codepoint <= 46)) {
		return codepoint - 44;
	} else {
		return -1;
	}
}

const struct font_t font_morse_font = {
	.codepoint_to_charindex_fn = codepoint_to_charindex,
	.glyphs = {
		[0] = { // Codepoint 32 (" "), char index 0
			.xadvance = 6,
			.xoffset = 0,
			.yoffset = 0,
			.width = 0,
			.height = 0,
			.data = (const uint8_t[]) {  },
		},

		[1] = { // Codepoint 45 ("-"), char index 1
			.xadvance = 7,
			.xoffset = 0,
			.yoffset = -7,
			.width = 6,
			.height = 3,
			.data = (const uint8_t[]) { 0x00, 0x3e, 0x00 },
		},

		[2] = { // Codepoint 46 ("."), char index 2
			.xadvance = 7,
			.xoffset = 1,
			.yoffset = -9,
			.width = 5,
			.height = 6,
			.data = (const uint8_t[]) { 0x00, 0x0e, 0x1f, 0x1f, 0x1f, 0x0e },
		},

	}
};

