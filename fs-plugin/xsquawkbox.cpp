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
#include <XPLMPlugin.h>
#include <XPLMUtilities.h>

XSquawkBoxConnection *xsquawkbox_connection;

static void* event_loop_thread(void *ctx) {
	XSquawkBoxConnection *connection = (XSquawkBoxConnection*)ctx;
	connection->event_loop();
	return NULL;
}

void XSquawkBoxConnection::event_loop() {
	xplane_datadef_refs_t refs;
	xplane_datadef_data_t old_data;
	memset(&refs, 0, sizeof(refs));
	memset(&old_data, 0, sizeof(old_data));
	xplane_resolve_refs(refs);
	while (_loop_running) {
#if 0
		xplane_datadef_data_t new_data;
		memset(&new_data, 0, sizeof(new_data));
		xplane_get_data(refs, new_data);
		xplane_print_new(old_data, new_data);
		memcpy(&old_data, &new_data, sizeof(old_data));
#else
		poll_data();
#endif
		sleep(1);
	}
}

static double inhg_to_millibar(double pressure_inhg) {
	return 33.8639 * pressure_inhg;
}

void XSquawkBoxConnection::poll_data() {
#if 0
	_instrument_data.com1.freq_active_khz = XPLMGetDatai(_datarefs.com1.frequency_active) * 10;
	_instrument_data.com1.freq_standby_khz = XPLMGetDatai(_datarefs.com1.frequency_standby) * 10;
	_instrument_data.com1.rx = XPLMGetDatai(_datarefs.com1.rx);

	_instrument_data.com2.freq_active_khz = XPLMGetDatai(_datarefs.com2.frequency_active) * 10;
	_instrument_data.com2.freq_standby_khz = XPLMGetDatai(_datarefs.com2.frequency_standby) * 10;
	_instrument_data.com2.rx = XPLMGetDatai(_datarefs.com2.rx);

	if (_instrument_data.com1.rx) {
		_instrument_data.com1.tx = true;
		_instrument_data.com2.tx = false;
	} else {
		_instrument_data.com1.tx = false;
		_instrument_data.com2.tx = _instrument_data.com2.rx;
	}

	_instrument_data.nav1.freq_active_khz = XPLMGetDatai(_datarefs.nav1.frequency_active) * 10;
	_instrument_data.nav1.freq_standby_khz = XPLMGetDatai(_datarefs.nav1.frequency_standby) * 10;
	_instrument_data.nav1.obs = round(XPLMGetDataf(_datarefs.nav1.obs));
	XPLMGetDatab(_datarefs.nav1.ident, _instrument_data.nav1.ident, 0, 8);
	_instrument_data.nav1.sound = XPLMGetDatai(_datarefs.nav1.sound);

	_instrument_data.nav2.freq_active_khz = XPLMGetDatai(_datarefs.nav2.frequency_active) * 10;
	_instrument_data.nav2.freq_standby_khz = XPLMGetDatai(_datarefs.nav2.frequency_standby) * 10;
	_instrument_data.nav2.obs = round(XPLMGetDataf(_datarefs.nav2.obs));
	XPLMGetDatab(_datarefs.nav2.ident, _instrument_data.nav2.ident, 0, 8);
	_instrument_data.nav2.sound = XPLMGetDatai(_datarefs.nav2.sound);

	_instrument_data.adf.freq_hz = XPLMGetDatai(_datarefs.adf.frequency) * 1000;
	XPLMGetDatab(_datarefs.adf.ident, _instrument_data.adf.ident, 0, 8);
	_instrument_data.adf.compass_rose = round(XPLMGetDataf(_datarefs.adf.compass_rose));
	_instrument_data.adf.sound = XPLMGetDatai(_datarefs.adf.sound);

	_instrument_data.dme.available = XPLMGetDatai(_datarefs.dme.available);
	_instrument_data.dme.nav_id = 1;
	_instrument_data.dme.distance_nm_tenths = round(XPLMGetDataf(_datarefs.dme.distance) * 10);
	_instrument_data.dme.speed_kt = round(XPLMGetDataf(_datarefs.dme.speed));
	_instrument_data.dme.sound = XPLMGetDatai(_datarefs.dme.sound);

	_instrument_data.lights.beacon = XPLMGetDatai(_datarefs.lights.beacon);
	_instrument_data.lights.landing = XPLMGetDatai(_datarefs.lights.landing);
	_instrument_data.lights.taxi = XPLMGetDatai(_datarefs.lights.taxi);
	_instrument_data.lights.nav = XPLMGetDatai(_datarefs.lights.nav);
	_instrument_data.lights.strobe = XPLMGetDatai(_datarefs.lights.strobe);

	_instrument_data.ap.active = XPLMGetDatai(_datarefs.ap.active) >= 2;
	_instrument_data.ap.altitude_ft = round(XPLMGetDataf(_datarefs.ap.altitude));
	_instrument_data.ap.climbrate_ft_per_min = round(XPLMGetDataf(_datarefs.ap.climbrate));
	_instrument_data.ap.heading = round(XPLMGetDataf(_datarefs.ap.heading));
	_instrument_data.ap.speed_kts = round(XPLMGetDataf(_datarefs.ap.airspeed));
	_instrument_data.ap.hdg_hold = XPLMGetDatai(_datarefs.ap.hdg_hold);
	_instrument_data.ap.nav_hold = XPLMGetDatai(_datarefs.ap.nav_hold);
	_instrument_data.ap.alt_hold = XPLMGetDatai(_datarefs.ap.alt_hold) == 2;		// Alt Mode captured (0 = off, 1 = armed)
	_instrument_data.ap.rev_hold = XPLMGetDatai(_datarefs.ap.rev_hold);
	_instrument_data.ap.apr_hold = XPLMGetDatai(_datarefs.ap.apr_hold);
	_instrument_data.ap.ias_hold = XPLMGetDatai(_datarefs.ap.ias_hold) == 2;		// Speed Mode captured (0 = off, 1 = armed)
	_instrument_data.xpdr.squawk = XPLMGetDatai(_datarefs.xpdr.squawk);

	_instrument_data.misc.ias_kt = XPLMGetDataf(_datarefs.misc.ias);
	_instrument_data.misc.indicated_alt_ft = XPLMGetDataf(_datarefs.misc.altitude);
	_instrument_data.misc.qnh_millibar = round(inhg_to_millibar(XPLMGetDataf(_datarefs.misc.barometer_setting)));
	_instrument_data.misc.guide_gps = XPLMGetDatai(_datarefs.misc.hsi_selector) == 2; 		// 0 = NAV, 2 = GPS (HSI_selector_2: 1 = NAV, 3 = GPS)
#endif
}

