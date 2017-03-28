#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <hidapi.h>

int main(void) {
	if (hid_init()) {
		fprintf(stderr, "Failed to hid_init().\n");
		exit(EXIT_FAILURE);
	}

	struct hid_device_info *info = hid_enumerate(0x0483, 0x572b);
	if (!info) {
		fprintf(stderr, "Failed to hid_enumerate().\n");
		exit(EXIT_FAILURE);
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

	uint8_t data[128];
	while (true) {
		int bytes_read = hid_read(device, data, sizeof(data));
		fprintf(stderr, "Read %d: ", bytes_read);
		for (int i = 0; i < bytes_read; i++) {
			fprintf(stderr, "%02x ", data[i]);
		}
		fprintf(stderr, "\n");
		if ((bytes_read > 0) && ((data[0] % 30) == 0x0)) {
			memset(data, 0, sizeof(data));
			data[1] = 0xab;
			int written = hid_write(device, data, 2);
			fprintf(stderr, "Write: %d\n", written);
		}
	}

	return 0;
}
