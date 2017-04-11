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

#ifndef __FONT_RASTERIZE_H__
#define __FONT_RASTERIZE_H__

#include <cairo.h>
#include <stdint.h>

struct cairo_font_surface_t {
	const char *fontname;
	double fontsize;
	cairo_surface_t *csurface;
	cairo_t *cctx;
	cairo_font_extents_t font_extents;
	int base_x, base_y;
};

struct glyph_t {
	int codepoint;
	int width, height;
	int offset_x, offset_y;
	uint8_t *data;
	const char *text;
	cairo_text_extents_t text_extents;
};

/*************** AUTO GENERATED SECTION FOLLOWS ***************/
/***************  AUTO GENERATED SECTION ENDS   ***************/

#endif
