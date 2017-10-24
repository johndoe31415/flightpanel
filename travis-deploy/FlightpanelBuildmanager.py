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
import sys
import datetime
import shutil
import subprocess
import hashlib

from .BuildManager import BuildManager
from .WorkDir import WorkDir
from .TimeLogger import TimeLogger
from .GitUtils import GitUtils
from .GitHubAccessor import GitHubAccessor

class FlightpanelBuildmanager(BuildManager):
	_SUPPORTED_ACTIONS = [ "setup", "build_firmware", "build_plugins", "package_binaries", "release_binaries"  ]
	_REQUIRED_ENVIRONMENT = {
		"setup":				[ "CACHE_FILE_UPSTREAM_URI" ],
		"release_binaries":		[ "GITHUB_ACCESS_TOKEN" ],
	}
	_PATHS = {
		"cache_dir":				"${HOME}.cache/flightpanel/",
		"compiler_bindir":			"${HOME}bin/gcc-cm4/bin/",
	}
	_FILES = {
		"compiler_targz":			"${cache_dir}compiler.tar.gz",
		"extst_zip":				"${cache_dir}en.stm32f4_dsp_stdperiph_lib.zip",
		"cubef4_zip":				"${cache_dir}en.stm32cubef4.zip",
		"sdk_simconnect_targz":		"${cache_dir}sdk-simconnect.tar.gz",
		"xsquawkbox_targz":			"${cache_dir}xsquawkbox.tar.gz",
	}

	def __init__(self, args):
		BuildManager.__init__(self, args)
		self._add_to_path(self._get_dir("compiler_bindir"))
		self._now_utc = datetime.datetime.utcnow()
		self._asset_basename = "flightpanel-devbuild.tar.gz"

	def do_setup(self):
		if not os.path.isfile(self._get_file("compiler_targz", must_exist = False)):
			# Download and extract cache from upstream
			try:
				with TimeLogger(self._log, "Downloading toolchain from CACHE_FILE_UPSTREAM_URI"):
					self._execute([ "wget", "-o", "/dev/null", "-O", "cache.tar", os.getenv("CACHE_FILE_UPSTREAM_URI") ])
				filesize = os.stat("cache.tar").st_size
				self._log.info("Download of toolchain finished successfully from CACHE_FILE_UPSTREAM_URI: %d bytes", filesize)

				with TimeLogger(self._log, "Hashing downloaded file"):
					hashval = hashlib.md5()
					with open("cache.tar", "rb") as f:
						for chunk in iter(lambda: f.read(512 * 1024), b""):
							hashval.update(chunk)
				self._log.info("Download of CACHE_FILE_UPSTREAM_URI: MD5SUM is %s", hashval.hexdigest())
			except subprocess.CalledProcessError:
				# Catch and exit so that stacktrace does not reveal URI in log
				self._log.info("Download of CACHE_FILE_UPSTREAM_URI failed. Omitting further details (keeping the URI hidden). Confirm that the URI is accessible (permissions?).")
				sys.exit(1)
			self._execute([ "tar", "-x", "-v", "-C", os.getenv("HOME"), "-f", "cache.tar" ])
		else:
			self._log.info("Not downloading toolchain from CACHE_FILE_UPSTREAM_URI, using cached: %s" % (self._get_file("compiler_targz")))

		if not os.path.isdir(self._get_dir("compiler_bindir")):
			# Extract compiler from cache
			self._log.info("Extracting compiler from %s" % (self._get_file("compiler_targz")))
			self._execute([ "tar", "-x", "-z", "-C", os.getenv("HOME"), "-f", self._get_file("compiler_targz") ])
		else:
			self._log.info("Not extracting Cortex-M4 compiler, already present: %s" % (self._get_dir("compiler_bindir")))

	def do_build_firmware(self):
		with WorkDir("firmware"):
			shutil.copy(self._get_file("extst_zip", must_exist = True), "en.stm32f4_dsp_stdperiph_lib.zip")
			shutil.copy(self._get_file("cubef4_zip", must_exist = True), "en.stm32cubef4.zip")
			with TimeLogger(self._log, "Running bootstrap.sh script"):
				self._execute([ "./bootstrap.sh" ])
			with WorkDir("ext-st"), TimeLogger(self._log, "Building ST external peripheral library"):
				self._execute([ "make", "clean" ])
				self._execute([ "make", "all", "-j%d" % (self._args.parallel) ], add_env = { "CFLAGS": "-DRELEASE_BUILD" })
			with WorkDir("cube"), TimeLogger(self._log, "Building ST cube library"):
				self._execute([ "make", "clean" ])
				self._execute([ "make", "all", "-j%d" % (self._args.parallel) ], add_env = { "CFLAGS": "-DRELEASE_BUILD" })
			with TimeLogger(self._log, "Running firmware tests"):
				self._execute([ "make", "clean" ])
				self._execute([ "make", "all", "-j%d" % (self._args.parallel) ], add_env = { "CFLAGS": "-DRELEASE_BUILD" })
				self._execute([ "make", "tests" ])
			with TimeLogger(self._log, "Building firmware binary"):
				self._execute([ "make", "clean" ])
				self._execute([ "make", "all", "-j%d" % (self._args.parallel) ], add_env = { "CFLAGS": "-DRELEASE_BUILD" })

	def do_build_plugins(self):
		with WorkDir("fs-plugin"):
			self._log.info("Extracting SimConnect header files")
			self._execute([ "tar", "xfz", self._get_file("sdk_simconnect_targz", must_exist = True) ])
			self._log.info("Extracting XSquawkbox header files")
			self._execute([ "tar", "xfz", self._get_file("xsquawkbox_targz", must_exist = True) ])
			with TimeLogger(self._log, "Bootstrapping libhidapi"):
				self._execute([ "./bootstrap-hidapi.py" ])
			for variant in [ "linux-emulator", "linux-xplane", "windows-fsx" ]:
				with TimeLogger(self._log, "Building connector plugin for variant %s" % (variant)):
					self._execute([ "make", "clean" ], add_env = { "VARIANT": variant })
					self._execute([ "make", "all", "-j%d" % (self._args.parallel) ], add_env = { "VARIANT": variant })
					self._execute([ "make", "install" ], add_env = { "VARIANT": variant })

	def do_package_binaries(self):
		self._log.info("Preparing devbuild package from all binaries and some metadata")
		dev_dir = "flightpanel-devbuild/"
		try:
			shutil.rmtree(dev_dir)
		except FileNotFoundError:
			pass
		os.makedirs(dev_dir)
		os.makedirs(dev_dir + "firmware")
		os.makedirs(dev_dir + "windows-fsx")
		os.makedirs(dev_dir + "linux-xplane/flightpanel/64")

		git_rev = GitUtils.get_rev()
		with open(dev_dir + "version_info.txt", "w") as f:
			print("Automatic development build", file = f)
			print("===========================", file = f)
			print(file = f)
			print("Created at: %s UTC" % (self._now_utc.strftime("%A, %Y-%m-%d %H:%M:%S")), file = f)
			print("Built from %s" % (git_rev), file = f)
			print(file = f)
			print("flightpanel is open source software and licensed under the GNU GPL-3.", file = f)
			print("More information and full source code:", file = f)
			print("    https://github.com/johndoe31415/flightpanel", file = f)
			print(file = f)
			print("~" * 120, file = f)
			print(file = f)
			print("Compiler used for firmware:", file = f)
			print(subprocess.check_output([ "arm-none-eabi-gcc", "-v" ], stderr = subprocess.STDOUT).decode(), file = f)
			print(file = f)
			print("Compiler used for running tests:", file = f)
			print(subprocess.check_output([ "cc", "-v" ], stderr = subprocess.STDOUT).decode(), file = f)
			print(file = f)
			print("Compiler used for Win32 cross compilation:", file = f)
			print(subprocess.check_output([ "i686-w64-mingw32-gcc", "-v" ], stderr = subprocess.STDOUT).decode(), file = f)

			if GitUtils.repo_is_dirty():
				# We're dealing with a dirty build. Include diff and status files.
				git_status = subprocess.check_output([ "git", "status", "--short" ])
				with open(dev_dir + "git-status.txt", "wb") as f:
					f.write(git_status)

				git_diff = subprocess.check_output([ "git", "diff" ])
				with open(dev_dir + "git-diff.txt", "wb") as f:
					f.write(git_diff)

		shutil.copy("LICENSE", dev_dir)
		shutil.copy("firmware/flightpanel.bin", dev_dir + "firmware")
		shutil.copy("fs-plugin/build/windows/libhidapi-0.dll", dev_dir + "windows-fsx")
		shutil.copy("fs-plugin/build/windows/windows-fsx_fpconnect.exe", dev_dir + "windows-fsx/fpconnect.exe")
		shutil.copy("fs-plugin/build/linux/linux-xplane_fpconnect.so", dev_dir + "linux-xplane/flightpanel/64/lin.xpl")
		for (basedir, subdirs, files) in os.walk(dev_dir):
			for filename in files:
				filename = basedir + "/" + filename
				os.chmod(filename, 0o644)

		self._log.info("Packaging everything into %s" % (self._asset_basename))
		self._execute([ "tar", "cfz", self._asset_basename, dev_dir ])

	def do_release_binaries(self):
		github = GitHubAccessor(owner = "johndoe31415", repo = "flightpanel", token = os.getenv("GITHUB_ACCESS_TOKEN"))
		git_rev = subprocess.check_output([ "git", "rev-parse", "HEAD" ]).decode("utf-8").rstrip("\r\n ")

		tagname = "devbuild"
		asset_content_type = "application/x-gtar-compressed"
		with open(self._asset_basename, "rb") as f:
			asset_data = f.read()
		self._log.info("Read asset from \"%s\", which was built from commit %s (%d bytes total)" % (self._asset_basename, git_rev, len(asset_data)))

		self._log.info("Updating ref of tag %s to point to %s" % (tagname, git_rev))
		result = github.update_ref("tags/" + tagname, git_rev)
		if result.status_code == 422:
			# No reference yet, create it
			self._log.info("Creating reference for '%s'" % (tagname))
			result = github.create_ref("tags/" + tagname, git_rev)
			if result.status_code != 201:
				raise Exception("Unexpected response when trying to create_ref: %s" % (result))
		elif result.status_code != 200:
			raise Exception("Unexpected response when trying to update_ref: %s" % (result))

		self._log.info("Getting release on GibHub by the tag name of \"%s\"" % (tagname))
		release = github.get_release_by_tagname(tagname)
		if release.status_code == 404:
			# No release yet, create
			self._log.info("Creating release on GitHub for '%s'" % (tagname))
			release = github.create_release(tagname, prerelease = True)
			if release.status_code != 201:
				raise Exception("Unexpected response when trying to create_release: %s" % (result))

		release = release.json()
		upload = True
		if not upload:
			self._log.info("Temporarily not pushing tag to GitHub until bug with build system is fixed")
		else:
			self._log.info("Updating release on GitHub to contain current timestamp")
			result = github.edit_release(release["id"], name = "Automatic release build", body = "Automatically built by Travis CI on %s UTC." % (self._now_utc.strftime("%A, %Y-%m-%d %H:%M:%S")))
			if result.status_code != 200:
				raise Exception("Unexpected response when trying to edit_release: %s" % (result))

			if "assets" in release:
				for asset in release["assets"]:
					if asset["name"] == self._asset_basename:
						asset_id = asset["id"]
						self._log.info("Deleting currently uploaded GitHub asset #%s" % (asset_id))
						result = github.release_delete_asset(asset_id)
						if result.status_code != 204:
							raise Exception("Unexpected response when trying to delete_release: %s" % (result))

			self._log.info("Uploading asset \"%s\" to GitHub" % (self._asset_basename))
			result = github.release_upload_asset(release["upload_url"], asset_content_type, self._asset_basename, asset_data)
			if result.status_code != 201:
				raise Exception("Unexpected response when trying to upload_asset: %s" % (result))

