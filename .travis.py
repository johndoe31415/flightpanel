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

import sys
import subprocess
import os
import shutil
import datetime
import requests
import json

class WorkDir(object):
	def __init__(self, newdir):
		self._prevdir = os.getcwd()
		self._newdir = newdir

	def __enter__(self):
		os.chdir(self._newdir)
		return self

	def __exit__(self, *args):
		os.chdir(self._prevdir)

class GitHubAccessor(object):
	def __init__(self, owner, repo, token):
		assert(token is not None)
		self._owner = owner
		self._repo = repo
		self._token = token
		self._session = requests.Session()

	@property
	def headers(self):
		return {
			"Accept":			"application/vnd.github.v3+json",
			"Content-Type":		"application/json; charset=utf-8",
			"Authorization":	"token %s" % (self._token),
		}

	def _post(self, uri, data):
		response = self._session.post("https://api.github.com" + uri, headers = self.headers, data = json.dumps(data))
		return response

	def _patch(self, uri, data):
		response = self._session.patch("https://api.github.com" + uri, headers = self.headers, data = json.dumps(data))
		return response

	def _get(self, uri):
		response = self._session.get("https://api.github.com" + uri, headers = self.headers)
		return response

	def _delete(self, uri):
		response = self._session.delete("https://api.github.com" + uri, headers = self.headers)
		return response

	def create_ref(self, refname, sha):
		uri = "/repos/%s/%s/git/refs" % (self._owner, self._repo)
		data = {
			"ref":	"refs/" + refname,
			"sha":	sha,
		}
		return self._post(uri, data)

	def update_ref(self, refname, sha, force = False):
		ref = refname
		uri = "/repos/%s/%s/git/refs/%s" % (self._owner, self._repo, ref)
		data = {
			"sha":	sha,
			"force": force,
		}
		return self._patch(uri, data)

	def list_releases(self):
		uri = "/repos/%s/%s/releases" % (self._owner, self._repo)
		return self._get(uri)

	def create_release(self, tagname, prerelease = False):
		uri = "/repos/%s/%s/releases" % (self._owner, self._repo)
		data = {
			"tag_name":		tagname,
			"prerelease":	prerelease,
		}
		return self._post(uri, data)

	def edit_release(self, release_id, tag_name = None, name = None, body = None, draft = None, prerelease = None):
		uri = "/repos/%s/%s/releases/%s" % (self._owner, self._repo, release_id)
		data = { }
		if tag_name is not None:
			data["tag_name"] = tag_name
		if name is not None:
			data["name"] = name
		if body is not None:
			data["body"] = body
		if draft is not None:
			data["draft"] = draft
		if prerelease is not None:
			data["prerelease"] = prerelease
		return self._patch(uri, data)

	def get_release_by_tagname(self, tagname):
		uri = "/repos/%s/%s/releases/tags/%s" % (self._owner, self._repo, tagname)
		return self._get(uri)

	def release_upload_asset(self, upload_url, content_type, filename, data):
		base_url = upload_url.split("{?")[0]
		headers = self.headers
		headers["Content-Type"] = content_type
		response = self._session.post(base_url + "?name=%s" % (filename), headers = headers, data = data)
		return response

	def release_delete_asset(self, asset_id):
		return self._delete("/repos/%s/%s/releases/assets/%s" % (self._owner, self._repo, asset_id))

def updated_env(update_dict):
	env = dict(os.environ)
	env.update(update_dict)
	return env

def msg(text):
	print("==> %s" % (text))

cache_dir = os.getenv("HOME") + "/.cache/flightpanel/"
cache_compiler_file = cache_dir + "compiler.tar.gz"
if not os.path.isfile(cache_compiler_file):
	# Download and extract cache from upstream
	try:
		msg("Downloading toolchain from CACHE_FILE_UPSTREAM_URI...")
		subprocess.check_call([ "wget", "-o", "/dev/null", "-O", "cache.tar", os.getenv("CACHE_FILE_UPSTREAM_URI") ])
		msg("Download of toolchain finished successfully from CACHE_FILE_UPSTREAM_URI.")
	except subprocess.CalledProcessError:
		# Catch and exit so that stacktrace does not reveal URI in log
		msg("Call to wget failed while trying to download the cache file from CACHE_FILE_UPSTREAM_URI.")
		msg("Not msging out further details since the URI is hidden.")
		msg("Check that the URI is accessible (permissions?).")
		sys.exit(1)
	subprocess.check_call([ "tar", "-x", "-v", "-C", os.getenv("HOME"), "-f", "cache.tar" ])
else:
	msg("Not downloading toolchain from CACHE_FILE_UPSTREAM_URI, using cached: %s" % (cache_compiler_file))

compiler_dir = os.getenv("HOME") + "/bin/gcc-cm4/bin"
if not os.path.isdir(compiler_dir):
	# Extract compiler from cache
	msg("Extracting compiler from %s" % (cache_compiler_file))
	subprocess.check_call([ "tar", "-x", , "-z", "-C", os.getenv("HOME"), "-f", cache_compiler_file ])
else:
	msg("Not extracting Cortex-M4 compiler, already present: %s" % (compiler_dir))

os.environ["PATH"] = compiler_dir + ":" + os.getenv("PATH")

