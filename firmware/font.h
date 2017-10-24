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

#ifndef __FONT_H__
#define __FONT_H__

#include <stdint.h>
#include "surface.h"

typedef int (*cp_to_charindex_t)(const unsigned int codepoint);

#define CHAR_DEGREES			"\xb0"		// °

#define TEXT_CENTER				-1
#define TEXT_RIGHT_JUSTIFY		-2

struct glyph_t {
	uint8_t xadvance;
	int8_t xoffset, yoffset;
	uint8_t width, height;
	const uint8_t *data;
};

struct font_t {
	cp_to_charindex_t codepoint_to_charindex_fn;
	struct glyph_t glyphs[];
};

struct cursor_t {
	int x, y;
};

/*************** AUTO GENERATED SECTION FOLLOWS ***************/
unsigned int font_determine_string_width(const struct font_t *font, const char *string);
void blit_string_to_cursor(const struct font_t *font, const char *string, const struct surface_t *surface, struct cursor_t *cursor, const bool invert);
/***************  AUTO GENERATED SECTION ENDS   ***************/

#endif
