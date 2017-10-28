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

#ifndef __SIMCONNECT_DATADEFS_HPP__
#define __SIMCONNECT_DATADEFS_HPP__

#include <stdint.h>

%for (group, defs) in sorted(datadefs.items()):
struct simconnect_datatype_${group}_t {
	%for member in defs:
	%if member.datatype == "char[256]":
	char ${member.varname}[256];
	%elif member.datatype == "char[8]":
	char ${member.varname}[8];
	%else:
	${member.datatype} ${member.varname};
	%endif
	%endfor
} __attribute__((packed));
static_assert(sizeof(struct simconnect_datatype_${group}_t) == ${sum(member.datatypesize for member in defs)}, "struct simconnect_datatype_${group}_t incorrectly packed.");
#define SIMCONNECT_DATATYPE_${group.upper()}_DATUM_COUNT			${len(defs)}

%endfor

%for (group, defs) in sorted(datadefs.items()):
void simconnect_datadefs_register_${group}(void *hSimConnect, int group_id);
%endfor

#endif
