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
#include <stdbool.h>
#include <stdint.h>
#include <math.h>
#include <stddef.h>
#include <unistd.h>
#include "inttypes_emulation.h"
#include "simconnect-events.h"
#include "simconnect-datadefs.hpp"
#include "fsconnection.hpp"
#include "simconnect.hpp"

static uint32_t bcd_to_decimal(uint32_t bcd_value) {
	uint32_t result = 0;
	uint32_t value = 1;
	while (bcd_value) {
		uint8_t digit = bcd_value & 0xf;
		result += digit * value;
		value *= 10;
		bcd_value >>= 4;
	}
	return result;
}

#if 0
static uint32_t frequency_bcd16_to_khz(uint32_t bcd16_value) {
	uint32_t khz = 100000 + (bcd_to_decimal(bcd16_value) * 10);
	if (((khz % 100) == 20) || ((khz % 100) == 70)) {
		khz += 5;
	}
	return khz;
}
#endif

static uint32_t frequency_bcd32_to_khz(uint32_t bcd32_value) {
	return bcd_to_decimal(bcd32_value) / 10;
}

static uint32_t adf_frequency_bcd32_to_hz(uint32_t bcd32_value) {
	return bcd_to_decimal(bcd32_value) / 10;
}

static void simconnect_instrument_to_abstract(const struct simconnect_datatype_instruments_t *in, struct instrument_data_t *out) {
	out->vhf1.freq_active_khz = frequency_bcd32_to_khz(in->vhf1_freq_active);
	out->vhf1.freq_standby_khz = frequency_bcd32_to_khz(in->vhf1_freq_standby);
	out->vhf1.tx = in->vhf1_tx;
	out->vhf2.freq_active_khz = frequency_bcd32_to_khz(in->vhf2_freq_active);
	out->vhf2.freq_standby_khz = frequency_bcd32_to_khz(in->vhf2_freq_standby);
	out->vhf2.tx = in->vhf2_tx;
	if (in->vhf_all_rx) {
		out->vhf1.rx = true;
		out->vhf2.rx = true;
	} else {
		out->vhf1.rx = out->vhf1.tx;
		out->vhf2.rx = out->vhf2.tx;
	}

	out->nav1.freq_active_khz = frequency_bcd32_to_khz(in->nav1_freq_active);
	out->nav1.freq_standby_khz = frequency_bcd32_to_khz(in->nav1_freq_standby);
	out->nav1.obs = round(in->nav1_obs_deg);
	out->nav1.sound = in->nav1_sound;
	memcpy(out->nav1.ident, in->nav1_ident, 8);

	out->nav2.freq_active_khz = frequency_bcd32_to_khz(in->nav2_freq_active);
	out->nav2.freq_standby_khz = frequency_bcd32_to_khz(in->nav2_freq_standby);
	out->nav2.obs = round(in->nav2_obs_deg);
	out->nav2.sound = in->nav2_sound;
	memcpy(out->nav2.ident, in->nav2_ident, 8);

	out->adf.freq_hz = adf_frequency_bcd32_to_hz(in->adf_freq_active);
	out->adf.compass_rose = round(in->adf_compass_rose_deg);
	out->adf.sound = in->adf_sound;
	memcpy(out->adf.ident, in->adf_ident, 8);

	out->dme.nav_id = in->dme_selected;
	out->dme.available = ((out->dme.nav_id == 1) && (in->dme_nav1)) || ((out->dme.nav_id == 2) && (in->dme_nav2));
	out->dme.distance_nm_tenths = round(in->dme_distance * 10);
	out->dme.speed_kt = round(in->dme_speed);
	out->dme.sound = in->dme_sound;

	out->xpdr.squawk = bcd_to_decimal(in->xpdr_squawk);

	out->lights.nav = (in->light_states & 0x0001);
	out->lights.beacon = (in->light_states & 0x0002);
	out->lights.landing = (in->light_states & 0x0004);
	out->lights.taxi = (in->light_states & 0x0008);
	out->lights.strobe = (in->light_states & 0x0010);

	out->ap.active = in->ap_master;
	out->ap.altitude_ft = round(in->ap_altitude_ft);
	out->ap.climbrate_ft_per_min = round(in->ap_climbrate_ft_per_min);
	out->ap.heading = round(in->ap_heading_deg);
	out->ap.hdg_hold = in->ap_hdg_hold;
	out->ap.nav_hold = in->ap_nav_hold;
	out->ap.apr_hold = in->ap_apr_hold;
	out->ap.rev_hold = in->ap_rev_hold;
	out->ap.alt_hold = in->ap_alt_hold;
	out->ap.ias_hold = in->ap_ias_hold;

	out->misc.ias_kt = round(in->ias);
	out->misc.indicated_alt_ft = round(in->indicated_alt_ft);
	out->misc.qnh_millibar = round(in->qnh_millibar);
	out->misc.guide_gps = in->gps_drives_nav;
}

