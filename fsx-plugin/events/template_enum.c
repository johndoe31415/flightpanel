#include <stdint.h>
#include "event-enum.h"

static const struct event_enum_data_t event_enum_data[] = {
%for event in events:
	[${event.enum_name}] = {
		.name = "${event.symbol}",
		.flags = ${event.flags},
	},
%endfor
};

const struct event_enum_data_t *get_event_enum_data(enum event_t value) {
	if ((value >= 0) && (value < EVENT_LAST)) {
		return &event_enum_data[value];
	} else {
		return NULL;
	}
}

const char *get_event_enum_name(enum event_t value) {
	const struct event_enum_data_t *data = get_event_enum_data(value);
	if (data) {
		return data->name;
	} else {
		return NULL;
	}
}
