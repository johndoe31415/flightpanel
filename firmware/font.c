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

#include "font.h"
#include "surface.h"

static void blit_glyph(const struct glyph_t *glyph, const struct surface_t *surface, const int x0, const int y0, const bool invert) {
	const int glyph_rowwidth = (glyph->width + 7) / 8;
	for (int y = 0; y < glyph->height; y++) {
		for (int x = 0; x < glyph->width; x++) {
			const int byte_offset = (x / 8) + (y * glyph_rowwidth);
			const int bit_offset = x % 8;
			const bool pixel = (((glyph->data[byte_offset] >> bit_offset) & 1) != 0) ^ invert;
			if (pixel) {
				surface_setpixel(surface, x0 + x + glyph->xoffset, y0 + y + glyph->yoffset);
			} else {
				surface_clrpixel(surface, x0 + x + glyph->xoffset, y0 + y + glyph->yoffset);
			}
		}
	}
}

static void blit_glyph_to_cursor(const struct glyph_t *glyph, const struct surface_t *surface, struct cursor_t *cursor, const bool invert) {
	blit_glyph(glyph, surface, cursor->x, cursor->y, invert);
	cursor->x += glyph->xadvance;
}

unsigned int font_determine_string_width(const struct font_t *font, const char *string) {
	unsigned int width = 0;
	while (*string) {
		const unsigned int c = (*string) & 0xff;
		const int glyph_index = font->codepoint_to_charindex_fn(c);
		if (glyph_index >= 0) {
			const struct glyph_t *glyph = &font->glyphs[glyph_index];
			width += glyph->xadvance;
		}
		string++;
	}
	return width;
}

void blit_string_to_cursor(const struct font_t *font, const char *string, const struct surface_t *surface, struct cursor_t *cursor, const bool invert) {
	if ((cursor->x == TEXT_CENTER) || (cursor->x == TEXT_RIGHT_JUSTIFY)) {
		const unsigned int string_width = font_determine_string_width(font, string);
		const unsigned int display_width = surface->width;

		if (string_width < display_width) {
			if (cursor->x == TEXT_CENTER) {
				cursor->x = (display_width - string_width) / 2;
			} else {
				cursor->x = display_width - string_width;
			}
		} else {
			/* Text does not fit. Left-adjust. */
			cursor->x = 0;
		}
	}
	while (*string) {
		const unsigned int c = (*string) & 0xff;
		const int glyph_index = font->codepoint_to_charindex_fn(c);
		if (glyph_index >= 0) {
			blit_glyph_to_cursor(&font->glyphs[glyph_index], surface, cursor, invert);
		}
		string++;
	}
}

