#!/usr/bin/python3
import os
import subprocess
import shutil

home_directory = os.path.realpath(os.getenv("HOME"))
target_directory = home_directory + "/.cache/flightpanel/"
current_directory = os.path.realpath(".") + "/"

try:
	shutil.rmtree(target_directory)
except FileNotFoundError:
	pass
os.makedirs(target_directory)

# Pack up compiler binaries
print("Packing up gcc...")
subprocess.check_call([ "tar", "-cz", "-f", target_directory + "compiler.tar.gz", "-C", home_directory, "bin/gcc-cm4" ])

# Pack up SDK-Simconnect header files necessary for building
print("Packing up simconnect...")
subprocess.check_call([ "tar", "-cz", "-f", target_directory + "sdk-simconnect.tar.gz", "-C", "../fs-plugin", "sdk-simconnect/SimConnect.h", "sdk-simconnect/SimConnect.lib" ])

# Pack up XSquawkBox header files necessary for building
print("Packing up XSquawkBox...")
subprocess.check_call([ "tar", "-cz", "-f", target_directory + "xsquawkbox.tar.gz", "-C", "../fs-plugin", "xsquawkbox" ])

# Copy STM binaries
print("Packing up STM libraries...")
shutil.copy("../firmware/en.stm32cubef4.zip", target_directory)
shutil.copy("../firmware/en.stm32f4_dsp_stdperiph_lib.zip", target_directory)

# Now finally, bundle everything into one TAR file for Travis-CI
print("Packing up all of the above...")
subprocess.check_call([ "tar", "-cv", "-f", current_directory + "travis-ci.tar", "-C", home_directory, ".cache/flightpanel" ])
