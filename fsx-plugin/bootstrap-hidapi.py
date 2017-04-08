#!/usr/bin/python3
import os
import subprocess
import shutil

if not os.path.isdir("hidapi"):
	subprocess.check_call([ "git", "clone", "https://github.com/signal11/hidapi.git" ])

os.chdir("hidapi")

for (host_params, src_dir, dst_dir) in [
		([ ], "libusb/.libs/", "linux" ),
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

