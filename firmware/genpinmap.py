#!/usr/bin/python3
#
#
import re
import collections

class PinmapGenerator(object):
	_ENTRY_LINE = re.compile("^\s*P(?P<port>[A-Z])(?P<pin>\d+)\s+(?P<usage>[^\s]+)\s+(?P<name>[^\s]+)")
	_PinTuple = collections.namedtuple("PinTuple", [ "port", "pin", "usage", "name", "lineno" ])

	def __init__(self, pinmap_file):
		self._pins = collections.defaultdict(list)
		self._names = set()
		with open(pinmap_file) as f:
			self._parse(f)
		self._check()

	def _parse(self, f):
		for (lineno, line) in enumerate(f, 1):
			line = line.rstrip("\r\n")
			if line.startswith("#"):
				continue
			if line == "":
				continue
			result = self._ENTRY_LINE.match(line)
			if not result:
				raise Exception("Cannot parse line %d (\"%s\")." % (lineno, line))
			result = result.groupdict()
			if result["name"] in self._names:
				raise Exception("Error: Pin name '%s' is duplicate." % (result["name"]))
			self._names.add(result["name"])
			pin = (result["port"], int(result["pin"]))
			pintuple = self._PinTuple(port = pin[0], pin = pin[1], usage = result["usage"], name = result["name"], lineno = lineno)
			self._pins[pin].append(pintuple)

	def _check(self):
		for pins in sorted(self._pins.values()):
			if len(pins) > 1:
				print("Warning: Port pin %s%d has multiple names:" % (pins[0].port, pins[0].pin))
				for pin in sorted(pins):
					print("    %s (%s) defined in line %d" % (pin.name, pin.usage, pin.lineno))
		print()

	@staticmethod
	def _print_cols(left_col, right_col, file):
		print("%s %s" % (left_col, right_col), file = file)

	def _write(self, f):
		print("#ifndef __PINMAP_H__", file = f)
		print("#define __PINMAP_H__", file = f)

		for pins in sorted(self._pins.values()):
			for pin in pins:
				print("// P%s%d: %s (%s)" % (pin.port, pin.pin, pin.name, pin.usage), file = f)
				self._print_cols("#define %s_GPIO" % (pin.name), "GPIO%s" % (pin.port), file = f)
				self._print_cols("#define %s_PinSource" % (pin.name), "GPIO_PinSource%d" % (pin.pin), file = f)
				self._print_cols("#define %s_Pin" % (pin.name), "GPIO_Pin_%d" % (pin.pin), file = f)
				self._print_cols("#define %s_SetHIGH()" % (pin.name), "%s_GPIO->BSRRL = %s_Pin" % (pin.name, pin.name), file = f)
				self._print_cols("#define %s_SetLOW()" % (pin.name), "%s_GPIO->BSRRH = %s_Pin" % (pin.name, pin.name), file = f)
				self._print_cols("#define %s_SetTo(value)" % (pin.name), "if (value) { %s_SetHIGH(); } else { %s_SetLOW(); }" % (pin.name, pin.name), file = f)
			print(file = f)

		print("#endif", file = f)

	def write_header(self, outfilename):
		with open(outfilename, "w") as f:
			self._write(f)

PinmapGenerator("pinmap.txt").write_header("pinmap.h")