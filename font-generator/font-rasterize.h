#ifndef __FONT_RASTERIZE_H__
#define __FONT_RASTERIZE_H__

#include <cairo.h>
#include <stdint.h>

struct cairo_font_surface_t {
	const char *fontname;
	double fontsize;
	cairo_surface_t *csurface;
	cairo_t *cctx;
	cairo_font_extents_t font_extents;
	int base_x, base_y;
};

struct glyph_t {
	int codepoint;
	int width, height;
	int offset_x, offset_y;
	uint8_t *data;
	const char *text;
	cairo_text_extents_t text_extents;
};

/*************** AUTO GENERATED SECTION FOLLOWS ***************/
/***************  AUTO GENERATED SECTION ENDS   ***************/

#endif
