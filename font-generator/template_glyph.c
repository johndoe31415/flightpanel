#include <stdint.h>
#include "font.h"
#include "${font.h_filename}"

static int codepoint_to_charindex(int codepoint) {
	%for (id, (start, end, difference)) in enumerate(font.font.charindex_to_glyph_mapping()):
	%if id == 0:
	if (${font.ifcond("codepoint", start, end)}) {
		return codepoint - ${difference};
	%else:
	} else if (${font.ifcond("codepoint", start, end)}) {
		return codepoint - ${difference};
	%endif
	%endfor
	} else {
		return -1;
	}
}

const struct font_t ${font.symbol} = {
	.codepoint_to_charindex_fn = codepoint_to_charindex,
	.glyphs = {
%for (codepoint, bitmap) in font.bitmaps:
		[${bitmap.glyph.charindex}] = { // Codepoint ${codepoint} ("${chr(codepoint)}"), char index ${bitmap.glyph.charindex}
			.xadvance = ${bitmap.glyph.xadvance},
			.xoffset = ${bitmap.glyph.xoffset},
			.yoffset = ${bitmap.glyph.yoffset},
			.width = ${bitmap.glyph.width},
			.height = ${bitmap.glyph.height},
			.data = (const uint8_t[]) ${font.hexarray(bitmap.data)},
		},

%endfor
	}
};

