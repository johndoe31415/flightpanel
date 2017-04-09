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
#include <XPLMPlugin.h>
#include <XPLMUtilities.h>

XSquawkBoxConnection *xsquawkbox_connection;

static void* event_loop_thread(void *ctx) {
	XSquawkBoxConnection *connection = (XSquawkBoxConnection*)ctx;
	while (connection->is_loop_running()) {
		connection->poll_data();
		sleep(1);
	}
	return NULL;
}

void XSquawkBoxConnection::event_loop() {
	_loop_running = true;
	pthread_create(&_periodic_query_thread, NULL, event_loop_thread, this);
}

static double inhg_to_millibar(double pressure_inhg) {
	return 33.8639 * pressure_inhg;
}

void XSquawkBoxConnection::poll_data() {
	struct instrument_data_t data;
	memset(&data, 0, sizeof(data));
	data.nav1.freq_active_khz = XPLMGetDatai(_datarefs.nav1.frequency_active) * 10;
	data.nav1.freq_standby_khz = XPLMGetDatai(_datarefs.nav1.frequency_standby) * 10;
	data.nav2.freq_active_khz = XPLMGetDatai(_datarefs.nav2.frequency_active) * 10;
	data.nav2.freq_standby_khz = XPLMGetDatai(_datarefs.nav2.frequency_standby) * 10;
	data.vhf1.freq_active_khz = XPLMGetDatai(_datarefs.vhf1.frequency_active) * 10;
	data.vhf1.freq_standby_khz = XPLMGetDatai(_datarefs.vhf1.frequency_standby) * 10;
	data.vhf2.freq_active_khz = XPLMGetDatai(_datarefs.vhf2.frequency_active) * 10;
	data.vhf2.freq_standby_khz = XPLMGetDatai(_datarefs.vhf2.frequency_standby) * 10;
	data.misc.qnh_millibar = round(inhg_to_millibar(XPLMGetDataf(_datarefs.misc.barometer_setting)));
	data.misc.indicated_alt_ft = XPLMGetDataf(_datarefs.misc.altitude);
	data.misc.ias_kt = XPLMGetDataf(_datarefs.misc.ias);
	fsconnection_incoming_data(&data);
}

XSquawkBoxConnection::XSquawkBoxConnection() {
	_loop_running = false;
	memset(&_datarefs, 0, sizeof(_datarefs));
	_datarefs.vhf1.frequency_active = XPLMFindDataRef("sim/cockpit/radios/com1_freq_hz");
	_datarefs.vhf1.frequency_standby = XPLMFindDataRef("sim/cockpit/radios/com1_stdby_freq_hz");
	_datarefs.vhf2.frequency_active = XPLMFindDataRef("sim/cockpit/radios/com2_freq_hz");
	_datarefs.vhf2.frequency_standby = XPLMFindDataRef("sim/cockpit/radios/com2_stdby_freq_hz");
	_datarefs.nav1.frequency_active = XPLMFindDataRef("sim/cockpit/radios/nav1_freq_hz");
	_datarefs.nav1.frequency_standby = XPLMFindDataRef("sim/cockpit/radios/nav1_stdby_freq_hz");
	_datarefs.nav2.frequency_active = XPLMFindDataRef("sim/cockpit/radios/nav2_freq_hz");
	_datarefs.nav2.frequency_standby = XPLMFindDataRef("sim/cockpit/radios/nav2_stdby_freq_hz");
	_datarefs.misc.barometer_setting = XPLMFindDataRef("sim/cockpit/misc/barometer_setting");
	_datarefs.misc.altitude = XPLMFindDataRef("sim/flightmodel/misc/h_ind");
	_datarefs.misc.ias = XPLMFindDataRef("sim/flightmodel/position/indicated_airspeed");
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
	//XPLMDestroyWindow(gWindow);
	fprintf(stderr, "X-Plane plugin stop.\n");
	xsquawkbox_connection->set_quit();
}

PLUGIN_API void XPluginDisable(void) {
	fprintf(stderr, "X-Plane plugin disable.\n");
}

PLUGIN_API int XPluginEnable(void) {
	fprintf(stderr, "X-Plane plugin enable.\n");
	return 1;
}

PLUGIN_API void XPluginReceiveMessage(XPLMPluginID inFromWho, long inMessage, void *inParam) {
	fprintf(stderr, "X-Plane receive msg from %u: %ld (%p)\n", inFromWho, inMessage, inParam);
}