XSquawkBoxConnection::XSquawkBoxConnection() {
	_loop_running = false;
	memset(&_datarefs, 0, sizeof(_datarefs));
	_datarefs.com1.frequency_active = XPLMFindDataRef("sim/cockpit/radios/com1_freq_hz");
	_datarefs.com1.frequency_standby = XPLMFindDataRef("sim/cockpit/radios/com1_stdby_freq_hz");
	_datarefs.com1.rx = XPLMFindDataRef("sim/cockpit2/radios/actuators/audio_selection_com1");

	_datarefs.com2.frequency_active = XPLMFindDataRef("sim/cockpit/radios/com2_freq_hz");
	_datarefs.com2.frequency_standby = XPLMFindDataRef("sim/cockpit/radios/com2_stdby_freq_hz");
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
	_datarefs.dme.distance = XPLMFindDataRef("sim/cockpit/radios/nav1_dme_dist_m");
	_datarefs.dme.speed = XPLMFindDataRef("sim/cockpit/radios/nav1_dme_speed_kts");
	_datarefs.dme.available = XPLMFindDataRef("sim/cockpit2/radios/indicators/nav1_has_dme");

	_datarefs.xpdr.squawk = XPLMFindDataRef("sim/cockpit/radios/transponder_code");

	_datarefs.ap.active = XPLMFindDataRef("sim/cockpit/autopilot/autopilot_mode");	// 1 or 2
	_datarefs.ap.heading = XPLMFindDataRef("sim/cockpit/autopilot/heading_mag");
	_datarefs.ap.climbrate = XPLMFindDataRef("sim/cockpit/autopilot/vertical_velocity");	// or sim/cockpit2/autopilot/vvi_dial_fpm?
	_datarefs.ap.altitude = XPLMFindDataRef("sim/cockpit2/autopilot/altitude_dial_ft");
	_datarefs.ap.airspeed = XPLMFindDataRef("sim/cockpit2/autopilot/airspeed_dial_kts");
	_datarefs.ap.hdg_hold = XPLMFindDataRef("sim/cockpit2/autopilot/heading_mode");	// 0 or 1
	_datarefs.ap.nav_hold = XPLMFindDataRef("sim/cockpit2/autopilot/nav_status");
	_datarefs.ap.apr_hold = XPLMFindDataRef("sim/cockpit2/autopilot/approach_status");
	_datarefs.ap.rev_hold = XPLMFindDataRef("sim/cockpit2/autopilot/backcourse_status");
	_datarefs.ap.alt_hold = XPLMFindDataRef("sim/cockpit2/autopilot/altitude_hold_status");
	_datarefs.ap.ias_hold = XPLMFindDataRef("sim/cockpit2/autopilot/speed_status");

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
	pthread_create(&_periodic_query_thread, NULL, event_loop_thread, this);
}

XSquawkBoxConnection::~XSquawkBoxConnection() {
}

PLUGIN_API int XPluginStart(char *outName, char *outSig, char *outDesc) {
	fprintf(stderr, "X-Plane plugin start.\n");
	strcpy(outName, "flightpanel");
	strcpy(outSig, "flightpanel.main");
	strcpy(outDesc, "Connects X-Plane to the flightpanel hardware unit.");
	xsquawkbox_connection = new XSquawkBoxConnection();
	xsquawkbox_connection->event_loop();
	return 1;
}

PLUGIN_API void XPluginStop(void) {
	fprintf(stderr, "X-Plane plugin stop.\n");
//	xsquawkbox_connection->set_quit();
}

PLUGIN_API int XPluginEnable(void) {
	fprintf(stderr, "X-Plane plugin enable.\n");
	return 1;
}

PLUGIN_API void XPluginDisable(void) {
	fprintf(stderr, "X-Plane plugin disable.\n");
//	xsquawkbox_connection->set_quit();
}

PLUGIN_API void XPluginReceiveMessage(XPLMPluginID inFromWho, long inMessage, void *inParam) {
	fprintf(stderr, "X-Plane receive msg from %u: %ld (%p)\n", inFromWho, inMessage, inParam);
}

