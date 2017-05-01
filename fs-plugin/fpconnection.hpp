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

#ifndef __FPCONNECTION_HPP__
#define __FPCONNECTION_HPP__

#include <pthread.h>
#include <hidapi/hidapi.h>
#include "fsconnection.hpp"

#define USB_VID		0x0483
#define USB_PID		0x572b

class FPConnection {
	private:
		hid_device *_device;

	public:
		FPConnection();
		bool connected() const {
			return _device != NULL;
		}
		void get_data(struct instrument_data_t *data);
		~FPConnection();
};

/*************** AUTO GENERATED SECTION FOLLOWS ***************/
struct fpconnection_t* flightpanel_init(void);
void flightpanel_close(struct fpconnection_t* context);
/***************  AUTO GENERATED SECTION ENDS   ***************/

#endif
