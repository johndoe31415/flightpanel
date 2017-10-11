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

#ifndef __SURFACE_H__
#define __SURFACE_H__

#include <stdint.h>
#include <stdbool.h>
#define SURFACE_ACCESSOR_LR_TB_COLBYTES

struct surface_t {
	int width, height;
	uint8_t *data;
};

/*************** AUTO GENERATED SECTION FOLLOWS ***************/
void surface_fill(const struct surface_t *surface, const uint8_t pattern);
void surface_clear(const struct surface_t *surface);
bool surface_getpixel(const struct surface_t *surface, const int x, const int y);
void surface_setpixel(const struct surface_t *surface, const int x, const int y);
void surface_clrpixel(const struct surface_t *surface, const int x, const int y);
void surface_dump(const struct surface_t *surface);
void surface_draw_border(const struct surface_t *surface);
void blit_rectangle(const struct surface_t *surface, const unsigned int xoffset, const unsigned int yoffset, const unsigned int width, const unsigned int height);
/***************  AUTO GENERATED SECTION ENDS   ***************/

#endif
