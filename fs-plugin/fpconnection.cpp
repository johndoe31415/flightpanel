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

#include <stdexcept>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <hidapi/hidapi.h>
#include <firmware/usb_hidreport.h>
#include <firmware/frequencies.h>
#include "fpconnection.hpp"

static void* event_loop_thread(void *ctx) {
	FPConnection *connection = (FPConnection*)ctx;
	connection->event_loop();
	return NULL;
}

FPConnection::FPConnection() {
	struct hid_device_info *info = hid_enumerate(USB_VID, USB_PID);
	if (!info) {
		throw std::runtime_error("Failed to hid_enumerate(): No such VID/PID.");
	}

	struct hid_device_info *current = info;
	while (current) {
		if (current->serial_number == NULL) {
			fprintf(stderr, "Warning: No serial number provided, do you have rights to the node at %s?\n", current->path);
		}
		fprintf(stderr, "%04x:%04x: %ls : %ls, serial %ls\n", current->vendor_id, current->product_id, current->manufacturer_string, current->product_string, current->serial_number);
		current = current->next;
	}

	_device = hid_open(info->vendor_id, info->product_id, info->serial_number);
	hid_free_enumeration(info);
	if (!_device) {
		throw std::runtime_error("Failed to hid_open().");
	}

	memset(&_instrument_data, 0, sizeof(struct instrument_data_t));
	pthread_create(&_periodic_query_thread, NULL, event_loop_thread, this);
}

void FPConnection::event_loop() {
	while (true) {
		struct hid_report_t hid_report;
		int bytes_read = hid_read(_device, (uint8_t*)&hid_report, sizeof(hid_report));
		if (bytes_read == sizeof(hid_report)) {
			_instrument_data.com1.freq_active_khz = com_index_to_frequency_khz(hid_report.com1_active);
			_instrument_data.com1.freq_standby_khz = com_index_to_frequency_khz(hid_report.com1_standby);
		} else {
			fprintf(stderr, "Short read (%d of %zd), could not get full HID report.\n", bytes_read, sizeof(hid_report));
		}
	}
}

void FPConnection::get_data(struct instrument_data_t *data) {
	memcpy(data, &_instrument_data, sizeof(struct instrument_data_t));
}

void FPConnection::put_data(const struct instrument_data_t *data, const struct component_selection_t *selection) {
	struct hid_set_report_t hid_set_report;
	memset(&hid_set_report, 0, sizeof(hid_set_report));
	hid_set_report.report_id = 1;
	hid_set_report.com1_active = com_frequency_khz_to_index(data->com1.freq_active_khz);
	hid_set_report.com1_standby = com_frequency_khz_to_index(data->com1.freq_standby_khz);

	int bytes_written = hid_write(_device, (const uint8_t*)&hid_set_report, sizeof(hid_set_report));
	if (bytes_written != sizeof(hid_set_report)) {
		fprintf(stderr, "Sending HID report error: tried sending %zd bytes, but %d went through.\n", sizeof(hid_set_report), bytes_written);
	}
}

FPConnection::~FPConnection() {
	hid_close(_device);
}

