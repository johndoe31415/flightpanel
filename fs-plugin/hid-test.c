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
 *
 * DOS:
 *		i686-w64-mingw32-gcc -Wall -I ../firmware -I hidapi -o hid-test.exe hid-test.c -lhidapi -Lbuild/windows && ./hid-test.exe
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <usb_hidreport.h>
#include <hidapi/hidapi.h>

#define USB_VID		0x0483
#define USB_PID		0x572b

int main(int argc, char **argv) {
	struct hid_device_info *info = hid_enumerate(USB_VID, USB_PID);
	if (!info) {
		fprintf(stderr, "Enumeration failed. No such device VID:PID %04x:%04x\n", USB_VID, USB_PID);
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
			fprintf(stderr, "Unexpected read: %d bytes read, %d expected.\n", bytes_read, (int)sizeof(struct hid_report_t));
		}
	}

	for (int i = 0; i < 50; i++) {
		struct hid_set_report_02_t set_report;
		memset(&set_report, 0, sizeof(set_report));
		set_report.report_id = 2;
		set_report.nav1.obs = i;
		set_report.nav2.obs = 100 + i;
		fprintf(stderr, "Set OBS1/2: %d %d\n", set_report.nav1.obs, set_report.nav2.obs);
		int bytes_written = hid_write(device, (uint8_t*)&set_report, sizeof(struct hid_set_report_02_t));
		if (bytes_written != sizeof(struct hid_set_report_02_t)) {
			fprintf(stderr, "Unexpected write: %d bytes written, %d expected.\n", bytes_written, (int)sizeof(struct hid_set_report_02_t));
		}
	}

	if (argc == 2) {
		int maxlen = atoi(argv[1]);
		fprintf(stderr, "Brute forcing up to length of %d\n", maxlen);
		uint8_t buffer[maxlen];
		memset(buffer, 0, sizeof(buffer));

		int failed = 0;
		int success = 0;
		for (int len = 0; len < sizeof(buffer); len++) {
			for (int i = 0; i < 5; i++) {
				buffer[0] = i;
				int bytes_written = hid_write(device, buffer, len);
				if (bytes_written != -1) {
					fprintf(stderr, "Success! Wrote %d bytes (length %d) at report %d\n", bytes_written, len, i);
					success++;
				} else {
					failed++;
				}
			}
		}
		fprintf(stderr, "%d failed transmissions, %d successful ones.\n", failed, success);
	}

	return 0;
}
