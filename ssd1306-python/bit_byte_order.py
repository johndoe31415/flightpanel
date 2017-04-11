#!/usr/bin/python3
#
#	flightpanel - A Cortex-M4 based USB flight panel for flight simulators.
#	Copyright (C) 2017-2017 Johannes Bauer
#
#	This file is part of flightpanel.
#
#	flightpanel is free software; you can redistribute it and/or modify
#	it under the terms of the GNU General Public License as published by
#	the Free Software Foundation; this program is ONLY licensed under
#	version 3 of the License, later versions are explicitly excluded.
#
#	flightpanel is distributed in the hope that it will be useful,
#	but WITHOUT ANY WARRANTY; without even the implied warranty of
#	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#	GNU General Public License for more details.
#
#	You should have received a copy of the GNU General Public License
#	along with flightpanel; if not, write to the Free Software
#	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
#
#	Johannes Bauer <JohannesBauer@gmx.de>

width = 48
height = 16
#accessor = "SURFACE_ACCESSOR_LR_TB_ROWBYTES"
accessor = "SURFACE_ACCESSOR_LR_TB_COLBYTES"

print("%d x %d display with %s accessor" % (width, height, accessor))
print()

if accessor == "SURFACE_ACCESSOR_LR_TB_ROWBYTES":
	def get_byte(x, y):
		return (x // 8) + (y * (width // 8))

	def get_bit(x, y):
		return x % 8
elif accessor == "SURFACE_ACCESSOR_LR_TB_COLBYTES":
	def get_byte(x, y):
		return (y // 8 * width) + (x % width)

	def get_bit(x, y):
		return y % 8
else:
	raise Exception(NotImplemented)

for fnc in [ get_byte, get_bit ]:
	for y in range(height):
		line = ""
		for x in range(width):
			value = fnc(x, y)
			line += "%3d" % (value)
		print(line)
	print()
