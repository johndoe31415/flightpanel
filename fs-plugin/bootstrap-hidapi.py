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
import subprocess
import shutil

if not os.path.isdir("hidapi"):
	subprocess.check_call([ "git", "clone", "https://github.com/signal11/hidapi.git" ])

os.chdir("hidapi")

for (host_params, src_dir, dst_dir) in [
		#([ ], "libusb/.libs/", "linux" ),
		([ "--host=i686-w64-mingw32" ], "windows/.libs/", "windows" ),
	]:
	print("Building libhid for %s (configure parameters: %s)" % (dst_dir, str(host_params)))
	subprocess.check_call([ "git", "clean", "-dfx" ])
	subprocess.check_call([ "./bootstrap" ])
	subprocess.check_call([ "./configure" ] + host_params)
	subprocess.check_call([ "make" ])

	for filename in os.listdir(src_dir):
		src_file = src_dir + filename
		dst_dir = "../build/" + dst_dir + "/"
		try:
			os.makedirs(dst_dir)
		except FileExistsError:
			pass
		dst_file =  dst_dir + filename
		os.rename(src_file, dst_file)

