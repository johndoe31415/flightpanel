#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <getopt.h>
#include <cairo.h>

#include "font-rasterize.h"

#define UNSET_PIXEL_VALUE		0

static struct pgmopts_t {
	cairo_antialias_t antialias;
	double fontsize;
	const char *fontname;
	const char *outfilename;
	bool verbose;
} pgmopts = {
	.antialias = CAIRO_ANTIALIAS_BEST,
	.fontsize = 20,
	.verbose = false,
};


void cairo_font_surface_init(struct cairo_font_surface_t *cfs, const char *fontname, double fontsize) {
	memset(cfs, 0, sizeof(*cfs));
	cfs->fontname = fontname;
	cfs->fontsize = fontsize;
	int width, height;
	width = round(2 * fontsize);
	height = round(2 * fontsize);
	cfs->base_x = width / 2;
	cfs->base_y = height / 2;
	cfs->csurface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, width, height);
	cfs->cctx = cairo_create(cfs->csurface);
	cairo_set_source_rgb(cfs->cctx, 0, 0, 0);

	cairo_font_options_t *fopts = cairo_font_options_create();
	cairo_font_options_set_antialias(fopts, pgmopts.antialias);
	cairo_select_font_face(cfs->cctx, fontname ? fontname : "Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
	cairo_set_font_size(cfs->cctx, fontsize);
	cairo_set_font_options(cfs->cctx, fopts);
	cairo_font_extents(cfs->cctx, &cfs->font_extents);
}

static void cairo_font_surface_clear_surface(struct cairo_font_surface_t *cfs) {
	cairo_save(cfs->cctx);
	cairo_set_source_rgba(cfs->cctx, 0, 0, 0, 0);
	cairo_set_operator(cfs->cctx, CAIRO_OPERATOR_SOURCE);
	cairo_paint(cfs->cctx);
	cairo_restore(cfs->cctx);
}

static uint8_t compress_pixel(uint32_t value) {
	uint8_t alpha = (value >> 24) & 0xff;
	return alpha;
}

void cairo_font_surface_blit_text(struct cairo_font_surface_t *cfs, const char *text, struct glyph_t *glyph) {
	memset(glyph, 0, sizeof(*glyph));
	cairo_font_surface_clear_surface(cfs);
	glyph->text = text;
	cairo_move_to(cfs->cctx, cfs->base_x, cfs->base_y);
	cairo_text_extents(cfs->cctx, text, &glyph->text_extents);
	cairo_show_text(cfs->cctx, text);
	cairo_surface_flush(cfs->csurface);

	int width = cairo_image_surface_get_width(cfs->csurface);
	int height = cairo_image_surface_get_height(cfs->csurface);
	const uint32_t *pixel_data = (uint32_t*)cairo_image_surface_get_data(cfs->csurface);
	bool empty = true;
	int min_x = 0, min_y = 0;
	int max_x = 0, max_y = 0;
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			int offset = (y * width) + x;
			uint32_t pixel = pixel_data[offset];
			if (pixel != UNSET_PIXEL_VALUE) {
				if (empty || (x < min_x)) {
					min_x = x;
				}
				if (empty || (x > max_x)) {
					max_x = x;
				}
				if (empty || (y < min_y)) {
					min_y = y;
				}
				if (empty || (y > max_y)) {
					max_y = y;
				}
				empty = false;
			}
		}
	}

	if (empty) {
		return;
	}

	glyph->width = max_x - min_x + 1;
	glyph->height = max_y - min_y + 1;
	glyph->offset_x = min_x - cfs->base_x;
	glyph->offset_y = min_y - cfs->base_y;
	glyph->data = malloc(glyph->height * glyph->width);
	for (int y = 0; y < glyph->height; y++) {
		for (int x = 0; x < glyph->width; x++) {
			int orig_x = min_x + x;
			int orig_y = min_y + y;
			int orig_offset = (orig_y * width) + orig_x;
			int offset = (y * glyph->width) + x;
			uint32_t pixel = pixel_data[orig_offset];
			glyph->data[offset] = compress_pixel(pixel);
		}
	}
}

void cairo_glyph_to_json(FILE *f, const struct glyph_t *glyph) {
	fprintf(f, "{");
	fprintf(f, "\"text\": \"");
	int l = strlen(glyph->text);
	for (int i = 0; i < l; i++) {
		char c = glyph->text[i];
		if (c == '\\') {
			fprintf(f, "\\\\");
		} else if (c == '"') {
			fprintf(f, "\\\"");
		} else {
			fprintf(f, "%c", c);
		}
	}
	fprintf(f, "\", ");
	fprintf(f, "\"width\": %d, ", glyph->width);
	fprintf(f, "\"height\": %d, ", glyph->height);
	fprintf(f, "\"xoffset\": %d, ", glyph->offset_x);
	fprintf(f, "\"yoffset\": %d, ", glyph->offset_y);
	fprintf(f, "\"xadvance\": %d, ", (int)round(glyph->text_extents.x_advance));
	fprintf(f, "\"data\": \"");
	for (int i = 0; i < glyph->width * glyph->height; i++) {
		fprintf(f, "%02x", glyph->data[i]);
	}
	fprintf(f, "\"");
	fprintf(f, "}");
}

