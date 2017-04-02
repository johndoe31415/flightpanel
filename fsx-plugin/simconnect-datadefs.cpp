#include <stdio.h>
#include <stdint.h>
#include <windows.h>
#include "SimConnect.h"
#include "simconnect-datadefs.hpp"

void simconnect_datadefs_register_information(void *hSimConnect, int group_id) {
	SimConnect_AddToDataDefinition(hSimConnect, group_id, "TITLE", NULL, SIMCONNECT_DATATYPE_STRING256);
}

void simconnect_datadefs_register_instruments(void *hSimConnect, int group_id) {
	SimConnect_AddToDataDefinition(hSimConnect, group_id, "COM ACTIVE FREQUENCY:1", "MHz", SIMCONNECT_DATATYPE_FLOAT64);
	SimConnect_AddToDataDefinition(hSimConnect, group_id, "COM STANDBY FREQUENCY:1", "MHz", SIMCONNECT_DATATYPE_FLOAT64);
	SimConnect_AddToDataDefinition(hSimConnect, group_id, "COM ACTIVE FREQUENCY:2", "MHz", SIMCONNECT_DATATYPE_FLOAT64);
	SimConnect_AddToDataDefinition(hSimConnect, group_id, "COM STANDBY FREQUENCY:2", "MHz", SIMCONNECT_DATATYPE_FLOAT64);
	SimConnect_AddToDataDefinition(hSimConnect, group_id, "NAV ACTIVE FREQUENCY:1", "MHz", SIMCONNECT_DATATYPE_FLOAT64);
	SimConnect_AddToDataDefinition(hSimConnect, group_id, "NAV STANDBY FREQUENCY:1", "MHz", SIMCONNECT_DATATYPE_FLOAT64);
	SimConnect_AddToDataDefinition(hSimConnect, group_id, "NAV ACTIVE FREQUENCY:2", "MHz", SIMCONNECT_DATATYPE_FLOAT64);
	SimConnect_AddToDataDefinition(hSimConnect, group_id, "NAV STANDBY FREQUENCY:2", "MHz", SIMCONNECT_DATATYPE_FLOAT64);
	SimConnect_AddToDataDefinition(hSimConnect, group_id, "ADF ACTIVE FREQUENCY:1", "MHz", SIMCONNECT_DATATYPE_FLOAT64);
	SimConnect_AddToDataDefinition(hSimConnect, group_id, "KOHLSMAN SETTING MB", "MilliBars", SIMCONNECT_DATATYPE_FLOAT64);
	SimConnect_AddToDataDefinition(hSimConnect, group_id, "COM TRANSMIT:1", "Bool", SIMCONNECT_DATATYPE_INT32);
	SimConnect_AddToDataDefinition(hSimConnect, group_id, "COM TRANSMIT:2", "Bool", SIMCONNECT_DATATYPE_INT32);
	SimConnect_AddToDataDefinition(hSimConnect, group_id, "COM RECEIVE ALL", "Bool", SIMCONNECT_DATATYPE_INT32);
	SimConnect_AddToDataDefinition(hSimConnect, group_id, "NAV SOUND:1", "Bool", SIMCONNECT_DATATYPE_INT32);
	SimConnect_AddToDataDefinition(hSimConnect, group_id, "NAV SOUND:2", "Bool", SIMCONNECT_DATATYPE_INT32);
	SimConnect_AddToDataDefinition(hSimConnect, group_id, "ADF SOUND:1", "Bool", SIMCONNECT_DATATYPE_INT32);
	SimConnect_AddToDataDefinition(hSimConnect, group_id, "DME SOUND:1", "Bool", SIMCONNECT_DATATYPE_INT32);
	SimConnect_AddToDataDefinition(hSimConnect, group_id, "TRANSPONDER CODE:1", "BCO16", SIMCONNECT_DATATYPE_INT32);
	SimConnect_AddToDataDefinition(hSimConnect, group_id, "LIGHT ON STATES", "MASK", SIMCONNECT_DATATYPE_INT32);
}

