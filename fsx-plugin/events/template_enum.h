#ifndef __EVENT_ENUM_H__
#define __EVENT_ENUM_H__

#include <stdint.h>

struct event_enum_data_t {
	uint32_t flags;
	const char *name;
};

enum event_t {
%for event in events:
	${event.enum_name},			// ${event.text}
%endfor
	EVENT_LAST
};

#ifdef __cplusplus
extern "C" {
#endif

const struct event_enum_data_t *get_event_enum_data(enum event_t value);
const char *get_event_enum_name(enum event_t value);

#ifdef __cplusplus
};
#endif

#endif
