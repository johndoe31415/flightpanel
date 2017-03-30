#ifndef __SURFACE_H__
#define __SURFACE_H__

#include <stdint.h>
#include <stdbool.h>
#define SURFACE_ACCESSOR_LR_TB_COLBYTES

struct surface_t {
	int width, height;
	uint8_t *data;
};

/*************** AUTO GENERATED SECTION FOLLOWS ***************/
void surface_clear(const struct surface_t *surface);
bool surface_getpixel(const struct surface_t *surface, int x, int y);
void surface_setpixel(const struct surface_t *surface, int x, int y);
void surface_dump(const struct surface_t *surface);
/***************  AUTO GENERATED SECTION ENDS   ***************/

#endif
