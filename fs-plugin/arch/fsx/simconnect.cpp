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
#include "logging.hpp"
#include "globals.hpp"
#include "bcd.hpp"

#if 0
static uint32_t frequency_bcd16_to_khz(uint32_t bcd16_value) {
	uint32_t khz = 100000 + (bcd_to_decimal(bcd16_value) * 10);
	if (((khz % 100) == 20) || ((khz % 100) == 70)) {
		khz += 5;
	}
	return khz;
}
#endif

static uint32_t callback_count;
static uint32_t callback_inhibit_until;

static uint32_t frequency_bcd32_to_khz(uint32_t bcd32_value) {
	return bcd_to_decimal(bcd32_value) / 10;
}

static uint32_t adf_frequency_bcd32_to_hz(uint32_t bcd32_value) {
	return bcd_to_decimal(bcd32_value) / 10;
}

static void simconnect_instrument_to_abstract(const struct simconnect_datatype_instruments_t &in, struct instrument_data_t *out) {
	memset(out, 0, sizeof(struct instrument_data_t));

	out->external.radio_panel |= in.com1_tx ? RADIO_COM1 : 0;
	out->external.radio_panel |= in.com2_tx ? RADIO_COM2 : 0;
	out->external.radio_panel |= in.com_all_rx ? (RADIO_COM1 | RADIO_COM2) : 0;
	out->external.radio_panel |= in.nav1_sound ? RADIO_NAV1 : 0;
	out->external.radio_panel |= in.nav2_sound ? RADIO_NAV2 : 0;
	out->external.radio_panel |= in.dme_sound ? RADIO_DME : 0;
	out->external.radio_panel |= in.adf_sound ? RADIO_ADF : 0;

	out->external.com_divisions = COM_RANGE;
	out->external.nav_divisions = NAV_RANGE;
	out->external.tx_radio_id = in.com1_tx ? 1 : 2;

	out->external.com1.freq.active_index = frequency_khz_to_index((enum com_nav_range_t)out->external.com_divisions, frequency_bcd32_to_khz(in.com1_freq_active));
	out->external.com1.freq.standby_index = frequency_khz_to_index((enum com_nav_range_t)out->external.com_divisions, frequency_bcd32_to_khz(in.com1_freq_standby));
	out->external.com2.freq.active_index = frequency_khz_to_index((enum com_nav_range_t)out->external.com_divisions, frequency_bcd32_to_khz(in.com2_freq_active));
	out->external.com2.freq.standby_index = frequency_khz_to_index((enum com_nav_range_t)out->external.com_divisions, frequency_bcd32_to_khz(in.com2_freq_standby));

	out->external.nav1.freq.active_index = frequency_khz_to_index((enum com_nav_range_t)out->external.nav_divisions, frequency_bcd32_to_khz(in.nav1_freq_active));
	out->external.nav1.freq.standby_index = frequency_khz_to_index((enum com_nav_range_t)out->external.nav_divisions, frequency_bcd32_to_khz(in.nav1_freq_standby));
	out->external.nav1.obs = round(in.nav1_obs_deg);
	out->external.nav2.freq.active_index = frequency_khz_to_index((enum com_nav_range_t)out->external.nav_divisions, frequency_bcd32_to_khz(in.nav2_freq_active));
	out->external.nav2.freq.standby_index = frequency_khz_to_index((enum com_nav_range_t)out->external.nav_divisions, frequency_bcd32_to_khz(in.nav2_freq_standby));
	out->external.nav2.obs = round(in.nav2_obs_deg);

	out->external.xpdr.state = XPDR_STANDBY;		// TODO: Get those from IvAp
	out->external.xpdr.squawk = bcd_to_decimal(in.xpdr_squawk);

	out->external.flip_switches |= (in.light_states & 0x0001) ? SWITCH_NAV : 0;
	out->external.flip_switches |= (in.light_states & 0x0002) ? SWITCH_BCN : 0;
	out->external.flip_switches |= (in.light_states & 0x0004) ? SWITCH_LAND : 0;
	out->external.flip_switches |= (in.light_states & 0x0008) ? SWITCH_TAXI : 0;
	out->external.flip_switches |= (in.light_states & 0x0010) ? SWITCH_STRB : 0;

	out->external.adf.frequency_khz = adf_frequency_bcd32_to_hz(in.adf_freq_active) / 1000;

	out->external.ap.state |= in.ap_master ? AP_STATE_ACTIVE : 0;
	out->external.ap.state |= in.ap_hdg_hold ? AP_HEADING_HOLD : 0;
	out->external.ap.state |= in.ap_nav_hold ? AP_NAVIGATION_HOLD : 0;
	out->external.ap.state |= in.ap_apr_hold ? AP_GLIDESLOPE_HOLD : 0;
	out->external.ap.state |= in.ap_rev_hold ? AP_STATE_BACKCOURSE : 0;
	out->external.ap.state |= in.ap_alt_hold ? AP_ALTITUDE_HOLD : 0;
	out->external.ap.state |= in.ap_ias_hold ? AP_IAS_HOLD : 0;
	out->external.ap.altitude = round(in.ap_altitude_ft);
	out->external.ap.heading = round(in.ap_heading_deg);
	out->external.ap.ias = 0;		// TODO
	out->external.ap.climbrate = round(in.ap_climbrate_ft_per_min);

	out->external.qnh = round(in.qnh_millibar);
	out->external.navigate_by_gps = in.gps_drives_nav;

//	out->ap.climbrate_ft_per_min = round(in->ap_climbrate_ft_per_min);
//	out->internal.dme.nav_id = in.dme_selected;


	out->internal.dme.available = in.dme_speed >= 0;
	if (out->internal.dme.available) {
		out->internal.dme.distance_tenth_nm = round(in.dme_distance * 10);
		out->internal.dme.velocity = round(in.dme_speed);
	}

	memcpy(out->internal.ident.nav1, in.nav1_ident, IDENT_LENGTH_BYTES);
	memcpy(out->internal.ident.nav2, in.nav2_ident, IDENT_LENGTH_BYTES);
	memcpy(out->internal.ident.adf, in.adf_ident, IDENT_LENGTH_BYTES);

	#if 0
	out->adf.compass_rose = round(in->adf_compass_rose_deg);
	out->misc.ias_kt = round(in->ias);
	out->misc.indicated_alt_ft = round(in->indicated_alt_ft);
	#endif
}

