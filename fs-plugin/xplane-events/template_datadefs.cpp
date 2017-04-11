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
#include <string.h>
#include <math.h>
#include "xplane-datadefs.hpp"

void xplane_resolve_refs(struct xplane_datadef_refs_t &datadefs) {
	%for datadef in datadefs:
	datadefs.${datadef.symbol} = XPLMFindDataRef("${datadef.def_id}");
	if (!datadefs.${datadef.symbol}) {
		fprintf(stderr, "Unresolvable symbol: ${datadef.def_id}\n");
	}
	%endfor
}

void xplane_get_data(const struct xplane_datadef_refs_t &datadefs, struct xplane_datadef_data_t &data) {
	%for datadef in datadefs:
	%if datadef.datatype == "float":
	data.${datadef.symbol} = XPLMGetDataf(datadefs.${datadef.symbol});
	%elif datadef.datatype == "int":
	data.${datadef.symbol} = XPLMGetDatai(datadefs.${datadef.symbol});
	%elif datadef.datatype == "byte[500]":
	XPLMGetDatab(datadefs.${datadef.symbol}, datadefs.${datadef.symbol}, 0, 500);
	%else:
	// ??? ${datadef.datatype}
	%endif
	%endfor
}

void xplane_print_new(const struct xplane_datadef_data_t &old_data, const struct xplane_datadef_data_t &new_data) {
	%for datadef in datadefs:
	%if datadef.datatype == "float":
	if (fabs(old_data.${datadef.symbol} - new_data.${datadef.symbol}) > 0.1) {
		fprintf(stderr, "${datadef.def_id}: %.3f\n", new_data.${datadef.symbol});
	}
	%elif datadef.datatype == "int":
	if (old_data.${datadef.symbol} != new_data.${datadef.symbol}) {
		fprintf(stderr, "${datadef.def_id}: %u\n", new_data.${datadef.symbol});
	}
	%elif datadef.datatype == "byte[500]":
	if (memcmp(old_data.${datadef.symbol}, new_data.${datadef.symbol}, 500)) {
		fprintf(stderr, "${datadef.def_id}: ");
		for (int i = 0; i < 10; i++) {
			fprintf(stderr, "%02x", new_data.${datadef.symbol}[i]);
		}
		fprintf(stderr, "\n");
	}
	%else:
	// ??? ${datadef.datatype}
	%endif
	%endfor
}
