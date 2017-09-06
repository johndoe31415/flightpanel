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

import os
import re
import sys
import collections

class ParseException(Exception): pass

class PinmapGenerator(object):
	_KNOWN_ATTRS = set([ "init", "pulldown", "activelow", "speed", "af", "comment", "connect" ])
	_ENTRY_LINE = re.compile("^\s*P(?P<port>[A-Z])(?P<pin>\d+)\s+(?P<usage>[^\s]+)\s+(?P<name>[^\s]+)(\s*(?P<attributes>.+))?")
	_PinTuple = collections.namedtuple("PinTuple", [ "lineno", "port", "pin", "usage", "name", "attributes" ])

	def __init__(self, pinmap_file):
		self._pins = collections.defaultdict(list)
		self._names = { }
		self._groups = { }
		with open(pinmap_file) as f:
			self._parse(f)
		self._groups = self._resolve_groups()
		self._check()

	def _parse_attributes(self, attr_str):
		result = { }
		if attr_str is not None:
			for attr in attr_str.split(","):
				if "=" in attr:
					(key, value) = attr.split("=", maxsplit = 1)
				else:
					(key, value) = (attr, True)
				result[key] = value
				if key not in self._KNOWN_ATTRS:
					raise ParseException("Unknown attribute key '%s' given." % (key))
		return result

	def _parse(self, f):
		for (lineno, line) in enumerate(f, 1):
			line = line.rstrip("\r\n")
			if line.startswith("#"):
				continue
			if line == "":
				continue
			if line.startswith(":"):
				# Group definition
				(group_name, group_regex) = line[1:].split()
				self._groups[group_name] = re.compile(group_regex)
				continue
			result = self._ENTRY_LINE.match(line)
			if not result:
				raise ParseException("Cannot parse line %d, \"%s\" does not match regular expression." % (lineno, line))

			result = result.groupdict()
			attributes = self._parse_attributes(result["attributes"])

			if result["name"] in self._names:
				raise ParseException("Pin name '%s' is duplicate." % (result["name"]))
			pin = (result["port"], int(result["pin"]))
			pintuple = self._PinTuple(port = pin[0], pin = pin[1], usage = result["usage"], name = result["name"], lineno = lineno, attributes = attributes)
			self._names[result["name"]] = pintuple
			self._pins[pin].append(pintuple)

	def _resolve_groups(self):
		result = { }
		for (group_name, group_re) in self._groups.items():
			matching_pins = [ self._names[name] for name in self._names if group_re.fullmatch(name) is not None ]
			result[group_name] = matching_pins
		return result

	def _check(self):
		error = False
		for pins in sorted(self._pins.values()):
			if len(pins) > 1:
				print("Warning: Port pin P%s%d has multiple names:" % (pins[0].port, pins[0].pin))
				for pin in sorted(pins):
					print("    %s (%s) defined in line %d" % (pin.name, pin.usage, pin.lineno))
		for ((port, pinno), pins) in sorted(self._pins.items()):
			count_of_init = sum(bool(pin.attributes.get("init")) for pin in pins)
			if count_of_init > 1:
				error = True
				print("Fatal error: pingroup P%s%d has multiple entries that have the 'init' attribute set." % (port, pinno))
				for pin in sorted(pins):
					print("    %s (%s) defined in line %d" % (pin.name, pin.usage, pin.lineno))
		for (group_name, group_members) in self._groups.items():
			if len(group_members) == 0:
				print("Warning: Group '%s' has no members." % (group_name))
		if error:
			print()
			print("Sanity check had errors, bailing out.")
			sys.exit(1)

	@staticmethod
	def _print_cols(left_col, right_col, file):
		tab_width = 4
		get_to_tabcol = 12
		left_len = len(left_col)
		tab_count = get_to_tabcol - (left_len // tab_width)

		if tab_count < 1:
			tab_count = 1
		tab_spacing = "\t" * tab_count
		print("%s%s%s" % (left_col, tab_spacing, right_col), file = file)

	def _write_header(self, f):
		if os.path.isfile("pinmap_header.h"):
			with open("pinmap_header.h") as hdrf:
				f.write(hdrf.read())
		print("#ifndef __PINMAP_H__", file = f)
		print("#define __PINMAP_H__", file = f)
		print(file = f)
		print("#include <stm32f4xx_gpio.h>", file = f)
		print("#include \"timer.h\"", file = f)
		print(file = f)
		print("struct gpio_definition_t {", file = f)
		print("	const char *pin_name;", file = f)
		print("	const char *name;", file = f)
		print("	GPIO_TypeDef *gpio;", file = f)
		print("	int pin;", file = f)
		print("	const char *comment;", file = f)
		print("	const char *connect;", file = f)
		print("};", file = f)
		print(file = f)
		for pins in sorted(self._pins.values()):
			for pin in sorted(pins):
				print("// P%s%d: %s (%s) -- %s" % (pin.port, pin.pin, pin.name, pin.usage, sorted(pin.attributes)), file = f)
				self._print_cols("#define %s_GPIO" % (pin.name), "GPIO%s" % (pin.port), file = f)
				self._print_cols("#define %s_PinSource" % (pin.name), "GPIO_PinSource%d" % (pin.pin), file = f)
				self._print_cols("#define %s_Pin" % (pin.name), "GPIO_Pin_%d" % (pin.pin), file = f)
				self._print_cols("#define %s_set_HIGH()" % (pin.name), "%s_GPIO->BSRRL = %s_Pin" % (pin.name, pin.name), file = f)
				self._print_cols("#define %s_set_LOW()" % (pin.name), "%s_GPIO->BSRRH = %s_Pin" % (pin.name, pin.name), file = f)
				if pin.attributes.get("activelow"):
					self._print_cols("#define %s_set_ACTIVE()" % (pin.name), "%s_set_LOW()" % (pin.name), file = f)
					self._print_cols("#define %s_set_INACTIVE()" % (pin.name), "%s_set_HIGH()" % (pin.name), file = f)
					self._print_cols("#define %s_get()" % (pin.name), "(!(%s_GPIO->IDR & %s_Pin))" % (pin.name, pin.name), file = f)
				else:
					self._print_cols("#define %s_set_ACTIVE()" % (pin.name), "%s_set_HIGH()" % (pin.name), file = f)
					self._print_cols("#define %s_set_INACTIVE()" % (pin.name), "%s_set_LOW()" % (pin.name), file = f)
					self._print_cols("#define %s_get()" % (pin.name), "(%s_GPIO->IDR & %s_Pin)" % (pin.name, pin.name), file = f)
				self._print_cols("#define %s_set(value)" % (pin.name), "if (value) { %s_set_ACTIVE(); } else { %s_set_INACTIVE(); }" % (pin.name, pin.name), file = f)
				self._print_cols("#define %s_toggle()" % (pin.name), "%s_GPIO->ODR ^= %s_Pin" % (pin.name, pin.name), file = f)
				self._print_cols("#define %s_pulse()" % (pin.name), "do { %s_set_ACTIVE(); delay_loopcnt(LOOPCOUNT_50NS); %s_set_INACTIVE(); } while (0)" % (pin.name, pin.name), file = f)
				self._print_cols("#define %s_npulse()" % (pin.name), "do { %s_set_INACTIVE(); delay_loopcnt(LOOPCOUNT_50NS); %s_set_ACTIVE(); } while (0)" % (pin.name, pin.name), file = f)

				structure_members = {
					"name":		"\"%s\"" % (pin.name),
					"pin_name":	"\"%s%d\"" % (pin.port, pin.pin),
					"gpio":		"%s_GPIO" % (pin.name),
					"pin":		"%s_Pin" % (pin.name),
				}
				if "comment" in pin.attributes:
					structure_members["comment"] = "\"%s\"" % (pin.attributes["comment"])
				if "connect" in pin.attributes:
					structure_members["connect"] = "\"%s\"" % (pin.attributes["connect"])
				structure = ", ".join(".%s = %s" % (key, value) for (key, value) in sorted(structure_members.items()))
				self._print_cols("#define %s_GPIO_Definition" % (pin.name), "{ %s }" % (structure), file = f)
			print(file = f)
		for (group_name, group_members) in self._groups.items():
			print("// %s: Group of %d pins (%s)" % (group_name, len(group_members), ", ".join(member.name for member in sorted(group_members))), file = f)

			active_high = collections.defaultdict(list)
			active_low = collections.defaultdict(list)
			for member in group_members:
				if member.attributes.get("activelow"):
					active_low[member.port].append(member)
				else:
					active_high[member.port].append(member)

			active_commands = [ ]
			inactive_commands = [ ]
			for (port, pins) in sorted(active_low.items()):
				active_commands.append("GPIO%s->BSRRH = %s" % (port, " | ".join("%s_Pin" % (pin.name) for pin in sorted(pins))))
				inactive_commands.append("GPIO%s->BSRRL = %s" % (port, " | ".join("%s_Pin" % (pin.name) for pin in sorted(pins))))
			for (port, pins) in sorted(active_high.items()):
				active_commands.append("GPIO%s->BSRRL = %s" % (port, " | ".join("%s_Pin" % (pin.name) for pin in sorted(pins))))
				inactive_commands.append("GPIO%s->BSRRH = %s" % (port, " | ".join("%s_Pin" % (pin.name) for pin in sorted(pins))))
			active_commands = "; ".join(active_commands)
			inactive_commands = "; ".join(inactive_commands)
			self._print_cols("#define %s_set_ACTIVE()" % (group_name), "do { %s; } while (0)" % (active_commands), file = f)
			self._print_cols("#define %s_set_INACTIVE()" % (group_name), "do { %s; } while (0)" % (inactive_commands), file = f)
			print(file = f)

		print("void pinmap_initialize(void);", file = f)
		print(file = f)

		print("#endif", file = f)

	def _write_source(self, f):
		print("#include \"pinmap.h\"", file = f)
		print(file = f)
		print("void pinmap_initialize(void) {", file = f)
		print("}", file = f)

	def write_header(self, outfilename):
		with open(outfilename, "w") as f:
			self._write_header(f)

	def write_source(self, outfilename):
		with open(outfilename, "w") as f:
			self._write_source(f)


pmgen = PinmapGenerator("pinmap.txt")
pmgen.write_header("pinmap.h")
pmgen.write_source("pinmap.c")

