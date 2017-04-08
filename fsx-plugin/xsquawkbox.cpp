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

void XSquawkBoxConnection::event_loop() {
	_loop_running = true;

	while (_loop_running) {
	}
}

XSquawkBoxConnection::XSquawkBoxConnection() {
	_loop_running = false;
}

XSquawkBoxConnection::~XSquawkBoxConnection() {
}

PLUGIN_API int XPluginStart(char *outName, char *outSig, char *outDesc) {
	fprintf(stderr, "X-Plane plugin start.\n");
#if 0
	strcpy(outName, "flightpanel");
	strcpy(outSig, "flightpanel.main");
	strcpy(outDesc, "Connects X-Plane to the flightpanel hardware unit.");
#endif
        strcpy(outName, "HelloWorld");
        strcpy(outSig, "xplanesdk.examples.helloworld");
        strcpy(outDesc, "A plugin that makes a window.");

	return 1;
}

PLUGIN_API void XPluginStop(void) {
	//XPLMDestroyWindow(gWindow);
	fprintf(stderr, "X-Plane plugin stop.\n");
}

PLUGIN_API void XPluginDisable(void) {
	fprintf(stderr, "X-Plane plugin disable.\n");
}

PLUGIN_API int XPluginEnable(void) {
	fprintf(stderr, "X-Plane plugin enable.\n");
	return 1;
}

PLUGIN_API void XPluginReceiveMessage(XPLMPluginID inFromWho, long inMessage, void *inParam) {
	fprintf(stderr, "X-Plane receive msg.\n");
}

