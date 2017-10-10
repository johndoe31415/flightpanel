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
#include "testbed.h"
#include "helper_surface.h"
#include <vcr-osd-mono-20.h>
#include <surface.h>

#define TEST_WIDTH		85
#define TEST_HEIGHT		32

static void test_font_write(void) {
	subtest_start();
	const struct surface_t surface = {
		.width = TEST_WIDTH,
		.height = TEST_HEIGHT,
		.data = (uint8_t[TEST_WIDTH * TEST_HEIGHT / 8]) { },
	};

	surface_clear(&surface);
	struct cursor_t cursor = { 0, 20 };

	blit_string_to_cursor(&font_vcr_osd_mono_20, "123.456", &surface, &cursor, false);
	if (test_verbose()) {
		surface_dump(&surface);
	}

	struct pixel_list_t *pixel_list = enumerate_pixels(&surface);
	char *pixel_string = pixel_list_to_str(pixel_list);
	test_assert_str_eq(pixel_string, "5,5:6,5:15,5:16,5:17,5:18,5:19,5:20,5:27,5:28,5:29,5:30,5:31,5:32,5:55,5:56,5:61,5:62,5:63,5:64,5:65,5:66,5:67,5:68,5:69,5:70,5:75,5:76,5:77,5:78,5:79,5:80,5:4,6:5,6:6,6:14,6:15,6:16,6:17,6:18,6:19,6:20,6:21,6:26,6:27,6:28,6:29,6:30,6:31,6:32,6:33,6:54,6:55,6:56,6:61,6:62,6:63,6:64,6:65,6:66,6:67,6:68,6:69,6:70,6:74,6:75,6:76,6:77,6:78,6:79,6:80,6:81,6:3,7:4,7:5,7:6,7:13,7:14,7:15,7:20,7:21,7:22,7:25,7:26,7:27,7:32,7:33,7:34,7:53,7:54,7:55,7:56,7:61,7:62,7:73,7:74,7:75,7:80,7:81,7:82,7:3,8:4,8:5,8:6,8:13,8:14,8:21,8:22,8:25,8:26,8:33,8:34,8:52,8:53,8:54,8:55,8:56,8:61,8:62,8:73,8:74,8:81,8:82,8:5,9:6,9:21,9:22,9:33,9:34,9:51,9:52,9:53,9:55,9:56,9:61,9:62,9:63,9:64,9:65,9:66,9:67,9:68,9:73,9:74,9:5,10:6,10:20,10:21,10:22,10:32,10:33,10:34,10:50,10:51,10:52,10:55,10:56,10:61,10:62,10:63,10:64,10:65,10:66,10:67,10:68,10:69,10:73,10:74,10:5,11:6,11:15,11:16,11:17,11:18,11:19,11:20,11:21,11:29,11:30,11:31,11:32,11:33,11:49,11:50,11:51,11:55,11:56,11:68,11:69,11:70,11:73,11:74,11:75,11:76,11:77,11:78,11:79,11:80,11:5,12:6,12:14,12:15,12:16,12:17,12:18,12:19,12:20,12:29,12:30,12:31,12:32,12:33,12:49,12:50,12:55,12:56,12:69,12:70,12:73,12:74,12:75,12:76,12:77,12:78,12:79,12:80,12:81,12:5,13:6,13:13,13:14,13:15,13:32,13:33,13:34,13:49,13:50,13:51,13:52,13:53,13:54,13:55,13:56,13:57,13:58,13:69,13:70,13:73,13:74,13:80,13:81,13:82,13:5,14:6,14:13,14:14,14:33,14:34,14:49,14:50,14:51,14:52,14:53,14:54,14:55,14:56,14:57,14:58,14:69,14:70,14:73,14:74,14:81,14:82,14:5,15:6,15:13,15:14,15:25,15:26,15:33,15:34,15:55,15:56,15:61,15:62,15:69,15:70,15:73,15:74,15:81,15:82,15:5,16:6,16:13,16:14,16:25,16:26,16:27,16:32,16:33,16:34,16:55,16:56,16:61,16:62,16:63,16:68,16:69,16:70,16:73,16:74,16:75,16:80,16:81,16:82,16:3,17:4,17:5,17:6,17:7,17:8,17:13,17:14,17:15,17:16,17:17,17:18,17:19,17:20,17:21,17:22,17:26,17:27,17:28,17:29,17:30,17:31,17:32,17:33,17:40,17:41,17:55,17:56,17:62,17:63,17:64,17:65,17:66,17:67,17:68,17:69,17:74,17:75,17:76,17:77,17:78,17:79,17:80,17:81,17:3,18:4,18:5,18:6,18:7,18:8,18:13,18:14,18:15,18:16,18:17,18:18,18:19,18:20,18:21,18:22,18:27,18:28,18:29,18:30,18:31,18:32,18:40,18:41,18:55,18:56,18:63,18:64,18:65,18:66,18:67,18:68,18:75,18:76,18:77,18:78,18:79,18:80,18");
	free(pixel_string);
	free_pixel_list(pixel_list);
	subtest_finished();
}

int main(int argc, char **argv) {
	test_start(argc, argv);
	test_font_write();
	test_finished();
	return 0;
}

