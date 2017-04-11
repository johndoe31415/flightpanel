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

import enum

class UsagePage(enum.IntEnum):
	GenericDesktop = 1
	SimulationControls = 2
	VRControls = 3
	SportsControls = 4
	GameControls = 5
	GenericDeviceControls = 6
	Keyboard = 7
	LED = 8
	Button = 9
	Ordinal = 0xa
	Telephony = 0xb
	Consumer = 0xc
	Digitizers = 0xd
	Unicode = 0x10
	Alphanumeric = 0x14
	Merdical = 0x40

class Collection(enum.IntEnum):
	Physical = 0
	Application = 1
	Logical = 2
	Report = 3
	NamedArray = 4
	UsageSwitch = 5
	UsageModifier = 6

class InputOutputFeatureFlags(enum.IntEnum):
	NotData =  (1 << 0)
	Constant = (1 << 0)

	NotArray = (1 << 1)
	Variable = (1 << 1)

	NotAbsolute = (1 << 2)
	Relative = (1 << 2)

	Wraps = (1 << 3)
	NonLinear = (1 << 4)
	NoPreferredState = (1 << 5)
	NullState = (1 << 6)
	Volatile = (1 << 7)
	NotBitField = (1 << 8)
	BufferedBytes = (1 << 8)


class Item(enum.IntEnum):
	# Main Items
	Input = 0b100000
	Output = 0b100100
	Feature = 0b101100
	Collection = 0b101000
	EndCollection = 0b110000

	# Global Items
	UsagePage = 0b000001
	LogicalMinimum = 0b000101
	LogicalMaximum = 0b001001
	PhysicalMinimum = 0b001101
	PhysicalMaximum = 0b010001
	UnitExponent = 0b010101
	Unit = 0b011001
	ReportSize = 0b011101
	ReportID = 0b100001
	ReportCount = 0b100101
	Push = 0b101001
	Pop = 0b101101

	# Local Items
	Usage = 0b000010
	UsageMinimum = 0b000110
	UsageMaximum = 0b001010
	DesignatorIndex = 0b001110
	DesignatorMinimum = 0b010010
	DesignatorMaximum = 0b010110
	StringIndex = 0b011110
	StringMinimum = 0b100010
	StringMaximum = 0b100110

