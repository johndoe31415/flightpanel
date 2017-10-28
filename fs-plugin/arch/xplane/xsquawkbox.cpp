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

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>
#include <stddef.h>
#include <unistd.h>
#include <string.h>
#include "fsconnection.hpp"
#include "xsquawkbox.hpp"
#include "xplane-datadefs.hpp"
#include "units.hpp"
#include <XPLMPlugin.h>
#include <XPLMUtilities.h>

enum XPAutopilotState {
	AutothrottleEngaged = (1 << 0),
	HeadingHoldEngaged = (1 << 1),
	WingLevelerEngaged = (1 << 2),
	AirspeedHoldWithPitchEngaged = (1 << 3),
	VVIClimbEngaged = (1 << 4),
	AltitudeHoldArm = (1 << 5),
	FlightLevelChangeEngaged = (1 << 6),
	PitchSyncEngaged = (1 << 7),
	HNAVArmed = (1 << 8),
	HNAVEngaged = (1 << 9),
	GlideslopeArmed = (1 << 10),
	GlideslopeEngaged = (1 << 11),
	FMSArmed = (1 << 12),
	FMSEnaged = (1 << 13),
	AltitudeHoldEngaged = (1 << 14),
	HorizontalTOGAEngaged = (1 << 15),
	VerticalTOGAEngaged = (1 << 16),
	VNAVArmed = (1 << 17),
	VNAVEngaged = (1 << 18),
};

static void dump_xplane_ap_state(int ap_state) {
	fprintf(stderr, "AP State %6d: ", ap_state);
	if (ap_state & AutothrottleEngaged) fprintf(stderr, "AutothrottleEngaged ");
	if (ap_state & HeadingHoldEngaged) fprintf(stderr, "HeadingHoldEngaged ");
	if (ap_state & WingLevelerEngaged) fprintf(stderr, "WingLevelerEngaged ");
	if (ap_state & AirspeedHoldWithPitchEngaged) fprintf(stderr, "AirspeedHoldWithPitchEngaged ");
	if (ap_state & VVIClimbEngaged) fprintf(stderr, "VVIClimbEngaged ");
	if (ap_state & AltitudeHoldArm) fprintf(stderr, "AltitudeHoldArm ");
	if (ap_state & FlightLevelChangeEngaged) fprintf(stderr, "FlightLevelChangeEngaged ");
	if (ap_state & PitchSyncEngaged) fprintf(stderr, "PitchSyncEngaged ");
	if (ap_state & HNAVArmed) fprintf(stderr, "HNAVArmed ");
	if (ap_state & HNAVEngaged) fprintf(stderr, "HNAVEngaged ");
	if (ap_state & GlideslopeArmed) fprintf(stderr, "GlideslopeArmed ");
	if (ap_state & GlideslopeEngaged) fprintf(stderr, "GlideslopeEngaged ");
	if (ap_state & FMSArmed) fprintf(stderr, "FMSArmed ");
	if (ap_state & FMSEnaged) fprintf(stderr, "FMSEnaged ");
	if (ap_state & AltitudeHoldEngaged) fprintf(stderr, "AltitudeHoldEngaged ");
	if (ap_state & HorizontalTOGAEngaged) fprintf(stderr, "HorizontalTOGAEngaged ");
	if (ap_state & VerticalTOGAEngaged) fprintf(stderr, "VerticalTOGAEngaged ");
	if (ap_state & VNAVArmed) fprintf(stderr, "VNAVArmed ");
	if (ap_state & VNAVEngaged) fprintf(stderr, "VNAVEngaged ");
	fprintf(stderr, "\n");
}

#if 0
	XPLMGetDatab(_datarefs.nav1.ident, _instrument_data.nav1.ident, 0, 8);
	_instrument_data.nav1.sound = XPLMGetDatai(_datarefs.nav1.sound);


	_instrument_data.adf.freq_hz = XPLMGetDatai(_datarefs.adf.frequency) * 1000;
	XPLMGetDatab(_datarefs.adf.ident, _instrument_data.adf.ident, 0, 8);
	_instrument_data.adf.compass_rose = round(XPLMGetDataf(_datarefs.adf.compass_rose));
	_instrument_data.adf.sound = XPLMGetDatai(_datarefs.adf.sound);

	_instrument_data.dme.available = XPLMGetDatai(_datarefs.dme.available);
	_instrument_data.dme.nav_id = 1;
	_instrument_data.dme.distance_nm_tenths = round(XPLMGetDataf(_datarefs.dme.distance) * 10);
	_instrument_data.dme.speed_kt = round(XPLMGetDataf(_datarefs.dme.speed));
	_instrument_data.dme.sound = XPLMGetDatai(_datarefs.dme.sound);



	_instrument_data.misc.ias_kt = XPLMGetDataf(_datarefs.misc.ias);
	_instrument_data.misc.indicated_alt_ft = XPLMGetDataf(_datarefs.misc.altitude);
