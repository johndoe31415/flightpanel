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
#include <cstring>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <hidapi/hidapi.h>
#include <firmware/usb_hidreport.h>
#include <firmware/frequencies.h>
#include "fpconnection.hpp"

#define NO_REAL_DEVICE

static void* event_loop_thread(void *ctx) {
	FPConnection *connection = (FPConnection*)ctx;
	connection->event_loop();
	return NULL;
}

FPConnection::FPConnection() : _device(NULL) {
#ifndef NO_REAL_DEVICE
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
#endif

	std::memset(&_instrument_data, 0, sizeof(struct instrument_data_t));
	pthread_create(&_periodic_query_thread, NULL, event_loop_thread, this);
}

void FPConnection::event_loop() {
	_run_event_loop = true;
	while (_run_event_loop) {
		struct hid_report_t hid_report;
#ifdef NO_REAL_DEVICE
		usleep(100 * 1000);
		continue;
#endif
		int bytes_read = hid_read(_device, (uint8_t*)&hid_report, sizeof(hid_report));
		if (bytes_read == sizeof(hid_report)) {
			_instrument_data.com1.freq_active_khz = frequency_index_to_khz((enum com_nav_range_t)hid_report.com_divisions, hid_report.com1.active_index);
			_instrument_data.com1.freq_standby_khz = frequency_index_to_khz((enum com_nav_range_t)hid_report.com_divisions, hid_report.com1.standby_index);
			_instrument_data.com2.freq_active_khz = frequency_index_to_khz((enum com_nav_range_t)hid_report.com_divisions, hid_report.com2.active_index);
			_instrument_data.com2.freq_standby_khz = frequency_index_to_khz((enum com_nav_range_t)hid_report.com_divisions, hid_report.com2.standby_index);
			_instrument_data.nav1.freq_active_khz = frequency_index_to_khz((enum com_nav_range_t)hid_report.nav_divisions, hid_report.nav1.active_index);
			_instrument_data.nav1.freq_standby_khz = frequency_index_to_khz((enum com_nav_range_t)hid_report.nav_divisions, hid_report.nav1.standby_index);
			_instrument_data.nav2.freq_active_khz = frequency_index_to_khz((enum com_nav_range_t)hid_report.nav_divisions, hid_report.nav2.active_index);
			_instrument_data.nav2.freq_standby_khz = frequency_index_to_khz((enum com_nav_range_t)hid_report.nav_divisions, hid_report.nav2.standby_index);
		} else {
			fprintf(stderr, "Short read (%d of %zd), could not get full HID report.\n", bytes_read, sizeof(hid_report));
		}
	}
}

void FPConnection::get_data(struct instrument_data_t *data) {
	memcpy(data, &_instrument_data, sizeof(*data));
}

void FPConnection::put_data(const struct instrument_data_t *data, const struct component_selection_t *selection) {
	struct hid_set_report_t hid_set_report;
	std::memset(&hid_set_report, 0, sizeof(hid_set_report));
	hid_set_report.report_id = 1;
	// TODO this is all different now
#if 0
	hid_set_report.com1_active = com_frequency_khz_to_index(data->com1.freq_active_khz);
	hid_set_report.com1_standby = com_frequency_khz_to_index(data->com1.freq_standby_khz);
	hid_set_report.com2_active = com_frequency_khz_to_index(data->com2.freq_active_khz);
	hid_set_report.com2_standby = com_frequency_khz_to_index(data->com2.freq_standby_khz);
	hid_set_report.nav1_active = nav_frequency_khz_to_index(data->nav1.freq_active_khz);
	hid_set_report.nav1_standby = nav_frequency_khz_to_index(data->nav1.freq_standby_khz);
	hid_set_report.nav2_active = nav_frequency_khz_to_index(data->nav2.freq_active_khz);
	hid_set_report.nav2_standby = nav_frequency_khz_to_index(data->nav2.freq_standby_khz);
#endif
#ifndef NO_REAL_DEVICE
	int bytes_written = hid_write(_device, (const uint8_t*)&hid_set_report, sizeof(hid_set_report));
#else
	int bytes_written = sizeof(hid_set_report);
#endif
	if (bytes_written != sizeof(hid_set_report)) {
		fprintf(stderr, "Sending HID report error: tried sending %zd bytes, but %d went through.\n", sizeof(hid_set_report), bytes_written);
	}
}

FPConnection::~FPConnection() {
	_run_event_loop = false;
	pthread_join(_periodic_query_thread, NULL);
	hid_close(_device);
}

