#!/usr/bin/python3
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

	def _append(self, data):
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

	def _add_item(self, enum, data):
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
		self._append([ value ] + encoding)
		return self

	def add_usage_page(self, usage_page):
		self._add_item(Item.UsagePage, usage_page)
		return self

	def add_usage(self, usage):
		self._add_item(Item.Usage, usage)
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

	def add_report_size(self, report_size):
		if report_size > 1:
			# Round up everything but buttons
			report_size = (report_size + 7) // 8 * 8
		self._last_report_size = report_size
		self._add_item(Item.ReportSize, report_size)
		return self

	def add_input(self, input_flags):
		self._report_length += self._last_report_count * self._last_report_size
		self._add_item(Item.Input, input_flags)
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

	def add_pushbuttons(self, count, start_button = 1):
		self.add_usage_page(UsagePage.Button)
		self.add_usage_minimum(start_button)
		self.add_usage_maximum(start_button + count - 1)
		self.add_logical_minimum(0)
		self.add_logical_maximum(1)
		self.add_report_count(count)
		self.add_report_size(1)
		self.add_input(InputOutputFeatureFlags.Variable)
		return self

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

collection.add_usage_page(UsagePage.SimulationControls)

collection.add_usage(SimulationControls.FlightCommunication)	# VHF1
collection.add_usage(SimulationControls.FlightCommunication)	# VHF1 STBY
collection.add_usage(SimulationControls.FlightCommunication)	# VHF2
collection.add_usage(SimulationControls.FlightCommunication)	# VHF2 STBY
collection.add_usage(SimulationControls.FlightCommunication)	# NAV1
collection.add_usage(SimulationControls.FlightCommunication)	# NAV1 STBY
collection.add_usage(SimulationControls.FlightCommunication)	# NAV2
collection.add_usage(SimulationControls.FlightCommunication)	# NAV2 STBY
collection.add_logical_minimum(0)
collection.add_logical_maximum(2047)
collection.add_report_size(10)
collection.add_report_count(8)
#collection.add_unit_exponent(0)
#collection.add_unit(0)
collection.add_input(InputOutputFeatureFlags.Variable | InputOutputFeatureFlags.Volatile)

collection.add_usage(SimulationControls.FlightCommunication)	# NAV1 OBS
collection.add_usage(SimulationControls.FlightCommunication)	# NAV2 OBS
collection.add_usage(SimulationControls.FlightCommunication)	# ADF OBS
collection.add_usage(SimulationControls.FlightCommunication)	# AP HDG
collection.add_logical_minimum(0)
collection.add_logical_maximum(359)
collection.add_report_size(9)
collection.add_report_count(4)
#collection.add_unit_exponent(0)
#collection.add_unit(0)
collection.add_input(InputOutputFeatureFlags.Variable | InputOutputFeatureFlags.Volatile)

collection.add_usage(SimulationControls.FlightCommunication)	# QNH
collection.add_logical_minimum(880)
collection.add_logical_maximum(1120)
collection.add_report_size(8)
collection.add_report_count(1)
#collection.add_unit_exponent(0)
#collection.add_unit(0)
collection.add_input(InputOutputFeatureFlags.Variable | InputOutputFeatureFlags.Volatile)

collection.add_usage(SimulationControls.FlightCommunication)	# XPDR Squawk
collection.add_logical_minimum(0)
collection.add_logical_maximum(4095)
collection.add_report_size(16)
collection.add_report_count(1)
#collection.add_unit_exponent(0)
#collection.add_unit(0)
collection.add_input(InputOutputFeatureFlags.Variable | InputOutputFeatureFlags.Volatile)

# Pushbuttons:
# 7: AP Active, NAV; APR, REV, ALT, HDG, IAS
# 2: Transponder Charly, Ident
# 1: NAV/GPS
# 2: Trottle down/throttle up
# 6: Lights
# 6: Audio VHF1, VHF2, NAV1, NAV2, DME, ADF
collection.add_pushbuttons(24)
data = bytes(hid_report)
print("// " + data.hex())
print("// Report length %d bits = %d bytes" % (collection.report_length, (collection.report_length + 7) // 8))
print()
print("static uint8_t HIDReportDescriptor[] = {")
DescriptorParser(base_indent = 1).dump(data)
print("};")
