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
#include <stdint.h>
#include <stdio.h>

#include "surface.h"
#include "debug.h"

void surface_fill(const struct surface_t *surface, uint8_t pattern) {
	memset(surface->data, pattern, surface->width * surface->height / 8);
}

void surface_clear(const struct surface_t *surface) {
	surface_fill(surface, 0);
}

#if defined(SURFACE_ACCESSOR_LR_TB_ROWBYTES)
static int surface_pixel_byteoffset(const struct surface_t *surface, int x, int y) {
	return (x / 8) + (y * (surface->width / 8));
}

static int surface_pixel_bitoffset(const struct surface_t *surface, int x, int y) {
	return x % 8;
}
#elif defined(SURFACE_ACCESSOR_LR_TB_COLBYTES)
static int surface_pixel_byteoffset(const struct surface_t *surface, int x, int y) {
	return (y / 8 * surface->width) + (x % surface->width);
}

static int surface_pixel_bitoffset(const struct surface_t *surface, int x, int y) {
	return y % 8;
}
#else
#error "No surface accessor function defined."
#endif

bool surface_getpixel(const struct surface_t *surface, int x, int y) {
	if ((x < 0) || (y < 0) || (x >= surface->width) || (y >= surface->height)) {
		return false;
	}
	const int byte_offset = surface_pixel_byteoffset(surface, x, y);
	const int bit_offset = surface_pixel_bitoffset(surface, x, y);
	return ((surface->data[byte_offset] >> bit_offset) & 1) != 0;
}

void surface_setpixel(const struct surface_t *surface, int x, int y) {
	if ((x < 0) || (y < 0) || (x >= surface->width) || (y >= surface->height)) {
		return;
	}
	const int byte_offset = surface_pixel_byteoffset(surface, x, y);
	const int bit_offset = surface_pixel_bitoffset(surface, x, y);
	surface->data[byte_offset] |= (1 << bit_offset);
}

void surface_dump(const struct surface_t *surface) {
	for (int y = 0; y < surface->height; y++) {
		printf("| ");
		for (int x = 0; x < surface->width; x++) {
			printf("%s", surface_getpixel(surface, x, y) ? "⬤ " : "  ");
		}
		printf(" |\n");
	}
}

void surface_draw_border(const struct surface_t *surface) {
	for (int x = 0; x < surface->width; x++) {
		surface_setpixel(surface, x, 0);
		surface_setpixel(surface, x, surface->height - 1);
	}
	for (int y = 0; y < surface->height; y++) {
		surface_setpixel(surface, 0, y);
		surface_setpixel(surface, surface->width - 1, y);
	}
}