void SimConnectConnection::put_data(const struct instrument_data_t &data, const struct arbiter_elements_t &elements) {
	if (elements.radio_panel || elements.tx_radio_id) {

		if (data.external.radio_panel & (RADIO_COM1 | RADIO_COM2)) {
			/* Some COM is active */
			SimConnect_TransmitClientEvent(_simconnect_handle, SIMCONNECT_OBJECT_ID_USER, EVENT_COM1_TRANSMIT_SELECT, (data.external.tx_radio_id == 1) ? 1 : 0, SIMCONNECT_GROUP_PRIORITY_STANDARD, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
			SimConnect_TransmitClientEvent(_simconnect_handle, SIMCONNECT_OBJECT_ID_USER, EVENT_COM2_TRANSMIT_SELECT, (data.external.tx_radio_id == 2) ? 1 : 0, SIMCONNECT_GROUP_PRIORITY_STANDARD, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);

			/* Both radios active for listening */
			SimConnect_TransmitClientEvent(_simconnect_handle, SIMCONNECT_OBJECT_ID_USER, EVENT_COM_RECEIVE_ALL_SET, ((data.external.radio_panel & (RADIO_COM1 | RADIO_COM2)) == (RADIO_COM1 | RADIO_COM2)) ? 1 : 0, SIMCONNECT_GROUP_PRIORITY_STANDARD, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);

		} else {
			/* All radios off */
			SimConnect_TransmitClientEvent(_simconnect_handle, SIMCONNECT_OBJECT_ID_USER, EVENT_COM1_TRANSMIT_SELECT, 0, SIMCONNECT_GROUP_PRIORITY_STANDARD, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
			SimConnect_TransmitClientEvent(_simconnect_handle, SIMCONNECT_OBJECT_ID_USER, EVENT_COM2_TRANSMIT_SELECT, 0, SIMCONNECT_GROUP_PRIORITY_STANDARD, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
			SimConnect_TransmitClientEvent(_simconnect_handle, SIMCONNECT_OBJECT_ID_USER, EVENT_COM_RECEIVE_ALL_SET, 0, SIMCONNECT_GROUP_PRIORITY_STANDARD, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
		}

		SimConnect_TransmitClientEvent(_simconnect_handle, SIMCONNECT_OBJECT_ID_USER, EVENT_RADIO_VOR1_IDENT_SET, (data.external.radio_panel & RADIO_NAV1) ? 1 : 0, SIMCONNECT_GROUP_PRIORITY_STANDARD, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
		SimConnect_TransmitClientEvent(_simconnect_handle, SIMCONNECT_OBJECT_ID_USER, EVENT_RADIO_VOR2_IDENT_SET, (data.external.radio_panel & RADIO_NAV2) ? 1 : 0, SIMCONNECT_GROUP_PRIORITY_STANDARD, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
		SimConnect_TransmitClientEvent(_simconnect_handle, SIMCONNECT_OBJECT_ID_USER, EVENT_RADIO_ADF_IDENT_SET, (data.external.radio_panel & RADIO_ADF) ? 1 : 0, SIMCONNECT_GROUP_PRIORITY_STANDARD, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
		SimConnect_TransmitClientEvent(_simconnect_handle, SIMCONNECT_OBJECT_ID_USER, EVENT_RADIO_SELECTED_DME_IDENT_SET, (data.external.radio_panel & RADIO_DME) ? 1 : 0, SIMCONNECT_GROUP_PRIORITY_STANDARD, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);

	}
	if (elements.com1) {
		SimConnect_TransmitClientEvent(_simconnect_handle, SIMCONNECT_OBJECT_ID_USER, EVENT_COM_RADIO_SET, decimal_to_bcd(frequency_index_to_khz((enum com_nav_range_t)data.external.com_divisions, data.external.com1.freq.active_index) / 10), SIMCONNECT_GROUP_PRIORITY_STANDARD, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
		SimConnect_TransmitClientEvent(_simconnect_handle, SIMCONNECT_OBJECT_ID_USER, EVENT_COM_STBY_RADIO_SET, decimal_to_bcd(frequency_index_to_khz((enum com_nav_range_t)data.external.com_divisions, data.external.com1.freq.standby_index) / 10), SIMCONNECT_GROUP_PRIORITY_STANDARD, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
	}
	if (elements.com2) {
		SimConnect_TransmitClientEvent(_simconnect_handle, SIMCONNECT_OBJECT_ID_USER, EVENT_COM2_RADIO_SET, decimal_to_bcd(frequency_index_to_khz((enum com_nav_range_t)data.external.com_divisions, data.external.com2.freq.active_index) / 10), SIMCONNECT_GROUP_PRIORITY_STANDARD, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
		SimConnect_TransmitClientEvent(_simconnect_handle, SIMCONNECT_OBJECT_ID_USER, EVENT_COM2_STBY_RADIO_SET, decimal_to_bcd(frequency_index_to_khz((enum com_nav_range_t)data.external.com_divisions, data.external.com2.freq.standby_index) / 10), SIMCONNECT_GROUP_PRIORITY_STANDARD, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
	}
	if (elements.nav1) {
		SimConnect_TransmitClientEvent(_simconnect_handle, SIMCONNECT_OBJECT_ID_USER, EVENT_NAV1_RADIO_SET, decimal_to_bcd(frequency_index_to_khz((enum com_nav_range_t)data.external.nav_divisions, data.external.nav1.freq.active_index) / 10), SIMCONNECT_GROUP_PRIORITY_STANDARD, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
		SimConnect_TransmitClientEvent(_simconnect_handle, SIMCONNECT_OBJECT_ID_USER, EVENT_NAV1_STBY_SET, decimal_to_bcd(frequency_index_to_khz((enum com_nav_range_t)data.external.nav_divisions, data.external.nav1.freq.standby_index) / 10), SIMCONNECT_GROUP_PRIORITY_STANDARD, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
		SimConnect_TransmitClientEvent(_simconnect_handle, SIMCONNECT_OBJECT_ID_USER, EVENT_VOR1_SET, data.external.nav1.obs, SIMCONNECT_GROUP_PRIORITY_STANDARD, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
	}
	if (elements.nav2) {
		SimConnect_TransmitClientEvent(_simconnect_handle, SIMCONNECT_OBJECT_ID_USER, EVENT_NAV2_RADIO_SET, decimal_to_bcd(frequency_index_to_khz((enum com_nav_range_t)data.external.nav_divisions, data.external.nav2.freq.active_index) / 10), SIMCONNECT_GROUP_PRIORITY_STANDARD, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
		SimConnect_TransmitClientEvent(_simconnect_handle, SIMCONNECT_OBJECT_ID_USER, EVENT_NAV2_STBY_SET, decimal_to_bcd(frequency_index_to_khz((enum com_nav_range_t)data.external.nav_divisions, data.external.nav2.freq.standby_index) / 10), SIMCONNECT_GROUP_PRIORITY_STANDARD, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
		SimConnect_TransmitClientEvent(_simconnect_handle, SIMCONNECT_OBJECT_ID_USER, EVENT_VOR2_SET, data.external.nav2.obs, SIMCONNECT_GROUP_PRIORITY_STANDARD, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
	}
	if (elements.xpdr) {
		SimConnect_TransmitClientEvent(_simconnect_handle, SIMCONNECT_OBJECT_ID_USER, EVENT_XPNDR_SET, decimal_to_bcd(data.external.xpdr.squawk), SIMCONNECT_GROUP_PRIORITY_STANDARD, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
	}
	if (elements.adf) {
		SimConnect_TransmitClientEvent(_simconnect_handle, SIMCONNECT_OBJECT_ID_USER, EVENT_ADF_SET, decimal_to_bcd(data.external.adf.frequency_khz), SIMCONNECT_GROUP_PRIORITY_STANDARD, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
	}
	if (elements.ap) {
		uint16_t toggle;
		{
			LockGuard guard(_datalock);
			toggle = _instrument_data.external.ap.state ^ data.external.ap.state;
		}
		if (toggle & AP_STATE_ACTIVE) {
			SimConnect_TransmitClientEvent(_simconnect_handle, SIMCONNECT_OBJECT_ID_USER, (data.external.ap.state & AP_STATE_ACTIVE) ? EVENT_AUTOPILOT_ON : EVENT_AUTOPILOT_OFF, 1, SIMCONNECT_GROUP_PRIORITY_STANDARD, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
		}
		if (toggle & (AP_ALTITUDE_ARMED | AP_ALTITUDE_HOLD)) {
			SimConnect_TransmitClientEvent(_simconnect_handle, SIMCONNECT_OBJECT_ID_USER, (data.external.ap.state & (AP_ALTITUDE_ARMED | AP_ALTITUDE_HOLD)) ? EVENT_AP_ALT_HOLD_ON : EVENT_AP_ALT_HOLD_OFF, 1, SIMCONNECT_GROUP_PRIORITY_STANDARD, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
		}
		if (toggle & (AP_HEADING_ARMED | AP_HEADING_HOLD)) {
			SimConnect_TransmitClientEvent(_simconnect_handle, SIMCONNECT_OBJECT_ID_USER, (data.external.ap.state & (AP_HEADING_ARMED | AP_HEADING_HOLD)) ? EVENT_AP_HDG_HOLD_ON : EVENT_AP_HDG_HOLD_OFF, 1, SIMCONNECT_GROUP_PRIORITY_STANDARD, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
		}
		if (toggle & (AP_NAVIGATION_ARMED | AP_NAVIGATION_HOLD)) {
			SimConnect_TransmitClientEvent(_simconnect_handle, SIMCONNECT_OBJECT_ID_USER, (data.external.ap.state & (AP_NAVIGATION_ARMED | AP_NAVIGATION_HOLD)) ? EVENT_AP_NAV1_HOLD_ON : EVENT_AP_NAV1_HOLD_OFF, 1, SIMCONNECT_GROUP_PRIORITY_STANDARD, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
		}
		if (toggle & (AP_GLIDESLOPE_ARMED | AP_GLIDESLOPE_HOLD)) {
			SimConnect_TransmitClientEvent(_simconnect_handle, SIMCONNECT_OBJECT_ID_USER, (data.external.ap.state & (AP_GLIDESLOPE_ARMED | AP_GLIDESLOPE_HOLD)) ? EVENT_AP_APR_HOLD_ON : EVENT_AP_APR_HOLD_OFF, 1, SIMCONNECT_GROUP_PRIORITY_STANDARD, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
		}
		if (toggle & (AP_IAS_ARMED | AP_IAS_HOLD)) {
			SimConnect_TransmitClientEvent(_simconnect_handle, SIMCONNECT_OBJECT_ID_USER, (data.external.ap.state & (AP_IAS_ARMED | AP_IAS_HOLD)) ? EVENT_AP_AIRSPEED_ON : EVENT_AP_AIRSPEED_OFF, 1, SIMCONNECT_GROUP_PRIORITY_STANDARD, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
		}
		if (toggle & (AP_STATE_BACKCOURSE)) {
			SimConnect_TransmitClientEvent(_simconnect_handle, SIMCONNECT_OBJECT_ID_USER, (data.external.ap.state & AP_STATE_BACKCOURSE) ? EVENT_AP_BC_HOLD_ON : EVENT_AP_BC_HOLD_OFF, 1, SIMCONNECT_GROUP_PRIORITY_STANDARD, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
		}

		SimConnect_TransmitClientEvent(_simconnect_handle, SIMCONNECT_OBJECT_ID_USER, EVENT_HEADING_BUG_SET, data.external.ap.heading, SIMCONNECT_GROUP_PRIORITY_STANDARD, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
		SimConnect_TransmitClientEvent(_simconnect_handle, SIMCONNECT_OBJECT_ID_USER, EVENT_AP_VS_VAR_SET_ENGLISH, data.external.ap.climbrate, SIMCONNECT_GROUP_PRIORITY_STANDARD, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
		SimConnect_TransmitClientEvent(_simconnect_handle, SIMCONNECT_OBJECT_ID_USER, EVENT_AP_ALT_VAR_SET_ENGLISH, data.external.ap.altitude, SIMCONNECT_GROUP_PRIORITY_STANDARD, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
		SimConnect_TransmitClientEvent(_simconnect_handle, SIMCONNECT_OBJECT_ID_USER, EVENT_AP_SPD_VAR_SET, data.external.ap.ias, SIMCONNECT_GROUP_PRIORITY_STANDARD, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
	}
	if (elements.qnh) {
		SimConnect_TransmitClientEvent(_simconnect_handle, SIMCONNECT_OBJECT_ID_USER, EVENT_KOHLSMAN_SET, data.external.qnh * 16, SIMCONNECT_GROUP_PRIORITY_STANDARD, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
	}
	if (elements.navigate_by_gps) {
		SimConnect_TransmitClientEvent(_simconnect_handle, SIMCONNECT_OBJECT_ID_USER, EVENT_TOGGLE_GPS_DRIVES_NAV1, 1, SIMCONNECT_GROUP_PRIORITY_STANDARD, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
	}
	if (elements.flip_switches) {
		uint8_t toggle;
		{
			LockGuard guard(_datalock);
			toggle = _instrument_data.external.flip_switches ^ data.external.flip_switches;
		}
		if (toggle & SWITCH_BCN) {
			SimConnect_TransmitClientEvent(_simconnect_handle, SIMCONNECT_OBJECT_ID_USER, EVENT_TOGGLE_BEACON_LIGHTS, 1, SIMCONNECT_GROUP_PRIORITY_STANDARD, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
		}
		SimConnect_TransmitClientEvent(_simconnect_handle, SIMCONNECT_OBJECT_ID_USER, EVENT_LANDING_LIGHTS_SET, (data.external.flip_switches & SWITCH_LAND) ? 1 : 0, SIMCONNECT_GROUP_PRIORITY_STANDARD, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
		if (toggle & SWITCH_TAXI) {
			SimConnect_TransmitClientEvent(_simconnect_handle, SIMCONNECT_OBJECT_ID_USER, EVENT_TOGGLE_TAXI_LIGHTS, 1, SIMCONNECT_GROUP_PRIORITY_STANDARD, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
		}
		if (toggle & SWITCH_NAV) {
			SimConnect_TransmitClientEvent(_simconnect_handle, SIMCONNECT_OBJECT_ID_USER, EVENT_TOGGLE_NAV_LIGHTS, 1, SIMCONNECT_GROUP_PRIORITY_STANDARD, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
		}
		SimConnect_TransmitClientEvent(_simconnect_handle, SIMCONNECT_OBJECT_ID_USER, EVENT_STROBES_SET, (data.external.flip_switches & SWITCH_STRB) ? 1 : 0, SIMCONNECT_GROUP_PRIORITY_STANDARD, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
	}

	/* Ingore next SimConnect callbacks (which would provide old data). Ugly workaround, but not sure how to do this properly for now */
	callback_inhibit_until = callback_count + 2;
	_data_fresh.reset();
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
			logmsg(LLVL_DEBUG, "Simulation started.");
			SimConnect_RequestDataOnSimObjectType(_simconnect_handle, REQUEST_DATADEF_INFO, DATADEF_INFO, 0, SIMCONNECT_SIMOBJECT_TYPE_USER);
			SimConnect_RequestDataOnSimObjectType(_simconnect_handle, REQUEST_DATADEF_INSTRUMENTS, DATADEF_INSTRUMENTS, 0, SIMCONNECT_SIMOBJECT_TYPE_USER);
		} else if (evt->uGroupID == EVENTGROUP_INSTRUMENT_CHANGED) {
			//printf("Instrument changed: %s\n", get_event_enum_name(event_id));
			SimConnect_RequestDataOnSimObjectType(_simconnect_handle, REQUEST_DATADEF_INSTRUMENTS, DATADEF_INSTRUMENTS, 0, SIMCONNECT_SIMOBJECT_TYPE_USER);
		} else {
			logmsg(LLVL_WARNING, "Unhandled event received: 0x%lx", evt->uEventID);
		}
	} else if (pData->dwID == SIMCONNECT_RECV_ID_SIMOBJECT_DATA_BYTYPE) {
		SIMCONNECT_RECV_SIMOBJECT_DATA_BYTYPE *pObjData = (SIMCONNECT_RECV_SIMOBJECT_DATA_BYTYPE*)pData;
		DWORD object_id = pObjData->dwObjectID;
		if (pObjData->dwRequestID == REQUEST_DATADEF_INFO) {
			struct simconnect_datatype_information_t *fdata = (struct simconnect_datatype_information_t*)&pObjData->dwData;
			fdata->title[255] = 0;
			logmsg(LLVL_DEBUG, "Receive data about object %ld: %s", object_id, fdata->title);
		} else if (pObjData->dwRequestID == REQUEST_DATADEF_INSTRUMENTS) {
			callback_count++;
			if (callback_count < callback_inhibit_until) {
				logmsg(LLVL_DEBUG, "Inhibited SimConnect callback %d (until %d)", callback_count, callback_inhibit_until);
				return;
			}
			struct simconnect_datatype_instruments_t *simconnect_data = (struct simconnect_datatype_instruments_t*)&pObjData->dwData;
			if (SIMCONNECT_DATATYPE_INSTRUMENTS_DATUM_COUNT != pObjData->dwDefineCount) {
				logmsg(LLVL_WARNING, "Warning: expected %u datums, but received %lu.", SIMCONNECT_DATATYPE_INSTRUMENTS_DATUM_COUNT, pObjData->dwDefineCount);
			}

			struct instrument_data_t instrument_data;
			simconnect_instrument_to_abstract(*simconnect_data, &instrument_data);
			{
				LockGuard guard(_datalock);
				_instrument_data = instrument_data;
			}
			_data_fresh.set();

//			sleep_millis(1000);
		} else {
			logmsg(LLVL_WARNING, "Recevied unhandled data for request 0x%lx", pObjData->dwRequestID);
		}
	} else if (pData->dwID == SIMCONNECT_RECV_ID_QUIT) {
		stop();
		logmsg(LLVL_INFO, "Received termination signal.");
	} else {
		logmsg(LLVL_WARNING, "Unhandled callback dwID 0x%lx", pData->dwID);
	}
}

void SimConnectConnection::get_data(struct instrument_data_t *data) {
	LockGuard guard(_datalock);
	memcpy(data, &_instrument_data, sizeof(struct instrument_data_t));
}

void SimConnectConnection::thread_action() {
	//SimConnect_RequestDataOnSimObjectType(_simconnect_handle, REQUEST_DATADEF_INFO, DATADEF_INFO, 0, SIMCONNECT_SIMOBJECT_TYPE_USER);
	SimConnect_RequestDataOnSimObjectType(_simconnect_handle, REQUEST_DATADEF_INSTRUMENTS, DATADEF_INSTRUMENTS, 0, SIMCONNECT_SIMOBJECT_TYPE_USER);
	SimConnect_CallDispatch(_simconnect_handle, simconnect_callback_wrapper, this);
}

SimConnectConnection::SimConnectConnection() : Thread(FLIGHTSIM_THREAD_INTERVAL_MILLIS) {
	_simconnect_handle = NULL;

	if (SUCCEEDED(SimConnect_Open(&_simconnect_handle, "Flight Panel", NULL, 0, 0, 0))) {
		logmsg(LLVL_INFO, "Successfully connected to flight simulator via SimConnect.");
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

	memset(&_instrument_data, 0, sizeof(_instrument_data));

	/* Start thread that polls the status periodically */
	start();
}

SimConnectConnection::~SimConnectConnection() {
	if (_simconnect_handle) {
		SimConnect_Close(_simconnect_handle);
	}
}

