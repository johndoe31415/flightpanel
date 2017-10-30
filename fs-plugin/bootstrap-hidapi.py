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
	cached_hidapi_dir = os.getenv("HOME") + "/.cache/flightpanel/hidapi"
	if os.path.isdir(cached_hidapi_dir):
		# Have a cached git repo available
		shutil.copytree(cached_hidapi_dir, "hidapi")
	else:
		# Clone git repo
		subprocess.check_call([ "git", "clone", "https://github.com/signal11/hidapi.git" ])

os.chdir("hidapi")
for (host_params, src_dir, dst_dir) in [
		#([ ], "libusb/.libs/", "linux" ),
		([ "--host=i686-w64-mingw32", "--enable-shared" ], "windows/.libs/", "windows" ),
		#([ "--host=x86_64-w64-mingw32", "--enable-shared" ], "windows/.libs/", "windows" ),
	]:
	install_dir = os.path.realpath("../build/" + dst_dir)
	try:
		shutil.rmtree(install_dir)
	except FileNotFoundError:
		pass
	os.makedirs(install_dir)
	print("Building libhid for %s (configure parameters: %s) in %s, installing in %s" % (dst_dir, str(host_params), os.getcwd(), install_dir))
	subprocess.check_call([ "git", "clean", "-dfx" ])
	subprocess.check_call([ "./bootstrap" ])
	subprocess.check_call([ "./configure", "--bindir", install_dir, "--libdir", install_dir ] + host_params)
	subprocess.check_call([ "make" ])
	shutil.copy("windows/.libs/libhidapi-0.dll", install_dir)
	shutil.copy("windows/.libs/libhidapi.dll.a", install_dir)
