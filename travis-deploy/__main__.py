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
import logging
from .FriendlyArgumentParser import FriendlyArgumentParser
from .FlightpanelBuildmanager import FlightpanelBuildmanager

parser = FriendlyArgumentParser()
parser.add_argument("--parallel", metavar = "proc_cnt", type = int, default = 1, help = "Specifies the number of concurrent processes to use during the build phase. Defaults to %(default)d.")
parser.add_argument("--do-setup", action = "store_true", help = "Setup the working enviroment (i.e., download and/or extract the Cortex-M4 compiler)")
parser.add_argument("--do-build-firmware", action = "store_true", help = "Build the firmware for the Cortex-M4")
parser.add_argument("--do-build-plugins", action = "store_true", help = "Build plugins for all architectures")
parser.add_argument("--do-package-binaries", action = "store_true", help = "Create a tar.gz archive that contains all build artifacts as well as some information text that contains build information")
parser.add_argument("--do-release-binaries", action = "store_true", help = "Upload the packaged tar.gz archive to GitHub as a release and set a tag that points to the source of the build")
parser.add_argument("--build-everything", action = "store_true", help = "Perform all do-* actions in the correct order")
parser.add_argument("-v", "--verbose", action = "store_true", help = "Be more verbose in regards to output logging")
args = parser.parse_args(sys.argv[1:])

root_logger = logging.getLogger()
if args.verbose:
	root_logger.setLevel(logging.DEBUG)
else:
	root_logger.setLevel(logging.INFO)

formatter = logging.Formatter("{asctime} [{levelname:.1s}]: {name} {message}", style = "{")

handler = logging.StreamHandler()
handler.setLevel(logging.DEBUG)
handler.setFormatter(formatter)
root_logger.addHandler(handler)

fpbm = FlightpanelBuildmanager(args)
if args.build_everything:
	action = fpbm.execute_all()
else:
	action = fpbm.execute()
if not action:
	print("No action specified.", file = sys.stderr)
