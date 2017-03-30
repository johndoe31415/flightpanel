#!/usr/bin/python3
import sys
import json
import argparse
import math
import os
import re
import mako.lookup
from FriendlyArgumentParser import FriendlyArgumentParser
from Font import Font
from Glyph import Glyph
from PnmPicture import PnmPicture

def parse_threshold(value):
	value = int(value)
	if not (1 <= value <= 255):
		raise argparse.ArgumentError("Threshold value must be inbetween 1 and 255.")
	return value

parser = FriendlyArgumentParser()
parser.add_argument("-t", "--threshold", metavar = "value", type = parse_threshold, default = 0x60, help = "Threshold at which pixels will be considered set. Defaults to %(default)d, must be a value between 1 and 255.")
parser.add_argument("-c", "--charset", metavar = "chars", type = str, help = "Characters that should be included in the character set, concatenated to one long string. By default, all JSON characters are included.")
parser.add_argument("--filename-include-threshold", action = "store_true", help = "Include the threshold in the output filename. Useful if you create a batch of multiple images and want to judge the quality of the rasterized font by looking at the multitude of PNMs.")
parser.add_argument("--create-pnm", action = "store_true", help = "Create a PNM file with the whole character set.")
parser.add_argument("--create-text", action = "store_true", help = "Create a PNM file with the given text.")
parser.add_argument("--text", metavar = "text", help = "Render this text to the PNM file")
parser.add_argument("--text-size", metavar = "WxH", default = "128x64", help = "When writing a text file, specifies the PNM output filesize.")
parser.add_argument("-f", "--force", action = "store_true", help = "Overwrite .c, .h and .pnm files if they exist.")
parser.add_argument("-v", "--verbose", action = "store_true", help = "Increase message verbosity.")
parser.add_argument("jsonfontfile", metavar = "jsonfontfile", type = str, help = "Rasterized JSON formatted font that was output by font-rasterize.")
args = parser.parse_args(sys.argv[1:])

if args.jsonfontfile.endswith(".json"):
	prefix = args.jsonfontfile[:-5]
else:
	prefix = args.jsonfontfile
if args.filename_include_threshold:
	prefix += "-%02x" % (args.threshold)

c_filename = prefix + ".c"
h_filename = prefix + ".h"
pnm_filename = prefix + ".pnm"
txtpnm_filename = prefix + "_text.pnm"

if (not args.force) and os.path.exists(c_filename):
	print("Refusing to overwrite %s" % (c_filename))
	sys.exit(1)

if (not args.force) and os.path.exists(h_filename):
	print("Refusing to overwrite %s" % (h_filename))
	sys.exit(1)

if (not args.force) and (args.create_pnm) and os.path.exists(pnm_filename):
	print("Refusing to overwrite %s" % (pnm_filename))
	sys.exit(1)

if (not args.force) and (args.create_text) and os.path.exists(txtpnm_filename):
	print("Refusing to overwrite %s" % (txtpnm_filename))
	sys.exit(1)

with open(args.jsonfontfile) as f:
	jsonfont = json.loads(f.read())

if args.charset is None:
	charset = None
else:
	charset = set(args.charset)
font = Font(jsonfont["fontname"], jsonfont["fontsize"])
for glyphdata in jsonfont["glyphs"]:
	glyph = Glyph(glyphdata)
	if (charset is None) or (glyph.text in charset):
		font.add_glyph(glyph)

# Then assign char IDs
font.enumerate_glyphs()

if args.verbose:
	font.dump()

bitmaps = font.bitmapize(args.threshold)

if args.create_pnm:
	max_width = font.max_glyph_width
	max_height = font.max_glyph_height
	spacing = 5

	glyph_count = len(font)
	glyph_count_x = math.ceil(math.sqrt(glyph_count))
	glyph_count_y = math.ceil(glyph_count / glyph_count_x)
	assert(glyph_count_x * glyph_count_y >= glyph_count)

	total_width = max_width + spacing
	total_height = max_height + spacing
	pic = PnmPicture.new((glyph_count_x + 1) * total_width, (glyph_count_y + 1) * total_height)
	for (gcnt, (codepoint, bitmap)) in enumerate(sorted(bitmaps.items())):
		ypos = ((gcnt // glyph_count_x) + 1) * total_height
		xpos = ((gcnt % glyph_count_x) + 1) * total_width
		bitmap.blit_to(pic, xpos, ypos)
	pic.write_file(pnm_filename)

if args.create_text:
	(width, height) = (int(value) for value in args.text_size.split("x"))
	pic = PnmPicture.new(width, height)
	(x, y) = (3, height - 4)
	for char in args.text:
		cp = ord(char)
		if cp in bitmaps:
			bitmap = bitmaps[cp]
			bitmap.blit_to(pic, x, y)
			x += bitmap.glyph.xadvance
	pic.write_file(txtpnm_filename)


class TemplateHelper(object):
	def __init__(self, font, bitmaps, h_filename):
		self._font = font
		self._bitmaps = bitmaps
		self._h_filename = h_filename

	@property
	def font(self):
		return self._font

	@property
	def bitmaps(self):
		yield from sorted(self._bitmaps.items())

	@property
	def h_filename(self):
		return self._h_filename

	@property
	def includeguard_symbol(self):
		return "__" + re.sub("[^A-Z0-9]", "_", self._h_filename.upper()) + "__"

	@property
	def symbol(self):
		return "font_" + re.sub("[^a-z0-9]", "_", self._h_filename.lower()[:-2])

	@staticmethod
	def hexarray(data):
		return "{ " + ", ".join("0x%02x" % (c) for c in data) + " }"

	@staticmethod
	def ifcond(varname, start, end):
		if start == end:
			return "%s == %d" % (varname, start)
		else:
			return "(%s >= %d) && (%s <= %d)" % (varname, start, varname, end)

		return "{ " + ", ".join("0x%02x" % (c) for c in data) + " }"

helper = TemplateHelper(font, bitmaps, h_filename = h_filename)
lookup = mako.lookup.TemplateLookup([ "." ], strict_undefined = True)
with open(c_filename, "w") as f:
	f.write(lookup.get_template("template_glyph.c").render(font = helper))
with open(h_filename, "w") as f:
	f.write(lookup.get_template("template_glyph.h").render(font = helper))

