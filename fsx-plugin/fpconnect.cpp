#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "inttypes_emulation.h"
#include "event-enum.h"

#include <windows.h>
#include "SimConnect.h"

static FILE *dbg_f = fopen("debug.log", "a");

struct information_data_t {
	char title[256];
};

struct instrument_data_t {
	double qnh;
	double nav1_active, nav1_standby;
	uint64_t adf1_active;
	uint64_t adf1_sound;
	double align;
} __attribute__((packed));

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

static int adf_byte1_bcd32_to_khz(uint8_t byte1) {
	if ((byte1 & 0xf0) == 0xb0) {
		return (byte1 & 0x0f) + 10;
	} else {
		switch (byte1) {
			case 0x72:
			case 0x78:
			case 0x79:
			case 0x74:		return 1;
			case 0x84:
			case 0x82:		return 2;
			case 0x8c:
			case 0x8a:		return 3;
			case 0x92:
			case 0x91:		return 4;
			case 0x96:
			case 0x95:		return 5;
			case 0x9a:
			case 0x99:		return 6;
			case 0x9e:
			case 0x9d:		return 7;
			case 0xa1:
			case 0xa0:		return 8;
			case 0xa3:
			case 0xa2:		return 9;
			default:
				printf("Cannot interpret ADF byte 1: 0x%x\n", byte1);
				return 0;
		}
	}
}

uint32_t adf_bcd32_to_khz(uint64_t value) {
	return adf_byte1_bcd32_to_khz((value >> (8 * 6)) & 0xff) * 100000
			+ ((value >> (4 * 12)) & 0x0f) * 10000
			+ ((value >> (4 * 11)) & 0x0f) * 1000
			+ ((value >> (4 * 10)) & 0x0f) * 100;

}

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
			struct instrument_data_t *fdata = (struct instrument_data_t*)&pObjData->dwData;
			//uint32_t adfx = adf_bcd32_to_khz(fdata->adf1_active);
			//printf("Receive data object %ld: QNH %.0f NAV1 %.2f (%.2f) ADF %u.%03u sound %" PRIx64 " align %.2f\n", object_id, fdata->qnh, fdata->nav1_active, fdata->nav1_standby, adf / 1000, adf % 1000, fdata->adf1_sound, fdata->align);
			/*
			printf("%d raw bytes:\n", sizeof(instrument_data_t));
			for (int i = 0; i < sizeof(struct instrument_data_t); i++) {
				if (i && ((i % 16) == 0)) printf("\n");
				printf("%02x ", ((uint8_t*)fdata)[i]);

			}
			printf("\n");
			*/

			/*
			printf("%lu values:\n", pObjData->dwDefineCount);
			for (unsigned int i = 0; i < pObjData->dwDefineCount; i++) {
				printf("%d: %" PRIx64 "\n", i, ((uint64_t*)fdata)[i]);
			}
			*/
			{
				//uint64_t adf = fdata->adf1_active;
				//for (int i = 7; i >= 0; i--) {
				//	fprintf(f, "%x ", (adf >> (32 + (4 * i))) & 0xf);
				//}
				//fprintf(f, " -> %u\n", adfx);
				fprintf(dbg_f, "%" PRIx64 "\n", fdata->adf1_active);
				fflush(dbg_f);
			}
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
		SimConnect_AddToDataDefinition(hSimConnect, DATADEF_INSTRUMENTS, "NAV ACTIVE FREQUENCY:1", "MHz");
		SimConnect_AddToDataDefinition(hSimConnect, DATADEF_INSTRUMENTS, "NAV STANDBY FREQUENCY:1", "MHz");
		SimConnect_AddToDataDefinition(hSimConnect, DATADEF_INSTRUMENTS, "ADF ACTIVE FREQUENCY:1", "Frequency ADF BCD32");
		SimConnect_AddToDataDefinition(hSimConnect, DATADEF_INSTRUMENTS, "ADF SOUND:1", "Bool");
		SimConnect_AddToDataDefinition(hSimConnect, DATADEF_INSTRUMENTS, "NAV ACTIVE FREQUENCY:1", "MHz");

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
