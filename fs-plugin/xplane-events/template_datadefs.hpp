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
