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

#ifndef __HELPER_SURFACE_H__
#define __HELPER_SURFACE_H__

#include <surface.h>

struct pixel_t {
	int x, y;
};

struct pixel_list_t {
	int pixel_cnt;
	struct pixel_t *pixels;
};

/*************** AUTO GENERATED SECTION FOLLOWS ***************/
struct pixel_list_t* enumerate_pixels(const struct surface_t *surface);
char *pixel_list_to_str(const struct pixel_list_t *pixel_list);
void free_pixel_list(struct pixel_list_t *pixel_list);
/***************  AUTO GENERATED SECTION ENDS   ***************/

#endif
