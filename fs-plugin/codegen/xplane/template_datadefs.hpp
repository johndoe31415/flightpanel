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

#ifndef __XPLANE_DATADEFS_HPP__
#define __XPLANE_DATADEFS_HPP__

#include <stdint.h>
#include <XPLMDataAccess.h>

struct xplane_datadef_refs_t {
	%for datadef in datadefs:
	XPLMDataRef ${datadef.symbol};
	%endfor
};

struct xplane_datadef_data_t {
	%for datadef in datadefs:
	%if datadef.datatype == "float":
	float ${datadef.symbol};
	%elif datadef.datatype == "int":
	int ${datadef.symbol};
	%elif datadef.datatype == "byte[500]":
	uint8_t ${datadef.symbol}[500];
	%else:
	// ??? ${datadef.datatype}
	%endif
	%endfor
};

void xplane_resolve_refs(struct xplane_datadef_refs_t &datadefs);
void xplane_get_data(const struct xplane_datadef_refs_t &datadefs, struct xplane_datadef_data_t &data);
void xplane_print_new(const struct xplane_datadef_data_t &old_data, const struct xplane_datadef_data_t &new_data);

#endif
