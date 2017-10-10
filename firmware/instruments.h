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

#ifndef __INSTRUMENTS_H__
#define __INSTRUMENTS_H__

#include <stdint.h>
#include <stdbool.h>
#include "rotary.h"
#include "debounce.h"
#include "usb_hidreport.h"

#define PIN_INVALID			255

struct rotary_encoder_with_button_t {
	struct rotary_encoder_t rotary;
	struct button_t button;
};

struct rotary_input_t {
	struct rotary_encoder_t *target;
	uint8_t pin1, pin2;
	bool *notify;
};

struct button_input_t {
	struct button_t *target;
	uint8_t pin;
	enum btnaction_t *notify;
};

enum ap_hold_t {
	HOLD_ALTITUDE = (1 << 0),
	HOLD_IAS = (1 << 1),
	HOLD_HEADING = (1 << 2),
	HOLD_NAVIGATION = (1 << 3),
	HOLD_REVERSE = (1 << 4),
	HOLD_APPROACH = (1 << 5),
};

struct instrument_state_t {
	uint16_t com1_active_index, com1_standby_index;
	uint16_t com2_active_index, com2_standby_index;
	uint16_t nav1_active_index, nav1_standby_index;
	uint16_t nav2_active_index, nav2_standby_index;
	struct {
		uint16_t squawk;
		bool mode_charly;
		uint8_t edit_char;
		uint16_t edit_timeout;
		bool identing;
		uint16_t ident_timeout;
	} xpdr;
	uint16_t qnh;

	struct {
		uint16_t altitude;
		uint16_t climbrate;
		uint16_t ias;
		uint16_t heading;

		bool active;
		uint8_t hold;
	} ap;
	bool navigate_by_gps;
};

struct flight_data_t {
	uint16_t ias;
	uint16_t altitude;
};

/*************** AUTO GENERATED SECTION FOLLOWS ***************/
void hid_tick(void);
void instruments_handle_inputs(void);
void dsr_idle_task(void);
void instruments_set_by_host(const struct hid_set_report_t *report);
void instruments_init(void);
/***************  AUTO GENERATED SECTION ENDS   ***************/

#endif
