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

