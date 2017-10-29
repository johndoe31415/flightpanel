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
#include "inttypes_emulation.h"
#include "fpconnection.hpp"
#include "osdeps.hpp"
#include "logging.hpp"

FPConnection::FPConnection() : Thread(500), _device(NULL), _data_initialized(false), _last_seqno(0x55) {
	std::memset(&_instrument_data, 0, sizeof(struct instrument_data_t));
	start();
}

void FPConnection::disconnect() {
	LockGuard guard(_devicelock, "disconnect");
	if (_device) {
		hid_close(_device);
		_device = NULL;
		_data_initialized = false;
	}
}

bool FPConnection::connect() {
	LockGuard guard(_devicelock, "connect");
	if (_device) {
		return true;
	}

	logmsg(LLVL_INFO, "Trying to connect to flightpanel...");
	struct hid_device_info *info = hid_enumerate(USB_VID, USB_PID);
	if (!info) {
		logmsg(LLVL_INFO, "Failed to hid_enumerate(): No such VID/PID %04x:%04x", USB_VID, USB_PID );
		return false;
	}

	struct hid_device_info *current = info;
	while (current) {
		if (current->serial_number == NULL) {
			logmsg(LLVL_INFO, "Warning: No serial number provided, do you have rights to the node at %s?", current->path);
		}
		logmsg(LLVL_INFO, "%04x:%04x: %ls : %ls, serial %ls", current->vendor_id, current->product_id, current->manufacturer_string, current->product_string, current->serial_number);
		current = current->next;
	}

	_device = hid_open(info->vendor_id, info->product_id, info->serial_number);
	hid_free_enumeration(info);
	if (!_device) {
		logmsg(LLVL_INFO, "Failed to hid_open().");
		return false;
	}
	logmsg(LLVL_INFO, "Successfully connected to flightpanel.");
	return true;
}

void FPConnection::thread_action() {
	if (!connect()) {
		return;
	}

	struct hid_report_t hid_report;
	while (thread_running()) {
		hid_device *device;
		{
			LockGuard guard(_devicelock, "thread_action");
			device = _device;
			if (!device) {
				break;
			}
		}
		logmsg(LLVL_DEBUG, "Reading from flightpanel...");
		int bytes_read = hid_read(device, (uint8_t*)&hid_report, sizeof(hid_report));
		logmsg(LLVL_INFO, "FP read %d bytes", bytes_read);
		if (bytes_read == sizeof(hid_report)) {
			LockGuard guard(_datalock);
			_instrument_data.external = hid_report;
			_data_fresh.set();
		} else if (bytes_read == -1) {
			logmsg(LLVL_INFO, "Flight panel diconnected.");
			disconnect();
		} else {
			logmsg(LLVL_INFO, "Short read (%d of %" PRIsizet "), could not get full HID report.", bytes_read, sizeof(hid_report));
			disconnect();
		}
	}
}

void FPConnection::get_data(struct instrument_data_t *data) {
	LockGuard guard(_datalock);
	memcpy(data, &_instrument_data, sizeof(*data));
}

template<typename T> bool FPConnection::send_report(T *report) {
	hid_device *device;
	{
		LockGuard guard(_devicelock, "connect");
		device = _device;
		if (!device) {
			return false;
		}
		_last_seqno++;
		report->seqno = _last_seqno;
	}
	int bytes_written = hid_write(device, (const uint8_t*)report, sizeof(*report));
	if (bytes_written != sizeof(*report)) {
		logmsg(LLVL_INFO, "Sending HID report error: tried sending %" PRIsizet " bytes, but %d went through.", sizeof(*report), bytes_written);
		disconnect();
		return false;
	}
	return true;
}

void FPConnection::put_data(const struct instrument_data_t &data, const struct arbiter_elements_t &elements, bool send_all) {
	if (!connect()) {
		return;
	}

	if (send_all || elements.fp_send_report_01()) {
		logmsg(LLVL_INFO, "Sending report #1");
		struct hid_set_report_01_t report;
		std::memset(&report, 0, sizeof(report));
		report.report_id = 1;
		report.radio_panel = data.external.radio_panel;
		report.com_divisions = data.external.com_divisions;
		report.nav_divisions = data.external.nav_divisions;
		report.com1 = data.external.com1;
		report.com2 = data.external.com2;
		report.nav1 = data.external.nav1;
		report.nav2 = data.external.nav2;
		report.xpdr = data.external.xpdr;
		report.adf = data.external.adf;
		report.ap = data.external.ap;
		report.qnh = data.external.qnh;
		report.navigate_by_gps = data.external.navigate_by_gps;
		if (send_report(&report)) {
			_instrument_data.external = data.external;
		}
	}
	if (send_all || elements.fp_send_report_02()) {
		logmsg(LLVL_INFO, "Sending report #2");
		struct hid_set_report_02_t report;
		std::memset(&report, 0, sizeof(report));
		report.report_id = 2;
		memcpy(&report.ident.nav1, &data.internal.ident.nav1, IDENT_LENGTH_BYTES);
		memcpy(&report.ident.nav2, &data.internal.ident.nav2, IDENT_LENGTH_BYTES);
		memcpy(&report.ident.adf, &data.internal.ident.adf, IDENT_LENGTH_BYTES);
		memcpy(&report.dme, &data.internal.dme, sizeof(data.internal.dme));
		if (send_report(&report)) {
			_instrument_data.internal = data.internal;
		}
	}
	_data_initialized = true;
	_data_fresh.reset();
}

void FPConnection::put_data(const struct instrument_data_t &data, const struct arbiter_elements_t &elements) {
	put_data(data, elements, !_data_initialized);
}

void FPConnection::wait_for_ack() {
	while (_instrument_data.external.seqno != _last_seqno) {
		//logmsg(LLVL_INFO, "Waiting for FP seqno %x (currently %x)...", _last_seqno, _instrument_data.external.seqno);
		sleep_millis(10);
	}
}

FPConnection::~FPConnection() {
	disconnect();
}

