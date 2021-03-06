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

import collections
import os
import subprocess
import shutil
import sys

Package = collections.namedtuple("Package", [ "name", "url", "subdir" ])
packages = [
	Package(name = "binutils", url = "https://ftp.gnu.org/gnu/binutils/binutils-2.29.tar.gz", subdir = "binutils-2.29"),
	Package(name = "gcc", url = "https://ftp.gnu.org/gnu/gcc/gcc-7.2.0/gcc-7.2.0.tar.gz", subdir = "gcc-7.2.0"),
	Package(name = "newlib", url = "http://ftp.gwdg.de/pub/linux/sources.redhat.com/newlib/newlib-2.5.0.20170818.tar.gz", subdir = "newlib-2.5.0.20170818"),
	Package(name = "gdb", url = "https://ftp.gnu.org/gnu/gdb/gdb-8.0.tar.gz", subdir = "gdb-8.0"),
	Package(name = "stlink2", url = "https://github.com/texane/stlink/archive/1.4.0.tar.gz", subdir = "stlink-1.4.0"),
]
prefix_dir = os.getenv("HOME") + "/bin/gcc-cm4"

########################################################################

os.environ["PATH"] = prefix_dir + "/bin:" + os.getenv("PATH")
with open("environment", "w") as f:
	print("export PS1=\"(cm4) ${PS1}\"", file = f)
	print("export LD_LIBRARY_PATH=\"%s/lib\"" % (prefix_dir), file = f)
	print("export PATH=\"%s/bin:${PATH}\"" % (prefix_dir), file = f)
try:
	os.makedirs(prefix_dir)
except FileExistsError:
	pass
build_packages = set(sys.argv[1:])

class Installer(object):
	def __init__(self, name, pkg, prefix):
		self._name = name
		self._pkg = pkg
		self._prefix = prefix
		self._root = os.path.realpath(os.getcwd())
		self._src = self._root + "/" + os.path.basename(self._pkg.url)
		self._work = self._root + "/work"
		self._builddir = self._work + "/" + self._pkg.subdir + "/build"

	@property
	def name(self):
		return self._name

	@property
	def pkg(self):
		return self._pkg

	def cleanup(self):
		try:
			shutil.rmtree(self._work)
		except FileNotFoundError:
			pass
		os.makedirs(self._work)

	def download(self):
		if os.path.isfile(self._src):
			if os.stat(self._src).st_size == 0:
				os.unlink(self._src)
			else:
				return
		subprocess.check_call([ "wget", "-O", self._src, self._pkg.url ])

	def extract(self):
		try:
			os.chdir(self._work)
			subprocess.check_call([ "tar", "xf", self._src ])
		finally:
			os.chdir(self._root)

	def configure(self, configure_options):
		try:
			os.makedirs(self._builddir)
			os.chdir(self._builddir)
			subprocess.check_call([ self._work + "/" + self._pkg.subdir + "/configure", "--prefix=" + self._prefix ] + configure_options)
		finally:
			os.chdir(self._root)

	def configure_cmake(self, configure_options):
		try:
			os.makedirs(self._builddir)
			os.chdir(self._builddir)
			subprocess.check_call([ "cmake", "-DCMAKE_INSTALL_PREFIX:PATH=" + self._prefix ] + configure_options + [ self._work + "/" + self._pkg.subdir ])
		finally:
			os.chdir(self._root)

	def compile(self):
		try:
			os.chdir(self._builddir)
			subprocess.check_call([ "make", "-j16" ])
		finally:
			os.chdir(self._root)

	def install(self):
		try:
			os.chdir(self._builddir)
			subprocess.check_call([ "make", "install" ])
		finally:
			os.chdir(self._root)

	def start(self, configure_options, cmake = False):
		self.download()
		self.cleanup()
		self.extract()
		if not cmake:
			self.configure(configure_options)
		else:
			self.configure_cmake(configure_options)
		self.compile()
		self.install()
		self.cleanup()

packages = { pkg.name: pkg for pkg in packages }

base_options = [
	"--target=arm-none-eabi",
	"--with-cpu=cortex-m4",
	"--with-fpu=fpv4-sp-d16",
	"--with-float=hard",
	"--with-mode=thumb",
	"--disable-interwork",
	"--disable-multilib",
	"--with-gnu-as",
	"--with-gnu-ld",
	"--disable-nls",
]
gcc_options = [
	"--enable-languages=c",
	"--with-system-zlib",
	"--with-newlib",
	"--disable-shared",
	"--disable-libssp",
	"--with-dwarf2",
	"--disable-threads",
	"--disable-libmudflap",
	"--disable-libgomp",
	"--enable-target-optspace",
]

installer = Installer("binutils", packages["binutils"], prefix_dir)
if (len(build_packages) == 0) or (installer.name in build_packages):
	installer.start(configure_options = base_options)

installer = Installer("gcc-bootstrap", packages["gcc"], prefix_dir)
if (len(build_packages) == 0) or (installer.name in build_packages):
	installer.start(configure_options = base_options + gcc_options + [
		"--without-headers",
	])

installer = Installer("newlib", packages["newlib"], prefix_dir)
if (len(build_packages) == 0) or (installer.name in build_packages):
	installer.start(configure_options = base_options + [
		"--disable-newlib-supplied-syscalls",
		"--enable-newlib-nano-malloc",
		"--enable-newlib-reent-small",
		"--enable-newlib-nano-formatted-io",
		"--disable-newlib-multithread",
		"--enable-lite-exit",
		"--disable-newlib-wide-orient",
		"--disable-newlib-fvwrite-in-streamio",
		"--disable-newlib-atexit-dynamic-alloc",
		"--disable-newlib-io-float",
		"--enable-lite-exit",
	])

installer = Installer("gcc", packages["gcc"], prefix_dir)
if (len(build_packages) == 0) or (installer.name in build_packages):
	installer.start(configure_options = base_options + gcc_options)

installer = Installer("gdb", packages["gdb"], prefix_dir)
if (len(build_packages) == 0) or (installer.name in build_packages):
	installer.start(configure_options = base_options + [
#		"--with-auto-load-safe-path=.:$debugdir:$datadir/auto-load",
		"--with-python=/usr/bin/python3",
	])

installer = Installer("stlink2", packages["stlink2"], prefix_dir)
if (len(build_packages) == 0) or (installer.name in build_packages):
	installer.start(configure_options = [
		"-DSTLINK_MODPROBED_DIR:PATH=" + prefix_dir + "/etc/modprobe.d",
		"-DSTLINK_UDEV_RULES_DIR:PATH=" + prefix_dir + "/etc/udev.d",
	], cmake = True)

# In the end, strip every binary and every library
subprocess.call([ "find", prefix_dir, "-type", "f", "-perm", "700", "-exec", "strip", "{}", "+" ])
subprocess.call([ "find", prefix_dir, "-type", "f", "-perm", "755", "-exec", "strip", "{}", "+" ])

