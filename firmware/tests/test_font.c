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

#define TEST_WIDTH		80
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

	blit_string_to_cursor(&font_vcr_osd_mono_20, "123.456", &surface, &cursor);
	if (test_verbose()) {
		surface_dump(&surface);
	}

	struct pixel_list_t *pixel_list = enumerate_pixels(&surface);
	char *pixel_string = pixel_list_to_str(pixel_list);
	test_assert_str_eq(pixel_string, "5,5:6,5:15,5:16,5:17,5:18,5:19,5:20,5:27,5:28,5:29,5:30,5:31,5:32,5:48,5:49,5:54,5:55,5:56,5:57,5:58,5:59,5:60,5:61,5:62,5:63,5:68,5:69,5:70,5:71,5:72,5:73,5:4,6:5,6:6,6:14,6:15,6:16,6:17,6:18,6:19,6:20,6:21,6:26,6:27,6:28,6:29,6:30,6:31,6:32,6:33,6:47,6:48,6:49,6:54,6:55,6:56,6:57,6:58,6:59,6:60,6:61,6:62,6:63,6:67,6:68,6:69,6:70,6:71,6:72,6:73,6:74,6:3,7:4,7:5,7:6,7:13,7:14,7:15,7:20,7:21,7:22,7:25,7:26,7:27,7:32,7:33,7:34,7:46,7:47,7:48,7:49,7:54,7:55,7:66,7:67,7:68,7:73,7:74,7:75,7:3,8:4,8:5,8:6,8:13,8:14,8:21,8:22,8:25,8:26,8:33,8:34,8:45,8:46,8:47,8:48,8:49,8:54,8:55,8:66,8:67,8:74,8:75,8:5,9:6,9:21,9:22,9:33,9:34,9:44,9:45,9:46,9:48,9:49,9:54,9:55,9:56,9:57,9:58,9:59,9:60,9:61,9:66,9:67,9:5,10:6,10:20,10:21,10:22,10:32,10:33,10:34,10:43,10:44,10:45,10:48,10:49,10:54,10:55,10:56,10:57,10:58,10:59,10:60,10:61,10:62,10:66,10:67,10:5,11:6,11:15,11:16,11:17,11:18,11:19,11:20,11:21,11:29,11:30,11:31,11:32,11:33,11:42,11:43,11:44,11:48,11:49,11:61,11:62,11:63,11:66,11:67,11:68,11:69,11:70,11:71,11:72,11:73,11:5,12:6,12:14,12:15,12:16,12:17,12:18,12:19,12:20,12:29,12:30,12:31,12:32,12:33,12:42,12:43,12:48,12:49,12:62,12:63,12:66,12:67,12:68,12:69,12:70,12:71,12:72,12:73,12:74,12:5,13:6,13:13,13:14,13:15,13:32,13:33,13:34,13:42,13:43,13:44,13:45,13:46,13:47,13:48,13:49,13:50,13:51,13:62,13:63,13:66,13:67,13:73,13:74,13:75,13:5,14:6,14:13,14:14,14:33,14:34,14:42,14:43,14:44,14:45,14:46,14:47,14:48,14:49,14:50,14:51,14:62,14:63,14:66,14:67,14:74,14:75,14:5,15:6,15:13,15:14,15:25,15:26,15:33,15:34,15:48,15:49,15:54,15:55,15:62,15:63,15:66,15:67,15:74,15:75,15:5,16:6,16:13,16:14,16:25,16:26,16:27,16:32,16:33,16:34,16:48,16:49,16:54,16:55,16:56,16:61,16:62,16:63,16:66,16:67,16:68,16:73,16:74,16:75,16:3,17:4,17:5,17:6,17:7,17:8,17:13,17:14,17:15,17:16,17:17,17:18,17:19,17:20,17:21,17:22,17:26,17:27,17:28,17:29,17:30,17:31,17:32,17:33,17:38,17:39,17:48,17:49,17:55,17:56,17:57,17:58,17:59,17:60,17:61,17:62,17:67,17:68,17:69,17:70,17:71,17:72,17:73,17:74,17:3,18:4,18:5,18:6,18:7,18:8,18:13,18:14,18:15,18:16,18:17,18:18,18:19,18:20,18:21,18:22,18:27,18:28,18:29,18:30,18:31,18:32,18:38,18:39,18:48,18:49,18:56,18:57,18:58,18:59,18:60,18:61,18:68,18:69,18:70,18:71,18:72,18:73,18");
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

