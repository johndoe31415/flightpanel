from GeneralEnums import *
from SpecificEnums import *

class BitFields(object):
	def __init__(self):
		pass

	def add(self, name, startbit, length):
		pass
#		print(name, startbit, length)

	def dump(self):
		pass

class DescriptorParser(object):
	__USAGE_CLASSES = {
		UsagePage.GenericDesktop:	GenericDesktop,
		UsagePage.SimulationControls:	SimulationControls,
	}
	def __init__(self, base_indent = 0):
		self._usage_page = None
		self._report_cnt = 0
		self._report_size = 0
		self._reports = [ ]
		self._indentation = "\t"
		self._tabsize = 4
		self._base_indent = base_indent

	@staticmethod
	def _decode_input_bitfield(value, only_set = False):
		bits = [ ]
		bits.append([ "Data" if (not only_set) else None, "Constant" ][bool(value & InputOutputFeatureFlags.Constant)])
		bits.append([ "Array" if (not only_set) else None, "Variable" ][bool(value & InputOutputFeatureFlags.Variable)])
		bits.append([ "Absolute" if (not only_set) else None, "Relative" ][bool(value & InputOutputFeatureFlags.Relative)])
		bits.append([ "NoWrap" if (not only_set) else None, "Wrap" ][bool(value & InputOutputFeatureFlags.Wraps)])
		bits.append([ "Linear" if (not only_set) else None, "NonLinear" ][bool(value & InputOutputFeatureFlags.NonLinear)])
		bits.append([ "PreferredState" if (not only_set) else None, "NoPreferredState" ][bool(value & InputOutputFeatureFlags.NoPreferredState)])
		bits.append([ "NoNullPosition" if (not only_set) else None, "NullState" ][bool(value & InputOutputFeatureFlags.NullState)])
		bits.append([ "NonVolatile" if (not only_set) else None, "Volatile" ][bool(value & InputOutputFeatureFlags.Volatile)])
		bits.append([ "BitField" if (not only_set) else None, "BufferedBytes" ][bool(value & InputOutputFeatureFlags.BufferedBytes)])
		return ", ".join(bit for bit in bits if (bit is not None))

	def _stringify_cmdcode(self, cmdcode, argument):
		if argument is not None:
			if cmdcode == Item.Collection:
				return "%s(%s)" % (cmdcode.name, Collection(argument).name)
			elif cmdcode == Item.UsagePage:
				self._usage_page = UsagePage(argument)
				return"%s(%s)" % (cmdcode.name, self._usage_page.name)
			elif cmdcode == Item.Usage:
				if self._usage_page in self.__USAGE_CLASSES:
					return "%s(%s)" % (cmdcode.name, self.__USAGE_CLASSES[self._usage_page](argument).name)
				else:
					return "%s(%d) ?" % (cmdcode.name, argument)

			elif cmdcode == Item.Input:
				self._reports.append((self._report_cnt, self._report_size))
				return "%s(%s)" % (cmdcode.name, self._decode_input_bitfield(argument, only_set = True))
			else:
				if cmdcode == Item.ReportSize:
					self._report_size = argument
				elif cmdcode == Item.ReportCount:
					self._report_cnt = argument
				return "%s(%d)" % (cmdcode.name, argument)
		else:
			return "%s" % (cmdcode.name)

	def _print_line(self, text, comment, target = 7):
		offset = 0
		for char in text:
			if char == "\t":
				offset = ((offset // self._tabsize) + 1) * self._tabsize
			else:
				offset += 1

		tabcnt = target - (offset // self._tabsize)
		spacer = "\t" * tabcnt
		base = "\t" * self._base_indent
		print("%s%s%s%s" % (base, text, spacer, comment))

	def _dump_cmdcode(self, indent, cmdcode, command_word, payload):
		indent_str = self._indentation * indent
		if len(payload) == 0:
			argument = None
		else:
			argument = sum(value << (8 * i) for (i, value) in enumerate(payload))
		hex_string = ", ".join("0x%02x" % (c) for c in [ command_word ] + list(payload))
		spacer = "\t\t\t"
		self._print_line("%s%s," % (indent_str, hex_string), "// %s" % (self._stringify_cmdcode(cmdcode, argument)), target = 7 + indent)
		if cmdcode == Item.Input:
			print()

	def dump(self, data):
		self._usage_page = None
		indent = 0
		offset = 0
		while offset < len(data):
			command = data[offset]
			cmdcode = (data[offset] >> 2) & 0x3f
			length = (data[offset] >> 0) & 0x03

			offset += 1
			if length == 0:
				payload = [ ]
			elif length == 1:
				payload = data[offset : offset + 1]
			elif length == 2:
				payload = data[offset : offset + 2]
			elif length == 3:
				payload = data[offset : offset + 4]
			offset += len(payload)

			cmdcode = Item(cmdcode)

			if cmdcode == Item.EndCollection:
				indent -= 1
			self._dump_cmdcode(indent, cmdcode, command, payload)
			if cmdcode == Item.Collection:
				indent += 1

		bitfields = BitFields()
		bitno = 0
		for (report_id, (report_cnt, reportsize)) in enumerate(self._reports):
			for i in range(report_cnt):
				bitfields.add("report_%d_%d" % (report_id, i), bitno, reportsize)
				bitno += reportsize
		bitfields.dump()
