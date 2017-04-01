#ifndef __SIMCONNECT_EVENTS_H__
#define __SIMCONNECT_EVENTS_H__

#include <stdint.h>

struct simconnect_event_enum_data_t {
	uint32_t flags;
	const char *name;
};

enum simconnect_event_t {
%for event in events:
	${event.enum_name},			// ${event.text}
%endfor
	EVENT_LAST
};

#ifdef __cplusplus
extern "C" {
#endif

const struct simconnect_event_enum_data_t *get_simconnect_event_enum_data(enum simconnect_event_t value);
const char *get_simconnect_event_enum_name(enum simconnect_event_t value);

#ifdef __cplusplus
};
#endif

#endif
