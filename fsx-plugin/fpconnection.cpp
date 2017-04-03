#include <stdio.h>
#include <stdlib.h>
#include <hidapi.h>
#include "fpconnection.hpp"

struct fpconnection_t* flightpanel_init(void) {
	struct fpconnection_t *context = (struct fpconnection_t*)calloc(1, sizeof(*context));
	if (!context) {
		return NULL;
	}

	struct hid_device_info *info = hid_enumerate(USB_VID, USB_PID);
	if (!info) {
		fprintf(stderr, "Failed to hid_enumerate().\n");
		free(context);
		return NULL;
	}

	struct hid_device_info *current = info;
	while (current) {
		if (current->serial_number == NULL) {
			fprintf(stderr, "Warning: No serial number provided, do you have rights to the node at %s?\n", current->path);
		}
		fprintf(stderr, "%04x:%04x: %ls : %ls, serial %ls\n", current->vendor_id, current->product_id, current->manufacturer_string, current->product_string, current->serial_number);
		current = current->next;
	}

	hid_device *device = hid_open(info->vendor_id, info->product_id, info->serial_number);
	if (!device) {
		fprintf(stderr, "Failed to hid_open().\n");
		exit(EXIT_FAILURE);
	}
	hid_free_enumeration(info);

	return context;
}

void flightpanel_loop(struct fpconnection_t *context) {
}

void flightpanel_close(struct fpconnection_t* context) {
	free(context);
}
