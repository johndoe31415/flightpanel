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
#include "inconsolata-30.h"

/*
 * Rasterized font: "Inconsolata" at 30.000 pt
 * Antialiasing used: CAIRO_ANTIALIAS_GOOD
 * Default rasterization threshold: 75
 * Custom rasterization thresholds: None
 * Character set: . 0123456789
 */

static int codepoint_to_charindex(const unsigned int codepoint) {
	if (codepoint == 32) {
		return codepoint - 32;
	} else if (codepoint == 46) {
		return codepoint - 45;
	} else if ((codepoint >= 48) && (codepoint <= 57)) {
		return codepoint - 46;
	} else {
		return -1;
	}
}

const struct font_t font_inconsolata_30 = {
	.codepoint_to_charindex_fn = codepoint_to_charindex,
	.glyphs = {
		[0] = { // Codepoint 32 (" "), char index 0
			.xadvance = 18,
			.xoffset = 0,
			.yoffset = 0,
			.width = 0,
			.height = 0,
			.data = (const uint8_t[]) {  },
		},

		[1] = { // Codepoint 46 ("."), char index 1
			.xadvance = 11,
			.xoffset = 4,
			.yoffset = -5,
			.width = 4,
			.height = 5,
			.data = (const uint8_t[]) { 0x0f, 0x0f, 0x0f, 0x0f, 0x0f },
		},

		[2] = { // Codepoint 48 ("0"), char index 2
			.xadvance = 18,
			.xoffset = 1,
			.yoffset = -22,
			.width = 16,
			.height = 22,
			.data = (const uint8_t[]) { 0xe0, 0x07, 0xf0, 0x0f, 0xf8, 0x1f, 0x3c, 0x3c, 0x1c, 0x38, 0x1e, 0x78, 0x1e, 0x70, 0x0e, 0x70, 0x8e, 0x71, 0xce, 0x73, 0xce, 0x73, 0x8e, 0x71, 0x0e, 0x70, 0x0e, 0x70, 0x0e, 0x70, 0x1e, 0x70, 0x1e, 0x78, 0x1c, 0x38, 0x3c, 0x3c, 0xf8, 0x1f, 0xf0, 0x0f, 0xe0, 0x07 },
		},

		[3] = { // Codepoint 49 ("1"), char index 3
			.xadvance = 18,
			.xoffset = 3,
			.yoffset = -22,
			.width = 14,
			.height = 22,
			.data = (const uint8_t[]) { 0xfc, 0x01, 0xff, 0x01, 0xff, 0x01, 0xe7, 0x01, 0xe0, 0x01, 0xe0, 0x01, 0xe0, 0x01, 0xe0, 0x01, 0xe0, 0x01, 0xe0, 0x01, 0xe0, 0x01, 0xe0, 0x01, 0xe0, 0x01, 0xe0, 0x01, 0xe0, 0x01, 0xe0, 0x01, 0xe0, 0x01, 0xe0, 0x01, 0xe0, 0x01, 0xfe, 0x1f, 0xfe, 0x1f, 0xfe, 0x1f },
		},

		[4] = { // Codepoint 50 ("2"), char index 4
			.xadvance = 18,
			.xoffset = 2,
			.yoffset = -22,
			.width = 14,
			.height = 22,
			.data = (const uint8_t[]) { 0xfc, 0x03, 0xff, 0x07, 0xff, 0x0f, 0x07, 0x1e, 0x00, 0x1c, 0x00, 0x3c, 0x00, 0x3c, 0x00, 0x1c, 0x00, 0x1c, 0x00, 0x1e, 0x00, 0x0f, 0x00, 0x07, 0x80, 0x03, 0xc0, 0x01, 0xe0, 0x00, 0xf0, 0x00, 0x78, 0x00, 0x3c, 0x00, 0x1e, 0x00, 0xff, 0x1f, 0xff, 0x3f, 0xff, 0x3f },
		},

		[5] = { // Codepoint 51 ("3"), char index 5
			.xadvance = 18,
			.xoffset = 2,
			.yoffset = -22,
			.width = 14,
			.height = 22,
			.data = (const uint8_t[]) { 0xfc, 0x03, 0xff, 0x0f, 0xff, 0x0f, 0x02, 0x1e, 0x00, 0x1c, 0x00, 0x1c, 0x00, 0x1c, 0x00, 0x1e, 0xf0, 0x1f, 0xf0, 0x07, 0xf0, 0x07, 0x00, 0x0f, 0x00, 0x1c, 0x00, 0x3c, 0x00, 0x38, 0x00, 0x38, 0x00, 0x3c, 0x00, 0x3c, 0x03, 0x1e, 0xff, 0x1f, 0xff, 0x0f, 0xfe, 0x03 },
		},

		[6] = { // Codepoint 52 ("4"), char index 6
			.xadvance = 18,
			.xoffset = 1,
			.yoffset = -22,
			.width = 16,
			.height = 22,
			.data = (const uint8_t[]) { 0x00, 0x1e, 0x00, 0x1f, 0x00, 0x1f, 0x80, 0x1f, 0xc0, 0x1d, 0xc0, 0x1c, 0xe0, 0x1c, 0x70, 0x1c, 0x70, 0x1c, 0x38, 0x1c, 0x18, 0x1c, 0x1c, 0x1c, 0x0e, 0x1c, 0x0e, 0x1c, 0xff, 0x7f, 0xff, 0xff, 0xff, 0xff, 0x00, 0x1c, 0x00, 0x1c, 0x00, 0x1c, 0x00, 0x1c, 0x00, 0x1c },
		},

		[7] = { // Codepoint 53 ("5"), char index 7
			.xadvance = 18,
			.xoffset = 2,
			.yoffset = -22,
			.width = 14,
			.height = 22,
			.data = (const uint8_t[]) { 0xfe, 0x0f, 0xfe, 0x0f, 0xfe, 0x0f, 0x0e, 0x00, 0x0e, 0x00, 0x0e, 0x00, 0x0e, 0x00, 0xfe, 0x03, 0xfe, 0x07, 0xfe, 0x0f, 0x02, 0x1e, 0x00, 0x1c, 0x00, 0x3c, 0x00, 0x3c, 0x00, 0x3c, 0x00, 0x3c, 0x00, 0x3c, 0x00, 0x1c, 0x03, 0x1e, 0xff, 0x0f, 0xff, 0x07, 0xfe, 0x03 },
		},

		[8] = { // Codepoint 54 ("6"), char index 8
			.xadvance = 18,
			.xoffset = 1,
			.yoffset = -22,
			.width = 16,
			.height = 22,
			.data = (const uint8_t[]) { 0xc0, 0x1f, 0xf0, 0x3f, 0xf8, 0x3f, 0x3c, 0x00, 0x1c, 0x00, 0x1c, 0x00, 0x0e, 0x00, 0xce, 0x0f, 0xee, 0x1f, 0xfe, 0x3f, 0x3e, 0x78, 0x1e, 0x78, 0x1e, 0x70, 0x0e, 0x70, 0x0e, 0x70, 0x0e, 0x70, 0x1e, 0x70, 0x1c, 0x78, 0x3c, 0x78, 0xf8, 0x3f, 0xf0, 0x1f, 0xe0, 0x0f },
		},

		[9] = { // Codepoint 55 ("7"), char index 9
			.xadvance = 18,
			.xoffset = 2,
			.yoffset = -22,
			.width = 14,
			.height = 22,
			.data = (const uint8_t[]) { 0xff, 0x3f, 0xff, 0x3f, 0xff, 0x3f, 0x00, 0x1c, 0x00, 0x1c, 0x00, 0x0e, 0x00, 0x0e, 0x00, 0x0f, 0x00, 0x07, 0x00, 0x07, 0x80, 0x07, 0x80, 0x03, 0xc0, 0x03, 0xc0, 0x01, 0xc0, 0x01, 0xe0, 0x01, 0xe0, 0x00, 0xf0, 0x00, 0xf0, 0x00, 0x70, 0x00, 0x78, 0x00, 0x38, 0x00 },
		},

		[10] = { // Codepoint 56 ("8"), char index 10
			.xadvance = 18,
			.xoffset = 1,
			.yoffset = -22,
			.width = 16,
			.height = 22,
			.data = (const uint8_t[]) { 0xf0, 0x0f, 0xf8, 0x1f, 0xfc, 0x3f, 0x1c, 0x78, 0x1e, 0x78, 0x1e, 0x78, 0x1e, 0x78, 0x1c, 0x38, 0xfc, 0x1f, 0xf8, 0x0f, 0xf0, 0x1f, 0x38, 0x3c, 0x1c, 0x78, 0x0e, 0x70, 0x0e, 0x70, 0x0e, 0x70, 0x0e, 0x70, 0x1e, 0x70, 0x1e, 0x78, 0xfc, 0x3f, 0xf8, 0x1f, 0xf0, 0x0f },
		},

		[11] = { // Codepoint 57 ("9"), char index 11
			.xadvance = 18,
			.xoffset = 1,
			.yoffset = -22,
			.width = 16,
			.height = 22,
			.data = (const uint8_t[]) { 0xf0, 0x07, 0xf8, 0x0f, 0xfc, 0x1f, 0x1e, 0x3c, 0x0e, 0x38, 0x0e, 0x78, 0x0e, 0x70, 0x0e, 0x70, 0x0e, 0x78, 0x0e, 0x78, 0x1e, 0x7c, 0xfc, 0x7f, 0xf8, 0x77, 0xf0, 0x73, 0x00, 0x70, 0x00, 0x78, 0x00, 0x38, 0x00, 0x3c, 0x00, 0x3e, 0xfc, 0x1f, 0xfc, 0x0f, 0xf8, 0x03 },
		},

	}
};

