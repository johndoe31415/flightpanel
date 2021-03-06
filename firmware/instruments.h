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
	#define TRANSITION_QNH 1013

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

enum screen_qnh_t {
	DEFAULT,
	QNH,
};

enum display_state_t {
	BOOTING = 0,
	ACTIVE,
	BLANKED,
};

struct instrument_state_t {
	struct hid_report_t external;
	struct {
		struct {
			enum display_state_t state;
			uint32_t timer;
		} display_mode;
		struct {
			enum screen_qnh_t qnh;
			uint16_t qnh_timeout;
		} screen_mplex;
		struct {
			char nav1[5], nav2[5];
			char adf[5];
			uint16_t nav1_ident_inhibit_timeout;
			uint16_t nav2_ident_inhibit_timeout;
		} ident;
		struct {
			uint8_t edit_char;
			uint16_t edit_timeout;
			uint16_t ident_timeout;
		} xpdr;
		struct dme_info_t dme;
		uint8_t active_obs;
	} internal;
};

/*************** AUTO GENERATED SECTION FOLLOWS ***************/
void hid_tick(void);
void instruments_handle_inputs(void);
void dsr_idle_task(void);
void instruments_set_by_host(const union hid_set_report_t *report);
void instruments_init(void);
/***************  AUTO GENERATED SECTION ENDS   ***************/

#endif
