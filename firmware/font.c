#include "font.h"
#include "surface.h"

static void blit_glyph(const struct glyph_t *glyph, const struct surface_t *surface, int x0, int y0) {
	const int glyph_rowwidth = (glyph->width + 7) / 8;
	for (int y = 0; y < glyph->height; y++) {
		for (int x = 0; x < glyph->width; x++) {
			const int byte_offset = (x / 8) + (y * glyph_rowwidth);
			const int bit_offset = x % 8;
			const bool pixel = ((glyph->data[byte_offset] >> bit_offset) & 1) != 0;
			if (pixel) {
				surface_setpixel(surface, x0 + x + glyph->xoffset, y0 + y + glyph->yoffset);
			}
		}
	}
}

static void blit_glyph_to_cursor(const struct glyph_t *glyph, const struct surface_t *surface, struct cursor_t *cursor) {
	blit_glyph(glyph, surface, cursor->x, cursor->y);
	cursor->x += glyph->xadvance;
}

void blit_string_to_cursor(const struct font_t *font, const char *string, const struct surface_t *surface, struct cursor_t *cursor) {
	while (*string) {
		char c = *string;
		int glyph_index = font->codepoint_to_charindex_fn(c);
		if (glyph_index >= 0) {
			blit_glyph_to_cursor(&font->glyphs[glyph_index], surface, cursor);
		}
		string++;
	}
}

#ifdef TEST_FONT
// gcc -O2 -Wall -std=c11 -DTEST_FONT -o testfont font.c vcr-osd-mono-20.c surface.c && ./testfont

#include "vcr-osd-mono-20.h"
#include "surface.h"

#define TEST_WIDTH		64
#define TEST_HEIGHT		32

static uint8_t surface_data[TEST_WIDTH * TEST_HEIGHT / 8];
static const struct surface_t surface = {
	.width = TEST_WIDTH,
	.height = TEST_HEIGHT,
	.data = surface_data,
};

int main(void) {
	surface_clear(&surface);
	struct cursor_t cursor = { 0, 20 };

	blit_string_to_cursor(&font_vcr_osd_mono_20, "123.456", &surface, &cursor);
	surface_dump(&surface);
	return 0;
}
#endif
