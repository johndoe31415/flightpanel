#include "testbed.h"
#include <surface.h>

struct pixel {
	int x, y;
};

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

	const int max_pixel_cnt = 64;
	struct pixel pixels[max_pixel_cnt];
	int pixel_cnt = 0;
	for (int y = 0; y < surface.height; y++) {
		for (int x = 0; x < surface.width; x++) {
			if (surface_getpixel(&surface, x, y) && (pixel_cnt < max_pixel_cnt)) {
				pixels[pixel_cnt].x = x;
				pixels[pixel_cnt].y = y;
				pixel_cnt++;
			}
		}
	}
#ifdef SURFACE_ACCESSOR_LR_TB_COLBYTES
	debug("%u pixels set:\n", pixel_cnt);
	for (int i = 0; i < pixel_cnt; i++) {
		debug("   %d: %3u, %3u\n", i, pixels[i].x, pixels[i].y);
	}
	test_fail_unless(pixel_cnt == 9);
	test_fail_unless(pixels[0].x == 0);
	test_fail_unless(pixels[0].y == 0);
	test_fail_unless(pixels[1].x == 1);
	test_fail_unless(pixels[1].y == 1);
	test_fail_unless(pixels[2].x == 2);
	test_fail_unless(pixels[2].y == 2);
	test_fail_unless(pixels[3].x == 3);
	test_fail_unless(pixels[3].y == 3);
	test_fail_unless(pixels[4].x == 4);
	test_fail_unless(pixels[4].y == 4);
	test_fail_unless(pixels[5].x == 5);
	test_fail_unless(pixels[5].y == 5);
	test_fail_unless(pixels[6].x == 6);
	test_fail_unless(pixels[6].y == 6);
	test_fail_unless(pixels[7].x == 7);
	test_fail_unless(pixels[7].y == 7);
	test_fail_unless(pixels[8].x == 0);
	test_fail_unless(pixels[8].y == 8);
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