enum event_group_t {
	EVENTGROUP_INSTRUMENT_CHANGED
};

enum datadefinition_t {
	DATADEF_INFO,
	DATADEF_INSTRUMENTS
};

enum data_request_t {
	REQUEST_DATADEF_INFO,
	REQUEST_DATADEF_INSTRUMENTS
};

static void CALLBACK simconnect_callback_wrapper(SIMCONNECT_RECV* pData, DWORD cbData, void *vcontext) {
	SimConnectConnection *conn = (SimConnectConnection*)vcontext;
	conn->simconnect_callback(pData, cbData);
}

void SimConnectConnection::simconnect_callback(SIMCONNECT_RECV *pData, DWORD cbData) {
	if (pData->dwID == SIMCONNECT_RECV_ID_EVENT) {
		SIMCONNECT_RECV_EVENT *evt = (SIMCONNECT_RECV_EVENT*)pData;
		enum simconnect_event_t event_id = (enum simconnect_event_t)evt->uEventID;
		if (event_id == EVENT_SIM_START) {
			printf("Simulation started.\n");
			SimConnect_RequestDataOnSimObjectType(_simconnect_handle, REQUEST_DATADEF_INFO, DATADEF_INFO, 0, SIMCONNECT_SIMOBJECT_TYPE_USER);
			SimConnect_RequestDataOnSimObjectType(_simconnect_handle, REQUEST_DATADEF_INSTRUMENTS, DATADEF_INSTRUMENTS, 0, SIMCONNECT_SIMOBJECT_TYPE_USER);
		} else if (evt->uGroupID == EVENTGROUP_INSTRUMENT_CHANGED) {
			//printf("Instrument changed: %s\n", get_event_enum_name(event_id));
			SimConnect_RequestDataOnSimObjectType(_simconnect_handle, REQUEST_DATADEF_INSTRUMENTS, DATADEF_INSTRUMENTS, 0, SIMCONNECT_SIMOBJECT_TYPE_USER);
		} else {
			printf("Unhandled event received: 0x%lx\n", evt->uEventID);
		}
	} else if (pData->dwID == SIMCONNECT_RECV_ID_SIMOBJECT_DATA_BYTYPE) {
		SIMCONNECT_RECV_SIMOBJECT_DATA_BYTYPE *pObjData = (SIMCONNECT_RECV_SIMOBJECT_DATA_BYTYPE*)pData;
		DWORD object_id = pObjData->dwObjectID;
		if (pObjData->dwRequestID == REQUEST_DATADEF_INFO) {
			struct simconnect_datatype_information_t *fdata = (struct simconnect_datatype_information_t*)&pObjData->dwData;
			fdata->title[255] = 0;
			printf("Receive data about object %ld: %s\n", object_id, fdata->title);
		} else if (pObjData->dwRequestID == REQUEST_DATADEF_INSTRUMENTS) {
			struct simconnect_datatype_instruments_t *simconnect_data = (struct simconnect_datatype_instruments_t*)&pObjData->dwData;
			if (SIMCONNECT_DATATYPE_INSTRUMENTS_DATUM_COUNT != pObjData->dwDefineCount) {
				printf("Warning: expected %u datums, but received %lu.\n", SIMCONNECT_DATATYPE_INSTRUMENTS_DATUM_COUNT, pObjData->dwDefineCount);
			}
			struct instrument_data_t abstract_data;
			memset(&abstract_data, 0, sizeof(abstract_data));
			simconnect_instrument_to_abstract(simconnect_data, &_instrument_data);
		} else {
			printf("Recevied unhandled data for request 0x%lx\n", pObjData->dwRequestID);
		}
	} else if (pData->dwID == SIMCONNECT_RECV_ID_QUIT) {
		_loop_running = false;
		printf("Received termination signal.\n");
	} else {
		printf("Unhandled callback dwID 0x%lx\n", pData->dwID);
	}
}

