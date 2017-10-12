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

import json
import requests

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
			"sha":		sha,
			"force":	force,
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
