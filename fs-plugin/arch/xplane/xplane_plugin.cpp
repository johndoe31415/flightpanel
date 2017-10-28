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
#include <string.h>
#include <XPLMPlugin.h>
#include "fpconnect.hpp"

PLUGIN_API int XPluginStart(char *outName, char *outSig, char *outDesc) {
	fprintf(stderr, "X-Plane plugin start.\n");
	strcpy(outName, "flightpanel");
	strcpy(outSig, "flightpanel.main");
	strcpy(outDesc, "Connects X-Plane to the flightpanel hardware unit.");
	return 1;
}

PLUGIN_API void XPluginStop(void) {
	fprintf(stderr, "X-Plane plugin stop.\n");
}

PLUGIN_API int XPluginEnable(void) {
	fprintf(stderr, "X-Plane plugin enable.\n");
	start_arbiter_thread();
	return 1;
}

PLUGIN_API void XPluginDisable(void) {
	fprintf(stderr, "X-Plane plugin disable.\n");
	stop_arbiter_thread();
}

PLUGIN_API void XPluginReceiveMessage(XPLMPluginID inFromWho, long inMessage, void *inParam) {
	fprintf(stderr, "X-Plane receive msg from %u: %ld (%p)\n", inFromWho, inMessage, inParam);
}