void SimConnectConnection::get_data(struct instrument_data_t *data) {
	memcpy(data, &_instrument_data, sizeof(struct instrument_data_t));
}

static void* event_loop_thread(void *ctx) {
	SimConnectConnection *connection = (SimConnectConnection*)ctx;
	connection->event_loop();
	return NULL;
}

void SimConnectConnection::event_loop() {
	_loop_running = true;
	SimConnect_RequestDataOnSimObjectType(_simconnect_handle, REQUEST_DATADEF_INFO, DATADEF_INFO, 0, SIMCONNECT_SIMOBJECT_TYPE_USER);
	SimConnect_RequestDataOnSimObjectType(_simconnect_handle, REQUEST_DATADEF_INSTRUMENTS, DATADEF_INSTRUMENTS, 0, SIMCONNECT_SIMOBJECT_TYPE_USER);


	while (_loop_running) {
		SimConnect_RequestDataOnSimObjectType(_simconnect_handle, REQUEST_DATADEF_INSTRUMENTS, DATADEF_INSTRUMENTS, 0, SIMCONNECT_SIMOBJECT_TYPE_USER);
		SimConnect_CallDispatch(_simconnect_handle, simconnect_callback_wrapper, this);
		Sleep(10);
	}

	/* Clean up loop thread */
//	pthread_join(_periodic_query_thread, NULL);
}

SimConnectConnection::SimConnectConnection() {
	_simconnect_handle = NULL;
	_loop_running = false;
	memset(&_periodic_query_thread, 0, sizeof(_periodic_query_thread));

	if (SUCCEEDED(SimConnect_Open(&_simconnect_handle, "Flight Panel", NULL, 0, 0, 0))) {
		printf("Successfully connected to flight simulator via SimConnect.\n");
		simconnect_datadefs_register_information(_simconnect_handle, DATADEF_INFO);
		simconnect_datadefs_register_instruments(_simconnect_handle, DATADEF_INSTRUMENTS);
		SimConnect_SubscribeToSystemEvent(_simconnect_handle, EVENT_SIM_START, "SimStart");

		/* Register all enum values which have flag 1 for instrument changed events */
		int event_enum = 0;
		const struct simconnect_event_enum_data_t *event_data;
		while ((event_data = get_simconnect_event_enum_data((simconnect_event_t)event_enum)) != NULL) {
			if (event_data->flags == 1) {
				/* Instrument changed group */
				SimConnect_MapClientEventToSimEvent(_simconnect_handle, event_enum, event_data->name);
				SimConnect_AddClientEventToNotificationGroup(_simconnect_handle, EVENTGROUP_INSTRUMENT_CHANGED, event_enum);
			}
			event_enum++;
		}
	} else {
		throw std::runtime_error("Connection to flight simulator via SimConnect failed.");
	}

	/* Create thread that polls the status every second */
	pthread_create(&_periodic_query_thread, NULL, event_loop_thread, this);
}

SimConnectConnection::~SimConnectConnection() {
	if (_simconnect_handle) {
		SimConnect_Close(_simconnect_handle);
	}
}

