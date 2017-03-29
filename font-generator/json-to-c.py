#!/usr/bin/python3
import sys
import json
import argparse
import math
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
pnm_filename = prefix + ".pnm"

with open(args.jsonfontfile) as f:
	jsonfont = json.loads(f.read())

font = Font(jsonfont["fontname"], jsonfont["fontsize"])
for glyphdata in jsonfont["glyphs"]:
	gylph = Glyph(glyphdata)
	font.add_glyph(gylph)

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


