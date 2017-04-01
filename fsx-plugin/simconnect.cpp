#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>
#include "inttypes_emulation.h"
#include "simconnect-events.h"
#include "fsconnection.hpp"

#include <windows.h>
#include "SimConnect.h"

static bool simconnect_connected;

struct information_data_t {
	char title[256];
};

struct simconnect_instrument_data_t {
	double qnh;
	double vhf1_active, vhf1_standby;
	double vhf2_active, vhf2_standby;
	uint64_t vhf1_tx, vhf2_tx, vhf_rx_all;
	double nav1_active, nav1_standby;
	double nav2_active, nav2_standby;
	uint64_t nav1_sound, nav2_sound;
	double adf1_active;
	uint64_t dme_ident;
	uint64_t adf1_ident;
	uint64_t squawk;
	uint64_t lights_on;
} __attribute__((packed));

static void simconnect_instrument_to_abstract(const struct simconnect_instrument_data_t *in, struct instrument_data_t *out) {
	out->vhf1.freq_active_khz = round(in->vhf1_active * 1e3);
	out->vhf1.freq_standby_khz = round(in->vhf1_standby * 1e3);
	out->vhf1.tx = in->vhf1_tx;
	out->vhf2.freq_active_khz = round(in->vhf2_active * 1e3);
	out->vhf2.freq_standby_khz = round(in->vhf2_standby * 1e3);
	out->vhf2.tx = in->vhf2_tx;
	if (in->vhf_rx_all) {
		out->vhf1.rx = true;
		out->vhf2.rx = true;
	} else {
		out->vhf1.rx = out->vhf1.tx;
		out->vhf2.rx = out->vhf2.tx;
	}
	
	out->nav1.freq_active_khz = round(in->nav1_active * 1e3);
	out->nav1.freq_standby_khz = round(in->nav1_standby * 1e3);
	out->nav1.sound = in->nav1_sound;
	
	out->nav2.freq_active_khz = round(in->nav2_active * 1e3);
	out->nav2.freq_standby_khz = round(in->nav2_standby * 1e3);
	out->nav2.sound = in->nav2_sound;
	
	out->adf.freq_hz = round(in->adf1_active * 1e6);
	out->adf.sound = in->adf1_ident;
	
	out->dme.sound = in->dme_ident;

	//printf("squawk %" PRIx64 "\n", in->squawk);
	out->xpdr.squawk = in->squawk >> (64 - 16);
	printf("lights %" PRIx64 "\n", in->lights_on);

	uint64_t lights = in->lights_on >> (64 - 16);
	out->lights.nav = (lights & 0x0001);
	out->lights.beacon = (lights & 0x0002);
	out->lights.landing = (lights & 0x0004);
	out->lights.taxi = (lights & 0x0008);
	out->lights.strobe = (lights & 0x0010);
	
	out->misc.qnh_millibar = round(in->qnh);
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

static void CALLBACK simconnect_callback(SIMCONNECT_RECV* pData, DWORD cbData, void *pContext) {
	HANDLE hSimConnect = (HANDLE)pContext;
	if (pData->dwID == SIMCONNECT_RECV_ID_EVENT) {
		SIMCONNECT_RECV_EVENT *evt = (SIMCONNECT_RECV_EVENT*)pData;
		enum simconnect_event_t event_id = (enum simconnect_event_t)evt->uEventID;
		if (event_id == EVENT_SIM_START) {
			printf("Simulation started.\n");
			SimConnect_RequestDataOnSimObjectType(hSimConnect, REQUEST_DATADEF_INFO, DATADEF_INFO, 0, SIMCONNECT_SIMOBJECT_TYPE_USER);
			SimConnect_RequestDataOnSimObjectType(hSimConnect, REQUEST_DATADEF_INSTRUMENTS, DATADEF_INSTRUMENTS, 0, SIMCONNECT_SIMOBJECT_TYPE_USER);
		} else if (evt->uGroupID == EVENTGROUP_INSTRUMENT_CHANGED) {
			//printf("Instrument changed: %s\n", get_event_enum_name(event_id));
			SimConnect_RequestDataOnSimObjectType(hSimConnect, REQUEST_DATADEF_INSTRUMENTS, DATADEF_INSTRUMENTS, 0, SIMCONNECT_SIMOBJECT_TYPE_USER);
		} else {
			printf("Unhandled event received: 0x%lx\n", evt->uEventID);
		}
	} else if (pData->dwID == SIMCONNECT_RECV_ID_SIMOBJECT_DATA_BYTYPE) {
		SIMCONNECT_RECV_SIMOBJECT_DATA_BYTYPE *pObjData = (SIMCONNECT_RECV_SIMOBJECT_DATA_BYTYPE*)pData;
		DWORD object_id = pObjData->dwObjectID;
		if (pObjData->dwRequestID == REQUEST_DATADEF_INFO) {
			struct information_data_t *fdata = (struct information_data_t*)&pObjData->dwData;
			fdata->title[255] = 0;
			printf("Receive data about object %ld: %s\n", object_id, fdata->title);
		} else if (pObjData->dwRequestID == REQUEST_DATADEF_INSTRUMENTS) {
			struct simconnect_instrument_data_t *simconnect_data = (struct simconnect_instrument_data_t*)&pObjData->dwData;

			struct instrument_data_t abstract_data;
			memset(&abstract_data, 0, sizeof(abstract_data));
			simconnect_instrument_to_abstract(simconnect_data, &abstract_data);
			fsconnection_incoming_data(&abstract_data);
		} else {
			printf("Recevied unhandled data for request 0x%lx\n", pObjData->dwRequestID);
		}
	} else if (pData->dwID == SIMCONNECT_RECV_ID_QUIT) {
		simconnect_connected = false;
		printf("Received termination signal.\n");
	} else {
		printf("Unhandled callback dwID 0x%lx\n", pData->dwID);
	}
}

void simconnect_event_loop(void *hSimConnect) {
	simconnect_connected = true;
	SimConnect_RequestDataOnSimObjectType(hSimConnect, REQUEST_DATADEF_INFO, DATADEF_INFO, 0, SIMCONNECT_SIMOBJECT_TYPE_USER);
	SimConnect_RequestDataOnSimObjectType(hSimConnect, REQUEST_DATADEF_INSTRUMENTS, DATADEF_INSTRUMENTS, 0, SIMCONNECT_SIMOBJECT_TYPE_USER);
	while (simconnect_connected) {
		SimConnect_CallDispatch(hSimConnect, simconnect_callback, hSimConnect);
		Sleep(10);
	}
}

void* simconnect_init(void) {
	void *hSimConnect = NULL;
	if (SUCCEEDED(SimConnect_Open(&hSimConnect, "Flight Panel", NULL, 0, 0, 0))) {
		printf("Connected to simulator.\n");
		SimConnect_AddToDataDefinition(hSimConnect, DATADEF_INFO, "Title", NULL, SIMCONNECT_DATATYPE_STRING256);
		SimConnect_AddToDataDefinition(hSimConnect, DATADEF_INSTRUMENTS, "KOHLSMAN SETTING MB", "MilliBars");
		SimConnect_AddToDataDefinition(hSimConnect, DATADEF_INSTRUMENTS, "COM ACTIVE FREQUENCY:1", "MHz");
		SimConnect_AddToDataDefinition(hSimConnect, DATADEF_INSTRUMENTS, "COM STANDBY FREQUENCY:1", "MHz");
		SimConnect_AddToDataDefinition(hSimConnect, DATADEF_INSTRUMENTS, "COM ACTIVE FREQUENCY:2", "MHz");
		SimConnect_AddToDataDefinition(hSimConnect, DATADEF_INSTRUMENTS, "COM STANDBY FREQUENCY:2", "MHz");
		SimConnect_AddToDataDefinition(hSimConnect, DATADEF_INSTRUMENTS, "COM TRANSMIT:1", "Bool");
		SimConnect_AddToDataDefinition(hSimConnect, DATADEF_INSTRUMENTS, "COM TRANSMIT:2", "Bool");
		SimConnect_AddToDataDefinition(hSimConnect, DATADEF_INSTRUMENTS, "COM RECEIVE ALL", "Bool");
		SimConnect_AddToDataDefinition(hSimConnect, DATADEF_INSTRUMENTS, "NAV ACTIVE FREQUENCY:1", "MHz");
		SimConnect_AddToDataDefinition(hSimConnect, DATADEF_INSTRUMENTS, "NAV STANDBY FREQUENCY:1", "MHz");
		SimConnect_AddToDataDefinition(hSimConnect, DATADEF_INSTRUMENTS, "NAV ACTIVE FREQUENCY:2", "MHz");
		SimConnect_AddToDataDefinition(hSimConnect, DATADEF_INSTRUMENTS, "NAV STANDBY FREQUENCY:2", "MHz");
		SimConnect_AddToDataDefinition(hSimConnect, DATADEF_INSTRUMENTS, "NAV SOUND:1", "Bool");
		SimConnect_AddToDataDefinition(hSimConnect, DATADEF_INSTRUMENTS, "NAV SOUND:2", "Bool");
		SimConnect_AddToDataDefinition(hSimConnect, DATADEF_INSTRUMENTS, "ADF ACTIVE FREQUENCY:1", "MHz");
		SimConnect_AddToDataDefinition(hSimConnect, DATADEF_INSTRUMENTS, "DME SOUND:1", "Bool");
		SimConnect_AddToDataDefinition(hSimConnect, DATADEF_INSTRUMENTS, "ADF SOUND:1", "Bool");
		SimConnect_AddToDataDefinition(hSimConnect, DATADEF_INSTRUMENTS, "TRANSPONDER CODE:1", "BCO16");
		SimConnect_AddToDataDefinition(hSimConnect, DATADEF_INSTRUMENTS, "LIGHT ON STATES", "MASK");

		SimConnect_SubscribeToSystemEvent(hSimConnect, EVENT_SIM_START, "SimStart");

		/* Register all enum values which have flag 1 for instrument changed events */
		int event_enum = 0;
		const struct simconnect_event_enum_data_t *event_data;
		while ((event_data = get_simconnect_event_enum_data((simconnect_event_t)event_enum)) != NULL) {
			if (event_data->flags == 1) {
				/* Instrument changed group */
				SimConnect_MapClientEventToSimEvent(hSimConnect, event_enum, event_data->name);
				SimConnect_AddClientEventToNotificationGroup(hSimConnect, EVENTGROUP_INSTRUMENT_CHANGED, event_enum);
			}
			event_enum++;
		}
	} else {
		printf("Connection failed.\n");
		return NULL;
	}
	return hSimConnect;
}

void simconnect_close(void *hSimConnect) {
	SimConnect_Close(hSimConnect);
}