#endif

XSquawkBoxConnection::XSquawkBoxConnection() : Thread(500) {
	memset(&_datarefs, 0, sizeof(_datarefs));
	_datarefs.com1.frequency_active = XPLMFindDataRef("sim/cockpit2/radios/actuators/com1_frequency_hz_833");
	_datarefs.com1.frequency_standby = XPLMFindDataRef("sim/cockpit2/radios/actuators/com1_standby_frequency_hz_833");
	_datarefs.com1.rx = XPLMFindDataRef("sim/cockpit2/radios/actuators/audio_selection_com1");

	_datarefs.com2.frequency_active = XPLMFindDataRef("sim/cockpit2/radios/actuators/com2_frequency_hz_833");
	_datarefs.com2.frequency_standby = XPLMFindDataRef("sim/cockpit2/radios/actuators/com2_standby_frequency_hz_833");
	_datarefs.com2.rx = XPLMFindDataRef("sim/cockpit2/radios/actuators/audio_selection_com2");

	_datarefs.nav1.frequency_active = XPLMFindDataRef("sim/cockpit/radios/nav1_freq_hz");
	_datarefs.nav1.frequency_standby = XPLMFindDataRef("sim/cockpit/radios/nav1_stdby_freq_hz");
	_datarefs.nav1.obs = XPLMFindDataRef("sim/cockpit/radios/nav1_obs_degm");
	_datarefs.nav1.ident = XPLMFindDataRef("sim/cockpit2/radios/indicators/nav1_nav_id");
	_datarefs.nav1.sound = XPLMFindDataRef("sim/cockpit2/radios/actuators/audio_selection_nav1");

	_datarefs.nav2.frequency_active = XPLMFindDataRef("sim/cockpit/radios/nav2_freq_hz");
	_datarefs.nav2.frequency_standby = XPLMFindDataRef("sim/cockpit/radios/nav2_stdby_freq_hz");
	_datarefs.nav2.obs = XPLMFindDataRef("sim/cockpit/radios/nav2_obs_degm");
	_datarefs.nav2.ident = XPLMFindDataRef("sim/cockpit2/radios/indicators/nav2_nav_id");
	_datarefs.nav2.sound = XPLMFindDataRef("sim/cockpit2/radios/actuators/audio_selection_nav2");

	_datarefs.adf.frequency = XPLMFindDataRef("sim/cockpit/radios/adf1_freq_hz");
	_datarefs.adf.compass_rose = XPLMFindDataRef("sim/cockpit/radios/adf1_cardinal_dir");
	_datarefs.adf.ident = XPLMFindDataRef("sim/cockpit2/radios/indicators/adf1_nav_id");
	_datarefs.adf.sound = XPLMFindDataRef("sim/cockpit2/radios/actuators/audio_selection_adf1");

	_datarefs.dme.sound = XPLMFindDataRef("sim/cockpit2/radios/actuators/audio_dme_enabled");
	_datarefs.dme.distance = XPLMFindDataRef("sim/cockpit2/radios/indicators/nav1_dme_distance_nm");
	_datarefs.dme.speed = XPLMFindDataRef("sim/cockpit2/radios/indicators/nav1_dme_speed_kts");
	_datarefs.dme.available = XPLMFindDataRef("sim/cockpit2/radios/indicators/nav1_has_dme");

	_datarefs.xpdr.mode = XPLMFindDataRef("sim/cockpit/radios/transponder_mode");
	_datarefs.xpdr.squawk = XPLMFindDataRef("sim/cockpit/radios/transponder_code");

	_datarefs.ap.active = XPLMFindDataRef("sim/cockpit/autopilot/autopilot_mode");	// 1 or 2
	_datarefs.ap.heading = XPLMFindDataRef("sim/cockpit/autopilot/heading_mag");
	_datarefs.ap.climbrate = XPLMFindDataRef("sim/cockpit2/autopilot/vvi_dial_fpm");
	_datarefs.ap.arm_altitude = XPLMFindDataRef("sim/cockpit2/autopilot/altitude_dial_ft");
	_datarefs.ap.altitude = XPLMFindDataRef("sim/cockpit/autopilot/current_altitude");
	_datarefs.ap.airspeed = XPLMFindDataRef("sim/cockpit2/autopilot/airspeed_dial_kts");
	_datarefs.ap.state = XPLMFindDataRef("sim/cockpit/autopilot/autopilot_state");
	_datarefs.ap.backcourse = XPLMFindDataRef("sim/cockpit/autopilot/backcourse_on");

	_datarefs.lights.beacon = XPLMFindDataRef("sim/cockpit/electrical/beacon_lights_on");
	_datarefs.lights.landing = XPLMFindDataRef("sim/cockpit/electrical/landing_lights_on");
	_datarefs.lights.taxi = XPLMFindDataRef("sim/cockpit/electrical/taxi_light_on");
	_datarefs.lights.nav = XPLMFindDataRef("sim/cockpit/electrical/nav_lights_on");
	_datarefs.lights.strobe = XPLMFindDataRef("sim/cockpit/electrical/strobe_lights_on");

	_datarefs.misc.barometer_setting = XPLMFindDataRef("sim/cockpit/misc/barometer_setting");
	_datarefs.misc.altitude = XPLMFindDataRef("sim/flightmodel/misc/h_ind");
	_datarefs.misc.ias = XPLMFindDataRef("sim/flightmodel/position/indicated_airspeed");
	_datarefs.misc.hsi_selector = XPLMFindDataRef("sim/cockpit/switches/HSI_selector");

	memset(&_instrument_data, 0, sizeof(struct instrument_data_t));
	start();
}

