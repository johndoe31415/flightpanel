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

class Register(object):
	def __init__(self, offset):
		self._offset = offset

	@property
	def addr(self):
		return self._offset

	def displace(self, displacement):
		return self.__class__(self.addr + displacement)

	@property
	def addr(self):
		return self._offset

class BaseRegisterMap(Register):
	_FIELDS = None

	def __init__(self, offset = 0):
		if self.__class__ == BaseRegisterMap:
			raise Exception("Attempted to instanciated abstract base class.")

		Register.__init__(self, offset)
		if self._FIELDS is None:
			fields = { }
			for (name, value) in self._get_fields():
				if isinstance(value, int):
					fields[name] = Register(value)
				else:
					fields[name] = value

			# Remove from attributes so that we can use __getattr__
			for name in fields:
				delattr(self.__class__, name)
			self.__class__._FIELDS = fields

	def _get_fields(self):
		for name in dir(self):
			if (name in [ "addr", "displace" ]) or name.startswith("_"):
				continue
			yield (name, getattr(self, name))

	def __getitem__(self, name):
		if name not in self._FIELDS:
			raise KeyError("Register map '%s' has no member called %s, only %s." % (self.__class__.__name__, name, ", ".join(sorted(self._FIELDS.keys()))))
		field = self._FIELDS[name]
		return field.displace(self.addr)

	def __getattr__(self, name):
		return self[name]

if __name__ == "__main__":
	class GpioMap(BaseRegisterMap):
		FOO = 0x0
		BAR = 0x4
		KOO = 0x8

	class RegisterMap(BaseRegisterMap):
		GPIOA = GpioMap(0x10000)
		GPIOB = GpioMap(0x20000)
		GPIOC = GpioMap(0x40000)

	regs = RegisterMap(0x80000000)
	print(hex(regs.addr))
	print(hex(regs.GPIOA.addr))
	print(hex(regs.GPIOB.addr))
	print(hex(regs.GPIOB.BAR.addr))
