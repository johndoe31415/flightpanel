/**
 *	flightpanel - A Cortex-M4 based USB flight panel for flight simulators.
 *	Copyright (C) 2017-2017 Johannes Bauer
 *
 *	This file is part of flightpanel.
 *
 *	flightpanel is free software; you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation; this program is ONLY licensed under
 *	version 3 of the License, later versions are explicitly excluded.
 *
 *	flightpanel is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with flightpanel; if not, write to the Free Software
 *	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *	Johannes Bauer <JohannesBauer@gmx.de>
**/

#include <stdio.h>
#include <stdint.h>
#include <windows.h>
#include "SimConnect.h"
#include "simconnect-datadefs.hpp"

%for (group, defs) in sorted(datadefs.items()):
void simconnect_datadefs_register_${group}(void *hSimConnect, int group_id) {
	%for member in defs:
	%if member.datatype in [ "char[256]", "char[8]" ]:
	SimConnect_AddToDataDefinition(hSimConnect, group_id, "${member.name}", NULL, ${member.simconnect_type});
	%else:
	SimConnect_AddToDataDefinition(hSimConnect, group_id, "${member.name}", "${member.unit}", ${member.simconnect_type});
	%endif
	%endfor
}

%endfor
