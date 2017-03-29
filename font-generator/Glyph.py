class BitmapGlyph(object):
	def __init__(self, data, glyph):
		assert(len(data) == glyph.width * glyph.height)
		self._bitmap_data = [ ]
		self._glyph = glyph

		for y in range(self._glyph.height):
			for x0 in range(0, self._glyph.width, 8):
				databyte = 0
				for xoffset in range(8):
					x = x0 + xoffset
					if x >= self._glyph.width:
						bitvalue = 0
					else:
						bitvalue = int(data[x + (y * self._glyph.width)])
					databyte = (databyte >> 1) | (0x80 * bitvalue)
				self._bitmap_data.append(databyte)

	@property
	def row_width(self):
		return (self._glyph.width + 7) // 8

	def get_pixel(self, x, y):
		byte_offset = (x // 8) + (y * self.row_width)
		bit_offset = x % 8
		return ((self._bitmap_data[byte_offset] >> bit_offset) & 1) != 0

	def print(self):
		for y in range(self._glyph.height):
			line = ""
			for x in range(self._glyph.width):
				if self.get_pixel(x, y):
					line += "⬤ "
				else:
					line += "  "
			print(line)
		print("-" * 120)

	def blit_to(self, pic, x0, y0):
		for gy in range(self._glyph.height):
			for gx in range(self._glyph.width):
				if self.get_pixel(gx, gy):
					x = x0 + self._glyph.xoffset + gx
					y = y0 + self._glyph.yoffset + gy
					pic.set_pixel(x, y, (0, 0, 0))

class Glyph(object):
	def __init__(self, metadata, codepoint = None):
		self._text = metadata["text"]
		if codepoint is None:
			self._codepoint = ord(self._text)
		else:
			self._codepoint = codepoint
		self._xoffset = metadata["xoffset"]
		self._yoffset = metadata["yoffset"]
		self._xadvance = metadata["xadvance"]
		self._width = metadata["width"]
		self._height = metadata["height"]
		self._rawdata = bytes.fromhex(metadata["data"])

	@property
	def codepoint(self):
		return self._codepoint

	@property
	def text(self):
		return self._text

	@property
	def xoffset(self):
		return self._xoffset

	@property
	def yoffset(self):
		return self._yoffset

	@property
	def xadvance(self):
		return self._xadvance

	@property
	def width(self):
		return self._width

	@property
	def height(self):
		return self._height

	@property
	def rawdata(self):
		return self._rawdata

	@property
	def bitmap_size(self):
		return ((self.width + 7) // 8) * self.height

	def bitmapize(self, threshold):
		data = [ (value >= threshold) for value in self.rawdata ]
		return BitmapGlyph(data, self)

	def __str__(self):
		return "Glyph<\"%s\", CP %d, %d x %d, %d bytes>" % (self.text, self.codepoint, self.width, self.height, self.bitmap_size)


