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

import subprocess
import re
import collections

class ELFSymbols(object):
	_SYMBOL_RE = re.compile("(?P<addr>[a-f0-9]+)\s+(?P<size>[a-f0-9]+)\s+(?P<type>.)\s+(?P<name>.*)")
	_Symbol = collections.namedtuple("Symbol", [ "addr", "size", "symtype", "name" ])

	def __init__(self, toolchain_prefix, binary, verbose = False):
		self._toolchain_prefix = toolchain_prefix
		self._binary = binary
		self._verbose = verbose
		self._symbols = collections.defaultdict(list)
		self._parse()

	def _parse(self):
		symbol_re = re.compile("(?P<addr>[a-f0-9]+)\s+(?P<size>[a-f0-9]+)\s+(?P<type>.)\s+(?P<name>.*)$")
		symbols = subprocess.check_output([ self._toolchain_prefix + "nm", "-S", self._binary ])
		for line in symbols.decode().split("\n"):
			result = self._SYMBOL_RE.fullmatch(line)
			if result is None:
				if self._verbose:
					print("No match for line: %s" % (line))
				continue

			result = result.groupdict()
			symbol = self._Symbol(addr = int(result["addr"], 16), size = int(result["size"], 16), symtype = result["type"], name = result["name"])
			self._symbols[symbol.addr].append(symbol)

	def dump(self):
		for (addr, symbols) in sorted(self._symbols.items()):
			for symbol in symbols:
				print("0x%8x %d %s" % (symbol.addr, symbol.size, symbol.name))

	def __iter__(self):
		return iter(self._symobls.values())
