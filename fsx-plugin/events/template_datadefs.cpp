#include <stdio.h>
#include <stdint.h>
#include <windows.h>
#include "SimConnect.h"
#include "simconnect-datadefs.hpp"

%for (group, defs) in sorted(datadefs.items()):
void simconnect_datadefs_register_${group}(void *hSimConnect, int group_id) {
	%for member in defs:
	%if member.datatype in [ "char[256]" ]:
	SimConnect_AddToDataDefinition(hSimConnect, group_id, "${member.name}", NULL, ${member.simconnect_type});
	%else:
	SimConnect_AddToDataDefinition(hSimConnect, group_id, "${member.name}", "${member.unit}", ${member.simconnect_type});
	%endif
	%endfor
}

%endfor