void XSquawkBoxConnection::get_data(struct instrument_data_t *data) {
	LockGuard guard(_datalock);
	*data = _instrument_data;
}

static int x2f_xpdr_mode(int xplane_value) {
	switch (xplane_value) {
		case 0: return XPDR_OFF;
		case 1: return XPDR_STANDBY;
		case 2: return XPDR_CHARLY;
		default:
			fprintf(stderr, "Unknown X-Plane XPDR value %d encountered.\n", xplane_value);
			return XPDR_OFF;
	}
}

static int f2x_xpdr_mode(int flightpanel_value) {
	switch (flightpanel_value) {
		case XPDR_OFF: return 0;
		case XPDR_STANDBY: return 1;
		case XPDR_CHARLY: return 2;
		default:
			fprintf(stderr, "Unknown flightpanel XPDR value %d encountered.\n", flightpanel_value);
			return 0;
	}
}

void XSquawkBoxConnection::thread_action() {
	LockGuard guard(_datalock);

	_instrument_data.external.radio_panel |= XPLMGetDatai(_datarefs.com1.rx) ? RADIO_COM1 : 0;
	_instrument_data.external.radio_panel |= XPLMGetDatai(_datarefs.com2.rx) ? RADIO_COM2 : 0;
	_instrument_data.external.radio_panel |= XPLMGetDatai(_datarefs.nav1.sound) ? RADIO_NAV1 : 0;
	_instrument_data.external.radio_panel |= XPLMGetDatai(_datarefs.nav2.sound) ? RADIO_NAV2 : 0;
	_instrument_data.external.radio_panel |= XPLMGetDatai(_datarefs.dme.sound) ? RADIO_DME : 0;
	_instrument_data.external.radio_panel |= XPLMGetDatai(_datarefs.adf.sound) ? RADIO_ADF : 0;

	_instrument_data.external.com_divisions = COM_RANGE;
	_instrument_data.external.nav_divisions = NAV_RANGE;

	_instrument_data.external.com1.freq.active_index = frequency_khz_to_index((enum com_nav_range_t)_instrument_data.external.com_divisions, XPLMGetDatai(_datarefs.com1.frequency_active));
	_instrument_data.external.com1.freq.standby_index = frequency_khz_to_index((enum com_nav_range_t)_instrument_data.external.com_divisions, XPLMGetDatai(_datarefs.com1.frequency_standby));
	_instrument_data.external.com2.freq.active_index = frequency_khz_to_index((enum com_nav_range_t)_instrument_data.external.com_divisions, XPLMGetDatai(_datarefs.com2.frequency_active));
	_instrument_data.external.com2.freq.standby_index = frequency_khz_to_index((enum com_nav_range_t)_instrument_data.external.com_divisions, XPLMGetDatai(_datarefs.com2.frequency_standby));

	_instrument_data.external.nav1.freq.active_index = frequency_khz_to_index((enum com_nav_range_t)_instrument_data.external.nav_divisions, XPLMGetDatai(_datarefs.nav1.frequency_active) * 10);
	_instrument_data.external.nav1.freq.standby_index = frequency_khz_to_index((enum com_nav_range_t)_instrument_data.external.nav_divisions, XPLMGetDatai(_datarefs.nav1.frequency_standby) * 10);
	_instrument_data.external.nav1.obs = round(XPLMGetDataf(_datarefs.nav1.obs));
	_instrument_data.external.nav2.freq.active_index = frequency_khz_to_index((enum com_nav_range_t)_instrument_data.external.nav_divisions, XPLMGetDatai(_datarefs.nav2.frequency_active) * 10);
	_instrument_data.external.nav2.freq.standby_index = frequency_khz_to_index((enum com_nav_range_t)_instrument_data.external.nav_divisions, XPLMGetDatai(_datarefs.nav2.frequency_standby) * 10);
	_instrument_data.external.nav2.obs = round(XPLMGetDataf(_datarefs.nav2.obs));

	_instrument_data.external.xpdr.state = x2f_xpdr_mode(XPLMGetDatai(_datarefs.xpdr.mode));
	_instrument_data.external.xpdr.squawk = XPLMGetDatai(_datarefs.xpdr.squawk);

	_instrument_data.external.adf.frequency_khz = XPLMGetDatai(_datarefs.adf.frequency);

	int xplane_ap_state = XPLMGetDatai(_datarefs.ap.state);
	int xplane_ap_active = XPLMGetDatai(_datarefs.ap.active);
	if ((xplane_ap_active != 1) && (xplane_ap_active != 2)) {
		fprintf(stderr, "Warning: Unrecognize AP activity value %d\n", xplane_ap_active);
	}
	dump_xplane_ap_state(xplane_ap_state);
	_instrument_data.external.ap.state = 0;
	_instrument_data.external.ap.state |= (xplane_ap_active == 2) ? AP_ACTIVE : 0;
	_instrument_data.external.ap.state |= ((xplane_ap_state & AltitudeHoldEngaged) || (xplane_ap_state & VVIClimbEngaged)) ? AP_HOLD_ALTITUDE : 0;
	_instrument_data.external.ap.state |= (xplane_ap_state & HeadingHoldEngaged) ? AP_HOLD_HEADING : 0;
	_instrument_data.external.ap.state |= (xplane_ap_state & AutothrottleEngaged) ? AP_HOLD_IAS : 0;
	_instrument_data.external.ap.state |= ((xplane_ap_state & HNAVArmed) || (xplane_ap_state & HNAVEngaged)) ? AP_HOLD_NAVIGATION : 0;
	_instrument_data.external.ap.state |= ((xplane_ap_state & GlideslopeArmed) || (xplane_ap_state & GlideslopeEngaged)) ? AP_HOLD_APPROACH : 0;
	_instrument_data.external.ap.state |= XPLMGetDatai(_datarefs.ap.backcourse) ? AP_HOLD_REVERSE : 0;

	_instrument_data.external.ap.altitude = round(XPLMGetDataf(_datarefs.ap.arm_altitude));
	_instrument_data.external.ap.heading = round(XPLMGetDataf(_datarefs.ap.heading));
	_instrument_data.external.ap.ias = round(XPLMGetDataf(_datarefs.ap.airspeed));
	_instrument_data.external.ap.climbrate = fabs(round(XPLMGetDataf(_datarefs.ap.climbrate)));

	_instrument_data.external.qnh = round(inhg_to_millibar(XPLMGetDataf(_datarefs.misc.barometer_setting)));
	_instrument_data.external.navigate_by_gps = XPLMGetDatai(_datarefs.misc.hsi_selector) == 2; 		// 0 = NAV, 2 = GPS (HSI_selector_2: 1 = NAV, 3 = GPS)

	_instrument_data.internal.dme.available = XPLMGetDatai(_datarefs.dme.available);
	_instrument_data.internal.dme.distance_tenth_nm = round(XPLMGetDataf(_datarefs.dme.distance) * 10);
	_instrument_data.internal.dme.velocity = round(XPLMGetDataf(_datarefs.dme.speed));
	_instrument_data.internal.ap.ignore_values = IGNORE_CLIMBRATE | IGNORE_IAS;

	XPLMGetDatab(_datarefs.nav1.ident, _instrument_data.internal.ident.nav1, 0, IDENT_LENGTH_BYTES);
	XPLMGetDatab(_datarefs.nav2.ident, _instrument_data.internal.ident.nav2, 0, IDENT_LENGTH_BYTES);
	XPLMGetDatab(_datarefs.adf.ident, _instrument_data.internal.ident.adf, 0, IDENT_LENGTH_BYTES);
//	fprintf(stderr, "NAV1 '%s'\n", _instrument_data.internal.ident.nav1);
	_data_fresh.set();
}

