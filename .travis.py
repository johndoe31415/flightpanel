#!/usr/bin/python3
import sys
import subprocess
import os
import shutil

class WorkDir(object):
	def __init__(self, newdir):
		self._prevdir = os.getcwd()
		self._newdir = newdir

	def __enter__(self):
		os.chdir(self._newdir)
		return self

	def __exit__(self, *args):
		os.chdir(self._prevdir)

cache_dir = os.getenv("HOME") + "/.cache/flightpanel/"
if not os.path.isfile(cache_dir + "compiler.tar.gz"):
	# Download and extract cache from upstream
	try:
		print("Downloading...")
		subprocess.check_call([ "wget", "-o", "/dev/null", "-O", "cache.tar", os.getenv("CACHE_FILE_UPSTREAM_URI") ])
		print("Download finished.")
	except subprocess.CalledProcessError:
		# Catch and exit so that stacktrace does not reveal URI in log
		print("Call to wget failed.")
		sys.exit(1)
	subprocess.check_call([ "tar", "-x", "-v", "-C", os.getenv("HOME"), "-f", "cache.tar" ])
else:
	print("Not downloading cache, already present.")

compiler_dir = os.getenv("HOME") + "/bin/gcc-cm4/bin"
if not os.path.isdir(compiler_dir):
	# Extract compiler from cache
	subprocess.check_call([ "tar", "-x", "-v", "-z", "-C", os.getenv("HOME"), "-f", cache_dir + "compiler.tar.gz" ])
else:
	print("Not extracting Cortex-M4 compiler, already present.")

os.environ["PATH"] = compiler_dir + ":" + os.getenv("PATH")

def updated_env(update_dict):
	env = dict(os.environ)
	env.update(update_dict)
	return env

if (len(sys.argv) == 1) or ("f" in sys.argv[1]):
	with WorkDir("firmware"):
		shutil.copy(cache_dir + "en.stm32cubef4.zip", "en.stm32cubef4.zip")
		shutil.copy(cache_dir + "en.stm32f4_dsp_stdperiph_lib.zip", "en.stm32f4_dsp_stdperiph_lib.zip")
		subprocess.check_call([ "./bootstrap.sh" ])
		with WorkDir("ext-st"):
			subprocess.check_call([ "make" ])
		with WorkDir("cube"):
			subprocess.check_call([ "make" ])
		subprocess.check_call([ "make" ])

if (len(sys.argv) == 1) or ("p" in sys.argv[1]):
	with WorkDir("fs-plugin"):
		subprocess.check_call([ "tar", "xfvz", cache_dir + "sdk-simconnect.tar.gz" ])
		subprocess.check_call([ "tar", "xfvz", cache_dir + "xsquawkbox.tar.gz" ])
		subprocess.check_call([ "./bootstrap-hidapi.py" ])
		for variant in [ "linux-emulator", "linux-xplane", "windows-fsx" ]:
			subprocess.check_call([ "make", "clean", "all" ], env = updated_env({ "VARIANT": variant }))

