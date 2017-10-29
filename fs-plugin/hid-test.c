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

/*
 * Linux:
 *		gcc -Wall -I ../firmware -I hidapi -o hid-test hid-test.c -lhidapi-libusb && ./hid-test
 */

#include <stdio.h>
#include <usb_hidreport.h>
#include <hidapi/hidapi.h>

#define USB_VID		0x0483
#define USB_PID		0x572b

int main(void) {
	struct hid_device_info *info = hid_enumerate(USB_VID, USB_PID);
	if (!info) {
		fprintf(stderr, "Enumeration failed.\n");
		return 1;
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
	hid_free_enumeration(info);
	if (!device) {
		fprintf(stderr, "Failed to hid_open(). Does another process claim the device already?\n");
		return 1;
	}
	fprintf(stderr, "Successfully connected to flightpanel.\n");

	for (int i = 0; i < 50; i++) {
		struct hid_report_t hid_report;
		int bytes_read = hid_read(device, (uint8_t*)&hid_report, sizeof(struct hid_report_t));
		if (bytes_read == sizeof(struct hid_report_t)) {
			fprintf(stderr, "Read OBS1/2: %d %d\n", hid_report.nav1.obs, hid_report.nav2.obs);
		} else {
			fprintf(stderr, "Unexpected read: %d bytes read, %d expected.\n", bytes_read, sizeof(struct hid_report_t));
		}
	}

	for (int i = 0; i < 50; i++) {
		struct hid_set_report_01_t set_report;
		memset(&set_report, 0, sizeof(set_report));
		set_report.report_id = 1;
		set_report.nav1.obs = i;
		set_report.nav2.obs = 100 + i;
		fprintf(stderr, "Set OBS1/2: %d %d\n", set_report.nav1.obs, set_report.nav2.obs);
		int bytes_written = hid_write(device, (uint8_t*)&set_report, sizeof(struct hid_set_report_01_t));
		if (bytes_written != sizeof(struct hid_set_report_01_t)) {
			fprintf(stderr, "Unexpected write: %d bytes written, %d expected.\n", bytes_written, sizeof(struct hid_set_report_01_t));
		}
	}

	return 0;
}
