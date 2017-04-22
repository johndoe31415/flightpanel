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
#include <stdlib.h>
#include <surface.h>

#include "helper_surface.h"

struct pixel_list_t* enumerate_pixels(const struct surface_t *surface) {
	struct pixel_list_t *pixel_list = calloc(1, sizeof(*pixel_list));
	test_fail_unless(pixel_list);

	for (int y = 0; y < surface->height; y++) {
		for (int x = 0; x < surface->width; x++) {
			if (surface_getpixel(surface, x, y)) {
				pixel_list->pixels = realloc(pixel_list->pixels, sizeof(struct pixel_t) * (pixel_list->pixel_cnt + 1));
				test_fail_unless(pixel_list->pixels);

				pixel_list->pixels[pixel_list->pixel_cnt].x = x;
				pixel_list->pixels[pixel_list->pixel_cnt].y = y;
				pixel_list->pixel_cnt++;
			}
		}
	}
	return pixel_list;
}

char *pixel_list_to_str(const struct pixel_list_t *pixel_list) {
	const int bufsize = 10 * pixel_list->pixel_cnt;		// ballpark
	char *buffer = calloc(1, bufsize);
	char *bufend = buffer + bufsize;
	test_fail_unless(buffer);

	char *curptr = buffer;
	for (int i = 0; i < pixel_list->pixel_cnt; i++) {
		curptr += snprintf(curptr, bufend - curptr, "%s%d,%d", (i == 0) ? "" : ":", pixel_list->pixels[i].x, pixel_list->pixels[i].y);
	}
	return buffer;
}

void free_pixel_list(struct pixel_list_t *pixel_list) {
	free(pixel_list->pixels);
	free(pixel_list);
}

