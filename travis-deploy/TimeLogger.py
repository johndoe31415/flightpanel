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

import time

class TimeLogger(object):
	def __init__(self, logger, msg):
		self._log = logger
		self._msg = msg
		self._t0 = None

	def __enter__(self):
		self._t0 = time.time()
		self._log.info("%s...", self._msg)
		return self

	def __exit__(self, *args):
		t = time.time() - self._t0
		secs = round(t)
		if secs < 60:
			self._log.info("%s finished, took %d seconds", self._msg, secs)
		else:
			self._log.info("%s finished, took %d:%02d minutes:seconds", self._msg, secs // 60, secs % 60)

