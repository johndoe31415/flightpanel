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

	def __len__(self):
		return len(self._glyphs)

	def __str__(self):
		return "Font<%d glyphs>" % (len(self))
