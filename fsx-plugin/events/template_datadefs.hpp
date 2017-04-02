#ifndef __SIMCONNECT_DATADEFS_HPP__
#define __SIMCONNECT_DATADEFS_HPP__

#include <stdint.h>

%for (group, defs) in sorted(datadefs.items()):
struct simconnect_datatype_${group}_t {
	%for member in defs:
	%if member.datatype == "char[256]":
	char ${member.varname}[256];
	%else:
	${member.datatype} ${member.varname};
	%endif
	%endfor
} __attribute__((packed));
static_assert(sizeof(struct simconnect_datatype_${group}_t) == ${sum(member.datatypesize for member in defs)}, "struct simconnect_datatype_${group}_t incorrectly packed.");

%endfor

%for (group, defs) in sorted(datadefs.items()):
void simconnect_datadefs_register_${group}(void *hSimConnect, int group_id);
%endfor

#endif
