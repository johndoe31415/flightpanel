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
