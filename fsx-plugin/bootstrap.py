#!/usr/bin/python3
import subprocess
import os

if not os.path.isdir("hidapi"):
	subprocess.check_call([ "git", "clone", "https://github.com/signal11/hidapi.git" ])

os.chdir("hidapi")
subprocess.check_call([ "git", "clean", "-dfx" ])
subprocess.check_call([ "./bootstrap" ])
subprocess.check_call([ "./configure", "--host=i686-w64-mingw32" ])
subprocess.check_call([ "make" ])


