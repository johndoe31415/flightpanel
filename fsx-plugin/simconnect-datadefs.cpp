#include <stdio.h>
#include <stdint.h>
#include <windows.h>
#include "SimConnect.h"
#include "simconnect-datadefs.hpp"

void simconnect_datadefs_register_information(void *hSimConnect, int group_id) {
	SimConnect_AddToDataDefinition(hSimConnect, group_id, "TITLE", NULL, SIMCONNECT_DATATYPE_STRING256);
}

void simconnect_datadefs_register_instruments(void *hSimConnect, int group_id) {
	SimConnect_AddToDataDefinition(hSimConnect, group_id, "NAV IDENT:1", NULL, SIMCONNECT_DATATYPE_STRING8);
	SimConnect_AddToDataDefinition(hSimConnect, group_id, "NAV OBS:1", "Degrees", SIMCONNECT_DATATYPE_FLOAT64);
	SimConnect_AddToDataDefinition(hSimConnect, group_id, "NAV IDENT:2", NULL, SIMCONNECT_DATATYPE_STRING8);
	SimConnect_AddToDataDefinition(hSimConnect, group_id, "NAV OBS:2", "Degrees", SIMCONNECT_DATATYPE_FLOAT64);
	SimConnect_AddToDataDefinition(hSimConnect, group_id, "ADF IDENT:1", NULL, SIMCONNECT_DATATYPE_STRING8);
	SimConnect_AddToDataDefinition(hSimConnect, group_id, "ADF CARD", "Degrees", SIMCONNECT_DATATYPE_FLOAT64);
	SimConnect_AddToDataDefinition(hSimConnect, group_id, "NAV DMESPEED:1", "Knots", SIMCONNECT_DATATYPE_FLOAT64);
	SimConnect_AddToDataDefinition(hSimConnect, group_id, "NAV DME:1", "Nautical miles", SIMCONNECT_DATATYPE_FLOAT64);
	SimConnect_AddToDataDefinition(hSimConnect, group_id, "AUTOPILOT HEADING LOCK DIR", "Degrees", SIMCONNECT_DATATYPE_FLOAT64);
	SimConnect_AddToDataDefinition(hSimConnect, group_id, "AUTOPILOT ALTITUDE LOCK VAR", "Feet", SIMCONNECT_DATATYPE_FLOAT64);
	SimConnect_AddToDataDefinition(hSimConnect, group_id, "AUTOPILOT VERTICAL HOLD VAR", "Feet/minute", SIMCONNECT_DATATYPE_FLOAT64);
	SimConnect_AddToDataDefinition(hSimConnect, group_id, "AIRSPEED INDICATED", "Knots", SIMCONNECT_DATATYPE_FLOAT64);
	SimConnect_AddToDataDefinition(hSimConnect, group_id, "INDICATED ALTITUDE", "Feet", SIMCONNECT_DATATYPE_FLOAT64);
	SimConnect_AddToDataDefinition(hSimConnect, group_id, "KOHLSMAN SETTING MB", "MilliBars", SIMCONNECT_DATATYPE_FLOAT64);
	SimConnect_AddToDataDefinition(hSimConnect, group_id, "COM ACTIVE FREQUENCY:1", "Frequency BCD32", SIMCONNECT_DATATYPE_INT32);
	SimConnect_AddToDataDefinition(hSimConnect, group_id, "COM STANDBY FREQUENCY:1", "Frequency BCD32", SIMCONNECT_DATATYPE_INT32);
	SimConnect_AddToDataDefinition(hSimConnect, group_id, "COM TRANSMIT:1", "Bool", SIMCONNECT_DATATYPE_INT32);
	SimConnect_AddToDataDefinition(hSimConnect, group_id, "COM ACTIVE FREQUENCY:2", "Frequency BCD32", SIMCONNECT_DATATYPE_INT32);
	SimConnect_AddToDataDefinition(hSimConnect, group_id, "COM STANDBY FREQUENCY:2", "Frequency BCD32", SIMCONNECT_DATATYPE_INT32);
	SimConnect_AddToDataDefinition(hSimConnect, group_id, "COM TRANSMIT:2", "Bool", SIMCONNECT_DATATYPE_INT32);
	SimConnect_AddToDataDefinition(hSimConnect, group_id, "COM RECEIVE ALL", "Bool", SIMCONNECT_DATATYPE_INT32);
	SimConnect_AddToDataDefinition(hSimConnect, group_id, "NAV ACTIVE FREQUENCY:1", "Frequency BCD32", SIMCONNECT_DATATYPE_INT32);
	SimConnect_AddToDataDefinition(hSimConnect, group_id, "NAV STANDBY FREQUENCY:1", "Frequency BCD32", SIMCONNECT_DATATYPE_INT32);
	SimConnect_AddToDataDefinition(hSimConnect, group_id, "NAV SOUND:1", "Bool", SIMCONNECT_DATATYPE_INT32);
	SimConnect_AddToDataDefinition(hSimConnect, group_id, "NAV ACTIVE FREQUENCY:2", "Frequency BCD32", SIMCONNECT_DATATYPE_INT32);
	SimConnect_AddToDataDefinition(hSimConnect, group_id, "NAV STANDBY FREQUENCY:2", "Frequency BCD32", SIMCONNECT_DATATYPE_INT32);
	SimConnect_AddToDataDefinition(hSimConnect, group_id, "NAV SOUND:2", "Bool", SIMCONNECT_DATATYPE_INT32);
	SimConnect_AddToDataDefinition(hSimConnect, group_id, "ADF ACTIVE FREQUENCY:1", "Frequency ADF BCD32", SIMCONNECT_DATATYPE_INT32);
	SimConnect_AddToDataDefinition(hSimConnect, group_id, "ADF SOUND:1", "Bool", SIMCONNECT_DATATYPE_INT32);
	SimConnect_AddToDataDefinition(hSimConnect, group_id, "DME SOUND:1", "Bool", SIMCONNECT_DATATYPE_INT32);
	SimConnect_AddToDataDefinition(hSimConnect, group_id, "SELECTED DME", "Number", SIMCONNECT_DATATYPE_INT32);
	SimConnect_AddToDataDefinition(hSimConnect, group_id, "NAV HAS DME:1", "Bool", SIMCONNECT_DATATYPE_INT32);
	SimConnect_AddToDataDefinition(hSimConnect, group_id, "NAV HAS DME:2", "Bool", SIMCONNECT_DATATYPE_INT32);
	SimConnect_AddToDataDefinition(hSimConnect, group_id, "TRANSPONDER CODE:1", "BCO16", SIMCONNECT_DATATYPE_INT32);
	SimConnect_AddToDataDefinition(hSimConnect, group_id, "LIGHT ON STATES", "MASK", SIMCONNECT_DATATYPE_INT32);
	SimConnect_AddToDataDefinition(hSimConnect, group_id, "AUTOPILOT MASTER", "Bool", SIMCONNECT_DATATYPE_INT32);
	SimConnect_AddToDataDefinition(hSimConnect, group_id, "AUTOPILOT HEADING LOCK", "Bool", SIMCONNECT_DATATYPE_INT32);
	SimConnect_AddToDataDefinition(hSimConnect, group_id, "AUTOPILOT NAV1 LOCK", "Bool", SIMCONNECT_DATATYPE_INT32);
	SimConnect_AddToDataDefinition(hSimConnect, group_id, "AUTOPILOT ALTITUDE LOCK", "Bool", SIMCONNECT_DATATYPE_INT32);
	SimConnect_AddToDataDefinition(hSimConnect, group_id, "AUTOPILOT BACKCOURSE HOLD", "Bool", SIMCONNECT_DATATYPE_INT32);
	SimConnect_AddToDataDefinition(hSimConnect, group_id, "AUTOPILOT APPROACH HOLD", "Bool", SIMCONNECT_DATATYPE_INT32);
	SimConnect_AddToDataDefinition(hSimConnect, group_id, "AUTOPILOT AIRSPEED HOLD", "Bool", SIMCONNECT_DATATYPE_INT32);
	SimConnect_AddToDataDefinition(hSimConnect, group_id, "GPS DRIVES NAV1", "Bool", SIMCONNECT_DATATYPE_INT32);
}

