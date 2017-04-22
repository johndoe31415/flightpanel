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

#include <stdlib.h>
#include "testbed.h"
#include "helper_surface.h"
#include <surface.h>

static void test_surface_read(void) {
	const struct surface_t surface = {
		.width = 64,
		.height = 32,
		.data = (uint8_t[64 * 32 / 8]) { },
	};
	surface.data[0] = 0x01;
	surface.data[1] = 0x02;
	surface.data[2] = 0x04;
	surface.data[3] = 0x08;
	surface.data[4] = 0x10;
	surface.data[5] = 0x20;
	surface.data[6] = 0x40;
	surface.data[7] = 0x80;
	surface.data[64] = 0x01;

	if (test_verbose()) {
		surface_dump(&surface);
	}

#ifdef SURFACE_ACCESSOR_LR_TB_COLBYTES
	struct pixel_list_t *pixel_list = enumerate_pixels(&surface);
	char *pixel_string = pixel_list_to_str(pixel_list);
	test_assert_str_eq(pixel_string, "0,0:1,1:2,2:3,3:4,4:5,5:6,6:7,7:0,8");
	free(pixel_string);
	free_pixel_list(pixel_list);
#else
	test_fail("No test define for this particular surface accessor function.");
#endif
}

int main(int argc, char **argv) {
	test_start(argc, argv);
	test_surface_read();
	test_success();
	return 0;
}