if (len(sys.argv) == 1) or ("f" in sys.argv[1]):
	# Build (f)irmware
	with WorkDir("firmware"):
		shutil.copy(cache_dir + "en.stm32cubef4.zip", "en.stm32cubef4.zip")
		shutil.copy(cache_dir + "en.stm32f4_dsp_stdperiph_lib.zip", "en.stm32f4_dsp_stdperiph_lib.zip")
		msg("Running bootstrap.sh script")
		subprocess.check_call([ "./bootstrap.sh" ])
		with WorkDir("ext-st"):
			msg("Building ST external peripheral library")
			subprocess.check_call([ "make" ], env = updated_env({ "CFLAGS": "-DRELEASE_BUILD" }))
		with WorkDir("cube"):
			msg("Building ST cube library")
			subprocess.check_call([ "make" ], env = updated_env({ "CFLAGS": "-DRELEASE_BUILD" }))
		msg("Running firmware tests")
		subprocess.check_call([ "make", "clean", "tests" ])
		msg("Building firmware")
		subprocess.check_call([ "make", "clean", "all" ], env = updated_env({ "CFLAGS": "-DRELEASE_BUILD" }))

if (len(sys.argv) == 1) or ("p" in sys.argv[1]):
	# Build (p)lugin
	with WorkDir("fs-plugin"):
		msg("Extracting SimConnect header files")
		subprocess.check_call([ "tar", "xfz", cache_dir + "sdk-simconnect.tar.gz" ])
		msg("Extracting XSquawkbox header files")
		subprocess.check_call([ "tar", "xfz", cache_dir + "xsquawkbox.tar.gz" ])
		msg("Bootstrapping libhidapi")
		subprocess.check_call([ "./bootstrap-hidapi.py" ])
		for variant in [ "linux-emulator", "linux-xplane", "windows-fsx" ]:
			msg("Building connector plugin for variant %s" % (variant))
			subprocess.check_call([ "make", "clean", "all", "install" ], env = updated_env({ "VARIANT": variant }))

asset_filename = "flightpanel-devbuild.tar.gz"
now_utc = datetime.datetime.utcnow()

if (len(sys.argv) == 1) or ("a" in sys.argv[1]):
	# Build (a)rtifact
	msg("Preparing devbuild package from all binaries and some metadata")
	dev_dir = "flightpanel-devbuild/"
	try:
		shutil.rmtree(dev_dir)
	except FileNotFoundError:
		pass
	os.makedirs(dev_dir)
	os.makedirs(dev_dir + "firmware")
	os.makedirs(dev_dir + "windows-fsx")
	os.makedirs(dev_dir + "linux-xplane/flightpanel/64")

	git_rev = subprocess.check_output([ "git", "describe", "--abbrev=10", "--dirty", "--always" ]).decode("utf-8").rstrip("\r\n ")
	with open(dev_dir + "version_info.txt", "w") as f:
		print("Automatic development build", file = f)
		print("===========================", file = f)
		print(file = f)
		print("Created at: %s UTC" % (now_utc.strftime("%A, %Y-%m-%d %H:%M:%S")), file = f)
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
		print("Compiler used for Win32 cross compilation:", file = f)
		print(subprocess.check_output([ "i686-w64-mingw32-gcc", "-v" ], stderr = subprocess.STDOUT).decode(), file = f)

		returncode = subprocess.call([ "git", "diff-index", "--quiet", "HEAD" ])
		if returncode != 0:
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

	msg("Packaging everything into %s" % (asset_filename))
	subprocess.check_call([ "tar", "cfz", asset_filename, dev_dir ])

if (len(sys.argv) == 1) or ("u" in sys.argv[1]):
	# (u)pload build artifact to GitHub
	github = GitHubAccessor(owner = "johndoe31415", repo = "flightpanel", token = os.getenv("GITHUB_ACCESS_TOKEN"))
	git_rev = subprocess.check_output([ "git", "rev-parse", "HEAD" ]).decode("utf-8").rstrip("\r\n ")

	tagname = "devbuild"
	asset_content_type = "application/x-gtar-compressed"
	with open(asset_filename, "rb") as f:
		asset_data = f.read()
	msg("Read asset from %s build from commit %s (%d bytes total)" % (asset_filename, git_rev, len(asset_data)))

	msg("Updating ref of tag %s to point to %s" % (tagname, git_rev))
	result = github.update_ref("tags/" + tagname, git_rev)
	if result.status_code == 422:
		# No reference yet, create
		msg("Creating reference for '%s'" % (tagname))
		result = github.create_ref("tags/" + tagname, git_rev)
		if result.status_code != 201:
			raise Exception("Unexpected response when trying to create_ref: %s" % (result))
	elif result.status_code != 200:
		raise Exception("Unexpected response when trying to update_ref: %s" % (result))

	msg("Getting release on GibHub by the tag name of %s" % (tagname))
	release = github.get_release_by_tagname(tagname)
	if release.status_code == 404:
		# No release yet, create
		msg("Creating release on GitHub for '%s'" % (tagname))
		release = github.create_release(tagname, prerelease = True)
		if release.status_code != 201:
			raise Exception("Unexpected response when trying to create_release: %s" % (result))

	release = release.json()
	msg("Updating release on GitHub to contain current timestamp")
	result = github.edit_release(release["id"], name = "Automatic release build", body = "Automatically built by Travis CI on %s UTC." % (now_utc.strftime("%A, %Y-%m-%d %H:%M:%S")))
	if result.status_code != 200:
		raise Exception("Unexpected response when trying to edit_release: %s" % (result))

	if "assets" in release:
		for asset in release["assets"]:
			if asset["name"] == asset_filename:
				asset_id = asset["id"]
				msg("Deleting asset %s" % (asset_id))
				result = github.release_delete_asset(asset_id)
				if result.status_code != 204:
					raise Exception("Unexpected response when trying to delete_release: %s" % (result))

	msg("Uploading asset %s to GitHub" % (asset_filename))
	result = github.release_upload_asset(release["upload_url"], asset_content_type, asset_filename, asset_data)
	if result.status_code != 201:
		raise Exception("Unexpected response when trying to upload_asset: %s" % (result))
