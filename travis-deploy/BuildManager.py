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
import re
import logging
import subprocess
from .TimeLogger import TimeLogger
from .Exceptions import BuildImplementationError, BuildPrerequisitesNotMetException

class BuildManager(object):
	__SUBSTITUTION_RE = re.compile(r"\${(?P<varname>[a-zA-Z_]+)}")
	_SUPPORTED_ACTIONS = [ ]
	_REQUIRED_ENVIRONMENT = { }
	_PATHS = { }
	_FILES = { }

	def __init__(self, args):
		self._args = args
		for filename in self._FILES.values():
			if filename.endswith("/"):
				raise BuildImplementationError("Declared file '%s' ends with '/'." % (filename))
		self._log = logging.getLogger("bm")

	def _execute(self, cmd, add_env = None):
		environment = dict(os.environ)
		if add_env is not None:
			environment.update(add_env)
		subprocess.check_call(cmd, env = environment)

	def _add_to_path(self, dirname):
		path = os.environ.get("PATH")
		if path is None:
			os.environ["PATH"] = dirname
		else:
			os.environ["PATH"] = dirname + ":" + path

	def _substitute(self, text):
		def sub_fnc(match):
			varname = match.groupdict()["varname"]
			return self._get_dir(varname, must_exist = False, create = False)
		text = self.__SUBSTITUTION_RE.sub(sub_fnc, text)
		return text

	def _get_dir(self, dirhandle, must_exist = False, create = False):
		if dirhandle == "HOME":
			directory = os.getenv("HOME")
		else:
			if dirhandle not in self._PATHS:
				raise BuildImplementationError("Requested directory '%s' not declared." % (dirhandle))
			directory = self._PATHS[dirhandle]

		directory = self._substitute(directory)
		if not directory.endswith("/"):
			directory += "/"

		if create and not os.path.exists(directory):
			os.makedirs(directory)

		if must_exist:
			if not os.path.exists(directory):
				raise BuildPrerequisitesNotMetException("Requested directory '%s' should exist, but does not." % (directory))

		if os.path.exists(directory) and (not os.path.isdir(directory)):
			raise BuildPrerequisitesNotMetException("Requested directory '%s' exists, but is not a directory." % (directory))

		return directory

	def _get_file(self, filehandle, must_exist = False):
		if filehandle not in self._FILES:
			raise BuildImplementationError("Requested filename '%s' not declared." % (filehandle))
		filename = self._FILES[filehandle]

		filename = self._substitute(filename)
		if must_exist:
			if not os.path.exists(filename):
				raise BuildPrerequisitesNotMetException("Requested file '%s' should exist, but does not." % (filename))
		if os.path.exists(filename) and (not os.path.isfile(filename)):
			raise BuildPrerequisitesNotMetException("Requested file '%s' exists, but is not a regular file." % (filename))
		return filename

	def execute_actions(self, actions):
		for action in actions:
			if action not in self._SUPPORTED_ACTIONS:
				raise BuildImplementationError("Action '%s' requested, but not contained within SUPPORTED_ACTIONS." % (action))

			required_environment = self._REQUIRED_ENVIRONMENT.get(action, [ ])
			for required_variable in required_environment:
				if os.getenv(required_variable) is None:
					raise BuildPrerequisitesNotMetException("Build action '%s' requires environment variable '%s' to be set, but this wasn't the case." % (action, required_variable))

		for action in self._SUPPORTED_ACTIONS:
			handler_name = "do_" + action
			handler = getattr(self, handler_name)
			if handler is None:
				raise BuildImplementationError("Action '%s' declared in SUPPORTED_ACTIONS, but no handler called '%s' implemented." % (action, handler_name))

		if len(actions) == 0:
			return False

		action_order = { action: index for (index, action) in enumerate(self._SUPPORTED_ACTIONS) }
		requested_action_ids = sorted(action_order[action] for action in actions)
		ordered_actions = [ self._SUPPORTED_ACTIONS[action_id] for action_id in requested_action_ids ]
		with TimeLogger(self._log, "Running %d build actions (%s)" % (len(ordered_actions), ", ".join(ordered_actions))):
			for action in ordered_actions:
				handler_name = "do_" + action
				handler = getattr(self, handler_name)
				with TimeLogger(self._log, "Performing \"%s\"" % (action)):
					handler()
		return True

	def execute(self):
		actions = [ ]
		for name in vars(self._args):
			value = getattr(self._args, name)
			if name.startswith("do_") and value:
				action = name[3:]
				actions.append(action)
		return self.execute_actions(actions)

	def execute_all(self):
		return self.execute_actions(self._SUPPORTED_ACTIONS)

