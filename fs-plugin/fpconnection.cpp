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
#include <hidapi/hidapi.h>
#include "fpconnection.hpp"

FPConnection::FPConnection() {
	struct hid_device_info *info = hid_enumerate(USB_VID, USB_PID);
	if (!info) {
		throw std::runtime_error("Failed to hid_enumerate().");
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
}

void FPConnection::get_data(struct instrument_data_t *data) {
}

FPConnection::~FPConnection() {
	hid_close(_device);
}

