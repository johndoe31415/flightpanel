#!/usr/bin/python3
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

os.chdir(os.getenv("HOME"))

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
	subprocess.check_call([ "tar", "xfv", "cache.tar" ])
else:
	print("Not downloading cache, already present.")

compiler_dir = os.getenv("HOME") + "/bin/gcc/arm-cm4-bare/bin"
if not os.path.isdir(compiler_dir):
	# Extract compiler from cache
	subprocess.check_call([ "tar", "xfvz", cache_dir + "compiler.tar.gz" ])
else:
	print("Not extracting Cortex-M4 compiler, already present.")

os.environ["PATH"] = compiler_dir + ":" + os.getenv("PATH")

with WorkDir("firmware"):
	shutil.copy(cache_dir + "en.stm32cubef4.zip", "en.stm32cubef4.zip")
	shutil.copy(cache_dir + "en.stm32f4_dsp_stdperiph_lib.zip", "en.stm32f4_dsp_stdperiph_lib.zip")
	subprocess.check_call([ "./bootstrap.sh" ])
	with WorkDir("ext-st"):
		subprocess.check_call([ "make" ])
	with WorkDir("cube"):
		subprocess.check_call([ "make" ])
	subprocess.check_call([ "make" ])


