#ifndef __FONT_H__
#define __FONT_H__

#include <stdint.h>
#include "surface.h"

typedef int (*cp_to_charindex_t)(int codepoint);

struct glyph_t {
	uint8_t xadvance;
	int8_t xoffset, yoffset;
	uint8_t width, height;
	const uint8_t *data;
};

struct font_t {
	cp_to_charindex_t codepoint_to_charindex_fn;
	struct glyph_t glyphs[];
};

struct cursor_t {
	int x, y;
};

/*************** AUTO GENERATED SECTION FOLLOWS ***************/
void blit_string_to_cursor(const struct font_t *font, const char *string, const struct surface_t *surface, struct cursor_t *cursor);
/***************  AUTO GENERATED SECTION ENDS   ***************/

#endif
