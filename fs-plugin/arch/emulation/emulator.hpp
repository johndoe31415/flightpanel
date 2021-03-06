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

#ifndef __EMULATOR_HPP__
#define __EMULATOR_HPP__

#include <stdbool.h>
#include <pthread.h>
#include "fsconnection.hpp"

class EmulatedConnection : public FSConnection {
	private:
		pthread_t _polling_thread;
		bool _loop_running;

		struct instrument_data_t _instrument_data;

	private:
		void randomize(struct com_state_t *com);
		void randomize(struct nav_state_t *nav);
		void randomize(struct adf_state_t *adf);
		void randomize(struct ap_state_t *ap);
		void randomize(struct xpdr_state_t *xpdr);
		void randomize(struct hid_report_t *misc);
		void randomize(struct internal_state_t *internal);
		void randomize(char ident[IDENT_LENGTH_BYTES]);

	public:
		EmulatedConnection();
		bool connected(void) const {
			return true;
		}
		virtual void get_data(struct instrument_data_t *data);
		virtual void put_data(const struct instrument_data_t &data, const arbiter_elements_t &elements);
		void poke();
		~EmulatedConnection();
};

#endif
