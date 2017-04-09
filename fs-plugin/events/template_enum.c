#include <stdio.h>
#include <stdint.h>
#include "simconnect-events.h"

static const struct simconnect_event_enum_data_t event_enum_data[] = {
%for event in events:
	[${event.enum_name}] = {
		.name = "${event.symbol}",
		.flags = ${event.flags},
	},
%endfor
};

const struct simconnect_event_enum_data_t *get_simconnect_event_enum_data(enum simconnect_event_t value) {
	if ((value >= 0) && (value < EVENT_LAST)) {
		return &event_enum_data[value];
	} else {
		return NULL;
	}
}

const char *get_simconnect_event_enum_name(enum simconnect_event_t value) {
	const struct simconnect_event_enum_data_t *data = get_simconnect_event_enum_data(value);
	if (data) {
		return data->name;
	} else {
		return NULL;
	}
}