void XSquawkBoxConnection::xplane_deactivate_autopilot() {
	int current_ap_state = XPLMGetDatai(_datarefs.ap.state);
	if (current_ap_state & AutothrottleEngaged) {
		XPLMSetDatai(_datarefs.ap.state, AutothrottleEngaged);
	}
	if (current_ap_state & AltitudeHoldEngaged) {
		XPLMSetDatai(_datarefs.ap.state, AltitudeHoldEngaged);
	}
	if (current_ap_state & VVIClimbEngaged) {
		XPLMSetDatai(_datarefs.ap.state, VVIClimbEngaged);
	}
	if (current_ap_state & HeadingHoldEngaged) {
		XPLMSetDatai(_datarefs.ap.state, HeadingHoldEngaged);
	}
	if (current_ap_state & HNAVArmed) {
		XPLMSetDatai(_datarefs.ap.state, HNAVArmed);
	}
	if (current_ap_state & HNAVEngaged) {
		XPLMSetDatai(_datarefs.ap.state, HNAVEngaged);
	}
	if (current_ap_state & GlideslopeEngaged) {
		XPLMSetDatai(_datarefs.ap.state, GlideslopeEngaged);
	}
	if (current_ap_state & GlideslopeArmed) {
		XPLMSetDatai(_datarefs.ap.state, GlideslopeArmed);
	}
}

