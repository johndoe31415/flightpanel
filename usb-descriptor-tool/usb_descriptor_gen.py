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

from GeneralEnums import *
from SpecificEnums import *
from DescriptorParser import DescriptorParser

class USBHidReportDescriptor(object):
	def __init__(self):
		self._data = [ ]
		self._offset = 0
		self._last_report_count = None
		self._last_report_size = None
		self._report_length = 0
		self._comments = { }

	@property
	def comments(self):
		return self._comments

	def _append(self, data, comment = None):
		if comment is not None:
			self._comments[len(self._data)] = comment
		if self._offset == 0:
			self._data += data
		else:
			for value in data:
				self._data.insert(self._offset, value)

	def _at_offset(self, index):
		clone = USBHidReportDescriptor()
		clone._data = self._data
		clone._offset += index
		return clone

	def _add_item(self, enum, data, comment = None):
		value = int(enum) << 2
		if data is None:
			encoding = [ ]
		elif 0 <= data <= 255:
			value |= 0b01
			encoding = [ data ]
		elif 256 <= data <= 65535:
			value |= 0b10
			encoding = [ (data >> 0) & 0xff, (data >> 8) & 0xff ]
		else:
			raise Exception("Unsupported.")
		self._append([ value ] + encoding, comment = comment)
		return self

	def add_usage_page(self, usage_page):
		self._add_item(Item.UsagePage, usage_page)
		return self

	def add_usage(self, usage, comment = None):
		self._add_item(Item.Usage, usage, comment = comment)
		return self

	def add_collection(self, collection_type):
		self._append([ 0xa1, int(collection_type), 0xc0 ])
		return self._at_offset(-1)

	def add_usage_minimum(self, min_value):
		self._add_item(Item.UsageMinimum, min_value)
		return self

	def add_usage_maximum(self, max_value):
		self._add_item(Item.UsageMaximum, max_value)
		return self

	def add_report_count(self, report_count):
		self._last_report_count = report_count
		self._add_item(Item.ReportCount, report_count)
		return self

	def add_report_size(self, report_size, round_up = True):
		if round_up:
			report_size = (report_size + 7) // 8 * 8
		self._last_report_size = report_size
		self._add_item(Item.ReportSize, report_size)
		return self

	def add_report_id(self, report_id):
		self._add_item(Item.ReportID, report_id)
		return self

	def add_input(self, input_flags, comment = None):
		self._report_length += self._last_report_count * self._last_report_size
		self._add_item(Item.Input, input_flags, comment = comment)
		return self

	def add_output(self, output_flags, comment = None):
		self._report_length += self._last_report_count * self._last_report_size
		self._add_item(Item.Output, output_flags, comment = comment)
		return self

	def add_logical_minimum(self, min_value):
		self._add_item(Item.LogicalMinimum, min_value)
		return self

	def add_logical_maximum(self, max_value):
		self._add_item(Item.LogicalMaximum, max_value)
		return self

	def add_unit(self, unit):
		self._add_item(Item.Unit, unit)
		return self

	def add_unit_exponent(self, unit_exponent):
		self._add_item(Item.UnitExponent, unit_exponent)
		return self

	def add_padding_bits(self, count, comment = None):
		self.add_report_count(1)
		self.add_report_size(count, round_up = False)
		self.add_input(InputOutputFeatureFlags.Constant, comment = comment)

	def add_pushbuttons(self, count, start_button = 1, comment = None):
		self.add_usage_page(UsagePage.Button)
		self.add_usage_minimum(start_button)
		self.add_usage_maximum(start_button + count - 1)
		self.add_logical_minimum(0)
		self.add_logical_maximum(1)
		self.add_report_count(count)
		self.add_report_size(1, round_up = False)
		self.add_input(InputOutputFeatureFlags.Variable, comment)
		return self

	def add_output_bytes(self, count, comment = None):
		self.add_report_count(count)
		self.add_report_size(8)
		self.add_output(InputOutputFeatureFlags.Variable, comment = comment)

	def fp_add_as_button(self, text, button_count, start_button = 1):
		padding_bits = 8 - (button_count % 8)
		self.add_pushbuttons(button_count, start_button, comment = text)
		if padding_bits != 8:
			self.add_padding_bits(padding_bits, comment = "Padding (%d bit)" % (padding_bits))

	def fp_add_items(self, items):
		def _emit(values):
			if len(values) == 0:
				return
			collection.add_usage_page(UsagePage.SimulationControls)
			for (text, size_bytes) in values:
				collection.add_usage(SimulationControls.FlightCommunication, comment = text)

			size_bytes = values[0][1]
			collection.add_logical_minimum(0)
			collection.add_logical_maximum((256 ** size_bytes) - 1)
			collection.add_report_count(len(values))
			collection.add_report_size(size_bytes * 8)
			collection.add_input(InputOutputFeatureFlags.Variable | InputOutputFeatureFlags.Volatile)

		similar = [ ]
		for (text, byte_length) in items:
			if (len(similar) != 0) and (similar[0][1] != byte_length):
				_emit(similar)
				similar = [ ]
			similar.append((text, byte_length))
		_emit(similar)

	@property
	def report_length(self):
		return self._report_length

	def __bytes__(self):
		return bytes(self._data)


hid_report = USBHidReportDescriptor()
hid_report.add_usage_page(UsagePage.GenericDesktop)
# Logical min 0?
hid_report.add_usage(GenericDesktop.Joystick)
collection = hid_report.add_collection(Collection.Application)

collection.add_report_id(1)
collection.fp_add_items([
	("Sequence number", 1),
	("Radio panel", 1),
	("COM divisions", 1),
	("NAV divisions", 1),
	("TX radio ID", 1),
	("DME nav ID", 1),
	("COM1 frequency active index", 2),
	("COM1 frequency standby index", 2),
	("COM2 frequency active index", 2),
	("COM2 frequency standby index", 2),
	("NAV1 frequency active index", 2),
	("NAV1 frequency standby index", 2),
	("NAV1 obs", 2),
	("NAV2 frequency active index", 2),
	("NAV2 frequency standby index", 2),
	("NAV2 obs", 2),
	("XPDR state", 1),
	("XPDR squawk", 2),
	("ADF frequency", 2),
	("AP state", 2),
	("AP altitude", 2),
	("AP heading", 2),
	("AP IAS", 2),
	("AP climbrate", 2),
	("Flip switches", 1),
	("QNH", 2),
	("Nav by GPS", 1),
])

collection.add_report_id(2)
collection.add_usage_page(UsagePage.GenericDesktop)
collection.add_usage(GenericDesktop.Undefined)
collection.add_output_bytes(44)

collection.add_report_id(3)
collection.add_usage_page(UsagePage.GenericDesktop)
collection.add_usage(GenericDesktop.Undefined)
collection.add_output_bytes(27)

data = bytes(hid_report)
print("// " + data.hex())
print("// Report length %d bits = %d bytes" % (collection.report_length, (collection.report_length + 7) // 8))
print()
print("static uint8_t HIDReportDescriptor[] = {")
DescriptorParser(base_indent = 1).dump(data, comments = hid_report.comments)
print("};")
