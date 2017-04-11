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

import serial
import time
import random
from PnmPicture import PnmPicture

class Conn(object):
	def __init__(self):
		self._conn = serial.Serial("/dev/ttyUSB0", baudrate = 115200)
		self._value = 0
		self._set_to(self._value)

	def _set_reset(self, bit, value):
		if value:
			self._value |= bit
		else:
			self._value &= ~bit
		self._set_to(self._value)

	def _set_to(self, value):
		assert(0 <= value <= 15)
		i = value + ord("a")
		self._conn.write(chr(i).encode())

	def _pin_clk(self, value):
		self._set_reset(1, value)

	def _pin_data(self, value):
		self._set_reset(2, value)

	def _pin_dc(self, value):
		self._set_reset(8, value)

	def _pin_reset(self, value):
		self._set_reset(4, value)

	def reset(self):
		self._pin_reset(False)
		self._pin_reset(True)

	def feed_byte(self, data_byte):
		for bit in reversed(range(8)):
			self._pin_data(bool((data_byte >> bit) & 1))
			self._pin_clk(True)
			self._pin_clk(False)

	def feed(self, data):
		for byte in data:
			self.feed_byte(byte)

	def mode_data(self):
		self._pin_dc(True)

	def mode_control(self):
		self._pin_dc(False)

conn = Conn()
conn.reset()
conn.mode_control()

# Datasheet init:              a83f d300 40 a0 c0 da02 817f a4 a6 d580 8d14 af
conn.feed(bytes.fromhex("2000 a83f d300 40 a1 c8 da12 817f a4 a6 d580 8d14 af"))

pic = PnmPicture().readfile("img.pnm")

conn.mode_data()
#for i in range(1024):
#	conn.feed(bytes.fromhex("00"))
#for i in range(20):
#	conn.feed(bytes.fromhex("00 01 02 04 08 10 20 40 80 aa 55 ff"))

for i in range(128 * 64 // 8):
	x = i % 128
	y0 = (i // 128) * 8
	value = 0
	for y in range(y0, y0 + 8):
		pix = pic.getpixel(x, y) != (255, 255, 255)
		value = (value >> 1) | (int(pix) * 0x80)
	conn.feed_byte(value)

#while True:
#	conn._pin_clk(bool(random.randint(0, 1)))
#	conn._pin_data(bool(random.randint(0, 1)))
