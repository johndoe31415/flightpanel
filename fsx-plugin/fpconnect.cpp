#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>
#include "inttypes_emulation.h"
#include "event-enum.h"

#include <windows.h>
#include "SimConnect.h"

static FILE *dbg_f = fopen("debug.log", "a");

struct information_data_t {
	char title[256];
};

struct instrument_data_t {
	uint16_t qnh_millibar;
	uint32_t vhf1_active_khz, vhf1_stby_khz;
	uint32_t vhf2_active_khz, vhf2_stby_khz;
	uint32_t nav1_active_khz, nav1_stby_khz;
	uint32_t nav2_active_khz, nav2_stby_khz;
	uint32_t adf_hz;
	uint16_t squawk;
	bool tx_vhf1, rx_vhf1;
	bool tx_vhf2, rx_vhf2;
	bool nav1_sound, nav2_sound;
	bool dme_ident, adf_ident;
	bool light_bcn, light_land, light_taxi, light_nav, light_strb;
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

void simconnect_instrument_to_abstract(const struct simconnect_instrument_data_t *in, struct instrument_data_t *out) {
	out->qnh_millibar = round(in->qnh);
	out->vhf1_active_khz = round(in->vhf1_active * 1e3);
	out->vhf1_stby_khz = round(in->vhf1_standby * 1e3);
	out->vhf2_active_khz = round(in->vhf2_active * 1e3);
	out->vhf2_stby_khz = round(in->vhf2_standby * 1e3);
	out->nav1_active_khz = round(in->nav1_active * 1e3);
	out->nav1_stby_khz = round(in->nav1_standby * 1e3);
	out->nav2_active_khz = round(in->nav2_active * 1e3);
	out->nav2_stby_khz = round(in->nav2_standby * 1e3);
	out->nav1_sound = in->nav1_sound;
	out->nav2_sound = in->nav2_sound;
	out->adf_hz = round(in->adf1_active * 1e6);
	out->tx_vhf1 = in->vhf1_tx;
	out->tx_vhf2 = in->vhf2_tx;
	if (in->vhf_rx_all) {
		out->rx_vhf1 = true;
		out->rx_vhf2 = true;
	} else {
		out->rx_vhf1 = out->tx_vhf1;
		out->rx_vhf2 = out->tx_vhf2;
	}
	out->dme_ident = in->dme_ident;
	out->adf_ident = in->adf1_ident;
	//printf("squawk %" PRIx64 "\n", in->squawk);
	out->squawk = in->squawk >> (64 - 16);
	printf("lights %" PRIx64 "\n", in->lights_on);
	uint64_t lights = in->lights_on >> (64 - 16);
	out->light_nav = (lights & 0x0001);
	out->light_bcn = (lights & 0x0002);
	out->light_land = (lights & 0x0004);
	out->light_taxi = (lights & 0x0008);
	out->light_strb = (lights & 0x0010);
}

void dump_instrument_data(const struct instrument_data_t *data) {
	printf("VHF1: %3d.%03d (%3d.%03d)     ", data->vhf1_active_khz / 1000, data->vhf1_active_khz % 1000, data->vhf1_stby_khz / 1000, data->vhf1_stby_khz % 1000);
	printf("VHF2: %3d.%03d (%3d.%03d)\n", data->vhf2_active_khz / 1000, data->vhf2_active_khz % 1000, data->vhf2_stby_khz / 1000, data->vhf2_stby_khz % 1000);
	printf("NAV1: %3d.%03d (%3d.%03d)     ", data->nav1_active_khz / 1000, data->nav1_active_khz % 1000, data->nav1_stby_khz / 1000, data->nav1_stby_khz % 1000);
	printf("NAV2: %3d.%03d (%3d.%03d)\n", data->nav2_active_khz / 1000, data->nav2_active_khz % 1000, data->nav2_stby_khz / 1000, data->nav2_stby_khz % 1000);
	printf("ADF %4d.%d kHz    QNH %d mBar\n", data->adf_hz / 1000, data->adf_hz % 1000 / 100, data->qnh_millibar);
	printf("Squawk %04x\n", data->squawk);
	printf("Radios: %1s%4s  %1s%4s %4s %4s %3s %3s\n", data->tx_vhf1 ? ">" : "", data->rx_vhf1 ? "VHF1" : "", data->tx_vhf2 ? ">" : "", data->rx_vhf2 ? "VHF2" : "",
			data->nav1_sound ? "NAV1" : "", data->nav2_sound ? "NAV2" : "",
			data->dme_ident ? "DME" : "", data->adf_ident ? "ADF" : "");
	printf("Lights: %3s %4s %4s %3s %4s\n", data->light_bcn ? "BCN" : "", data->light_land ? "LAND" : "", data->light_taxi ? "TAXI" : "", data->light_nav ? "NAV" : "", data->light_strb ? "STRB" : "");
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

static bool loop_running;

void CALLBACK MyDispatchProcRD(SIMCONNECT_RECV* pData, DWORD cbData, void *pContext) {
	HANDLE hSimConnect = (HANDLE)pContext;
	if (pData->dwID == SIMCONNECT_RECV_ID_EVENT) {
		SIMCONNECT_RECV_EVENT *evt = (SIMCONNECT_RECV_EVENT*)pData;
		enum event_t event_id = (enum event_t)evt->uEventID;
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
			//DWORD object_id = pObjData->dwObjectID;
			struct simconnect_instrument_data_t *simconnect_data = (struct simconnect_instrument_data_t*)&pObjData->dwData;

			struct instrument_data_t abstract_data;
			memset(&abstract_data, 0, sizeof(abstract_data));
			simconnect_instrument_to_abstract(simconnect_data, &abstract_data);
			dump_instrument_data(&abstract_data);
		} else {
			printf("Recevied unhandled data for request 0x%lx\n", pObjData->dwRequestID);
		}
	} else if (pData->dwID == SIMCONNECT_RECV_ID_QUIT) {
		loop_running = false;
		printf("Received termination signal.\n");
	} else {
		printf("Unhandled callback dwID 0x%lx\n", pData->dwID);
	}
}

void simconnect_loop(HANDLE hSimConnect) {
	bool loop_running = true;
	int i = 0;
	SimConnect_RequestDataOnSimObjectType(hSimConnect, REQUEST_DATADEF_INFO, DATADEF_INFO, 0, SIMCONNECT_SIMOBJECT_TYPE_USER);
	SimConnect_RequestDataOnSimObjectType(hSimConnect, REQUEST_DATADEF_INSTRUMENTS, DATADEF_INSTRUMENTS, 0, SIMCONNECT_SIMOBJECT_TYPE_USER);
	while (loop_running) {
		SimConnect_CallDispatch(hSimConnect, MyDispatchProcRD, hSimConnect);
		Sleep(10);
		i++;
		if (i == 100) {
			i = 0;
			//SimConnect_TransmitClientEvent(hSimConnect, 0, EVENT_BRAKES, 0, SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
			//SimConnect_TransmitClientEvent(hSimConnect, 0, EVENT_SET_ADF, 0x12340, SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
		}
	}
}

int main() {
	HANDLE hSimConnect = NULL;
	if (SUCCEEDED(SimConnect_Open(&hSimConnect, "Flight Panel", NULL, 0, 0, 0))) {
		printf("Connected.\n");
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
		const struct event_enum_data_t *event_data;
		while ((event_data = get_event_enum_data((event_t)event_enum)) != NULL) {
			if (event_data->flags == 1) {
				/* Instrument changed group */
				SimConnect_MapClientEventToSimEvent(hSimConnect, event_enum, event_data->name);
				SimConnect_AddClientEventToNotificationGroup(hSimConnect, EVENTGROUP_INSTRUMENT_CHANGED, event_enum);
			}
			event_enum++;
		}

		simconnect_loop(hSimConnect);
		SimConnect_Close(hSimConnect);
	} else {
		printf("Connection failed.\n");
	}
	return 0;
}