void XSquawkBoxConnection::xplane_set_autopilot(const struct ap_state_t &ap_state) {
	xplane_deactivate_autopilot();

	double climbrate = ap_state.climbrate;
	if (ap_state.state & AP_HOLD_ALTITUDE) {
		double current_altitude = XPLMGetDataf(_datarefs.misc.altitude);
		double height_difference = ap_state.altitude - current_altitude;
		if ((height_difference > -100) && (height_difference < 100)) {
			/* Small height difference, go directly into ALT hold mode */
			XPLMSetDatai(_datarefs.ap.state, AltitudeHoldEngaged);
			XPLMSetDataf(_datarefs.ap.altitude, ap_state.altitude);
		} else {
			/* Larger height difference. Use VS mode and capture altitude
			 * automatically */
			XPLMSetDatai(_datarefs.ap.state, VVIClimbEngaged);
			if (height_difference < 0) {
				/* Sink */
				climbrate = -climbrate;
			}
		}
	}
	if (ap_state.state & AP_HOLD_IAS) {
		XPLMSetDatai(_datarefs.ap.state, AutothrottleEngaged);
	}
	if (ap_state.state & AP_HOLD_HEADING) {
		XPLMSetDatai(_datarefs.ap.state, HeadingHoldEngaged);
	} else if (ap_state.state & AP_HOLD_NAVIGATION) {
		XPLMSetDatai(_datarefs.ap.state, HNAVArmed);
	}
	if (ap_state.state & AP_HOLD_APPROACH) {
		XPLMSetDatai(_datarefs.ap.state, GlideslopeArmed);
	}

	XPLMSetDataf(_datarefs.ap.arm_altitude, ap_state.altitude);
	XPLMSetDataf(_datarefs.ap.climbrate, climbrate);
	XPLMSetDataf(_datarefs.ap.airspeed, ap_state.ias);
	XPLMSetDataf(_datarefs.ap.heading, ap_state.heading);

	/* AP master */
	XPLMSetDatai(_datarefs.ap.active, (ap_state.state & AP_ACTIVE) ? 2 : 1);
}