void cairo_print_glyph(const struct glyph_t *glyph) {
	for (int y = 0; y < glyph->height; y++) {
		for (int x = 0; x < glyph->width; x++) {
			int offset = (y * glyph->width) + x;
			uint8_t pixel = glyph->data[offset];
			printf("%s", (pixel > 0x60) ? "⬤ " : "  ");
		}
		printf("\n");
	}
	printf("\n");
}

void cairo_free_glyph(struct glyph_t *glyph) {
	free(glyph->data);
	memset(glyph, 0, sizeof(*glyph));
}

static void show_syntax(const char *pgmname, const char *errmsg) {
	if (errmsg) {
		fprintf(stderr, "error: %s\n\n", errmsg);
	}
	fprintf(stderr, "%s [--antialias=default|none|gray|subpixel|fast|good|best] [-s fontsize] (fontname) (outfile.json)\n", pgmname);
	fprintf(stderr, "\n");
	fprintf(stderr, "  -a, --antialias=VALUE   Set the Cairo antialiasing mode for fonts to either default, none, gray,\n");
	fprintf(stderr, "                          subpixel, fast, good or best.");
	fprintf(stderr, "  -s, --fontsize SIZE     Set the font size which is used.\n");
	fprintf(stderr, "  -v                      Increase verbosity durign generation of rendered font.\n");
	fprintf(stderr, "  (fontname)              Name of the font that should be rendered.\n");
	fprintf(stderr, "  (outfile.json)          Name of the output JSON file.\n");
}

static bool parse_cmdline_options(int argc, char **argv) {
	const struct option long_options[] = {
		{ "antialias",	required_argument,	0, 'a' },
		{ "fontsize",	required_argument,	0, 's' },
		{ "verbose",	no_argument,		0, 'v' },
		{ 0 }
	};
	int opt;
	while ((opt = getopt_long(argc, argv, "a:s:v", long_options, NULL)) != -1) {
		switch (opt) {
			case 'a':
				if (!strcmp(optarg, "default")) {
					pgmopts.antialias = CAIRO_ANTIALIAS_DEFAULT;
				} else if (!strcmp(optarg, "none")) {
					pgmopts.antialias = CAIRO_ANTIALIAS_NONE;
				} else if (!strcmp(optarg, "gray")) {
					pgmopts.antialias = CAIRO_ANTIALIAS_GRAY;
				} else if (!strcmp(optarg, "subpixel")) {
					pgmopts.antialias = CAIRO_ANTIALIAS_SUBPIXEL;
				} else if (!strcmp(optarg, "fast")) {
					pgmopts.antialias = CAIRO_ANTIALIAS_FAST;
				} else if (!strcmp(optarg, "good")) {
					pgmopts.antialias = CAIRO_ANTIALIAS_GOOD;
				} else if (!strcmp(optarg, "best")) {
					pgmopts.antialias = CAIRO_ANTIALIAS_BEST;
				} else {
					show_syntax(argv[0], "argument given to option \"antialias\" is unknown.");
					exit(EXIT_FAILURE);
				}
				break;

			case 's':
				pgmopts.fontsize = atof(optarg);
				break;

			case 'v':
				pgmopts.verbose = true;
				break;
		}
	}

	if (optind + 2 != argc) {
		show_syntax(argv[0], "expected exactly two positional arguments.");
		exit(EXIT_FAILURE);
	}
	pgmopts.fontname = argv[optind];
	pgmopts.outfilename = argv[optind + 1];
	return true;
}

int main(int argc, char **argv) {
	if (!parse_cmdline_options(argc, argv)) {
		show_syntax(argv[0], NULL);
		exit(EXIT_FAILURE);
	}

	struct cairo_font_surface_t cfs;
	cairo_font_surface_init(&cfs, pgmopts.fontname, pgmopts.fontsize);

	FILE *f = fopen(pgmopts.outfilename, "w");

	fprintf(f, "{\n");
	fprintf(f, "	\"fontname\": \"%s\",\n", pgmopts.fontname);
	fprintf(f, "	\"fontsize\": \"%.3f\",\n", pgmopts.fontsize);
	fprintf(f, "	\"glyphs\": [\n\t\t");

	for (int i = 32; i < 127; i++) {
		char text[4];
		text[0] = i;
		text[1] = 0;
		struct glyph_t glyph;
		cairo_font_surface_blit_text(&cfs, text, &glyph);
		cairo_glyph_to_json(f, &glyph);
		if (pgmopts.verbose) {
			cairo_print_glyph(&glyph);
		}
		fprintf(f, ",\n\t\t");
		cairo_free_glyph(&glyph);
	}

	const char *special_chars[] = {
		"ä", "Ä",
		"ö", "Ö",
		"ü", "Ü",
		"ß",
	};
	const int special_char_cnt = sizeof(special_chars) / sizeof(char*);
	for (int i = 0; i < special_char_cnt; i++) {
		struct glyph_t glyph;
		cairo_font_surface_blit_text(&cfs, special_chars[i], &glyph);
		cairo_glyph_to_json(f, &glyph);
		if (pgmopts.verbose) {
			cairo_print_glyph(&glyph);
		}
		if (i != special_char_cnt - 1) {
			fprintf(f, ",\n\t\t");
		}
		cairo_free_glyph(&glyph);
	}

	fprintf(f, "\n\t]\n}\n");
	fclose(f);
}
