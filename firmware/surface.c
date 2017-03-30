#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "surface.h"
#include "debug.h"

void surface_clear(const struct surface_t *surface) {
	memset(surface->data, 0x00, surface->width * surface->height / 8);
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

#ifdef TEST_SURFACE
// gcc -O2 -Wall -std=c11 -DTEST_SURFACE -o testsurface surface.c && ./testsurface
int main(void) {
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
	surface_dump(&surface);
	return 0;
}
#endif
