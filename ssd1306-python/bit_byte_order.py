#!/usr/bin/python3
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
