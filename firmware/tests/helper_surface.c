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

