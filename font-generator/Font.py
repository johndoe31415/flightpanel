class Font(object):
	def __init__(self, name, size):
		self._name = name
		self._size = size
		self._glyphs = { }

	def add_glyph(self, glyph):
		if glyph.codepoint in self._glyphs:
			raise Exception("Glyph codepoint %d already present in font." % (glyph.codepoint))
		self._glyphs[glyph.codepoint] = glyph

	def dump(self):
		for (codepoint, glyph) in sorted(self._glyphs.items()):
			print(glyph)

	def bitmapize(self, threshold):
		bitmaps = { }
		for (codepoint, glyph) in sorted(self._glyphs.items()):
			bitmap = glyph.bitmapize(threshold)
			bitmaps[codepoint] = bitmap
		return bitmaps

	@property
	def max_glyph_width(self):
		return max(glyph.width for glyph in self._glyphs.values())

	@property
	def max_glyph_height(self):
		return max(glyph.height for glyph in self._glyphs.values())

	def enumerate_glyphs(self):
		for (charindex, (codepoint, glyph)) in enumerate(self):
			glyph.charindex = charindex

	def charindex_to_glyph_mapping(self):
		cp_diff = [ (codepoint, codepoint - charindex) for (charindex, (codepoint, glyph)) in enumerate(self) ]
		ranges = [ ]
		for (codepoint, difference) in cp_diff:
			if (len(ranges) == 0) or (difference != ranges[-1][2]):
				ranges.append([ codepoint, codepoint, difference ])
			else:
				ranges[-1][1] = codepoint
		return ranges

	def __iter__(self):
		return iter(sorted(self._glyphs.items()))

	def __len__(self):
		return len(self._glyphs)

	def __str__(self):
		return "Font<%d glyphs>" % (len(self))