void XSquawkBoxConnection::put_data(const struct instrument_data_t &data, const struct arbiter_elements_t &elements) {
	LockGuard guard(_datalock);

	if (elements.radio_panel) {
		XPLMSetDatai(_datarefs.com1.rx, data.external.radio_panel & RADIO_COM1 ? 1 : 0);
		XPLMSetDatai(_datarefs.com2.rx, data.external.radio_panel & RADIO_COM2 ? 1 : 0);
		XPLMSetDatai(_datarefs.nav1.sound, data.external.radio_panel & RADIO_NAV1 ? 1 : 0);
		XPLMSetDatai(_datarefs.nav2.sound, data.external.radio_panel & RADIO_NAV2 ? 1 : 0);
		XPLMSetDatai(_datarefs.dme.sound, data.external.radio_panel & RADIO_DME ? 1 : 0);
		XPLMSetDatai(_datarefs.adf.sound, data.external.radio_panel & RADIO_ADF ? 1 : 0);
	}
	if (elements.com1) {
		XPLMSetDatai(_datarefs.com1.frequency_active, frequency_index_to_khz((enum com_nav_range_t)data.external.com_divisions, data.external.com1.freq.active_index));
		XPLMSetDatai(_datarefs.com1.frequency_standby, frequency_index_to_khz((enum com_nav_range_t)data.external.com_divisions, data.external.com1.freq.standby_index));
	}
	if (elements.com2) {
		XPLMSetDatai(_datarefs.com2.frequency_active, frequency_index_to_khz((enum com_nav_range_t)data.external.com_divisions, data.external.com2.freq.active_index));
		XPLMSetDatai(_datarefs.com2.frequency_standby, frequency_index_to_khz((enum com_nav_range_t)data.external.com_divisions, data.external.com2.freq.standby_index));
	}
	if (elements.nav1) {
		XPLMSetDatai(_datarefs.nav1.frequency_active, frequency_index_to_khz((enum com_nav_range_t)data.external.nav_divisions, data.external.nav1.freq.active_index) / 10);
		XPLMSetDatai(_datarefs.nav1.frequency_standby, frequency_index_to_khz((enum com_nav_range_t)data.external.nav_divisions, data.external.nav1.freq.standby_index) / 10);
		XPLMSetDataf(_datarefs.nav1.obs, data.external.nav1.obs);
	}
	if (elements.nav2) {
		XPLMSetDatai(_datarefs.nav2.frequency_active, frequency_index_to_khz((enum com_nav_range_t)data.external.nav_divisions, data.external.nav2.freq.active_index) / 10);
		XPLMSetDatai(_datarefs.nav2.frequency_standby, frequency_index_to_khz((enum com_nav_range_t)data.external.nav_divisions, data.external.nav2.freq.standby_index) / 10);
		XPLMSetDataf(_datarefs.nav2.obs, data.external.nav2.obs);
	}
	if (elements.xpdr) {
		XPLMSetDatai(_datarefs.xpdr.mode, f2x_xpdr_mode(data.external.xpdr.state));
		XPLMSetDatai(_datarefs.xpdr.squawk, data.external.xpdr.squawk);
	}
	if (elements.adf) {
		XPLMSetDatai(_datarefs.adf.frequency, data.external.adf.frequency_khz);
	}
	if (elements.ap) {
		xplane_set_autopilot(data.external.ap);
	}
	if (elements.qnh) {
		XPLMSetDataf(_datarefs.misc.barometer_setting, millibar_to_inhg(data.external.qnh));
	}
	if (elements.navigate_by_gps) {
		XPLMSetDatai(_datarefs.misc.hsi_selector, data.external.navigate_by_gps ? 2 : 0);
	}
	if (elements.flip_switches) {
		XPLMSetDatai(_datarefs.lights.beacon, !!(data.external.flip_switches & SWITCH_BCN));
		XPLMSetDatai(_datarefs.lights.landing, !!(data.external.flip_switches & SWITCH_LAND));
		XPLMSetDatai(_datarefs.lights.taxi, !!(data.external.flip_switches & SWITCH_TAXI));
		XPLMSetDatai(_datarefs.lights.nav, !!(data.external.flip_switches & SWITCH_NAV));
		XPLMSetDatai(_datarefs.lights.strobe, !!(data.external.flip_switches & SWITCH_STRB));
	}
	_instrument_data = data;
	_data_fresh.reset();
}

XSquawkBoxConnection::~XSquawkBoxConnection() {
	stop();
}

