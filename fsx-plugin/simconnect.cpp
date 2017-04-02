#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>
#include <stddef.h>
#include <unistd.h>
#include "inttypes_emulation.h"
#include "simconnect-events.h"
#include "simconnect-datadefs.hpp"
#include "fsconnection.hpp"

#include <windows.h>
#include "SimConnect.h"

/* Internal data structure passed to callback, masked outside this module as
 * struct flightsim_connection_t* 
 **/
struct simconnect_context_t {
	HANDLE simconnect_handle;
	bool loop_running;
};

static uint32_t bcd_to_decimal(uint32_t bcd_value) {
	uint32_t result = 0;
	uint32_t value = 1;
	while (bcd_value) {
		uint8_t digit = bcd_value & 0xf;
		result += digit * value;
		value *= 10;
		bcd_value >>= 4;
	}
	return result;
}

#if 0
static uint32_t frequency_bcd16_to_khz(uint32_t bcd16_value) {
	uint32_t khz = 100000 + (bcd_to_decimal(bcd16_value) * 10);
	if (((khz % 100) == 20) || ((khz % 100) == 70)) {
		khz += 5;
	}
	return khz;
}
#endif

static uint32_t frequency_bcd32_to_khz(uint32_t bcd32_value) {
	return bcd_to_decimal(bcd32_value) / 10;
}

static uint32_t adf_frequency_bcd32_to_hz(uint32_t bcd32_value) {
	return bcd_to_decimal(bcd32_value) / 10;
}

static void simconnect_instrument_to_abstract(const struct simconnect_datatype_instruments_t *in, struct instrument_data_t *out) {
	out->vhf1.freq_active_khz = frequency_bcd32_to_khz(in->vhf1_freq_active);
	out->vhf1.freq_standby_khz = frequency_bcd32_to_khz(in->vhf1_freq_standby);
	out->vhf1.tx = in->vhf1_tx;
	out->vhf2.freq_active_khz = frequency_bcd32_to_khz(in->vhf2_freq_active);
	out->vhf2.freq_standby_khz = frequency_bcd32_to_khz(in->vhf2_freq_standby);
	out->vhf2.tx = in->vhf2_tx;
	if (in->vhf_all_rx) {
		out->vhf1.rx = true;
		out->vhf2.rx = true;
	} else {
		out->vhf1.rx = out->vhf1.tx;
		out->vhf2.rx = out->vhf2.tx;
	}

	out->nav1.freq_active_khz = frequency_bcd32_to_khz(in->nav1_freq_active);
	out->nav1.freq_standby_khz = frequency_bcd32_to_khz(in->nav1_freq_standby);
	out->nav1.sound = in->nav1_sound;

	out->nav2.freq_active_khz = frequency_bcd32_to_khz(in->nav2_freq_active);
	out->nav2.freq_standby_khz = frequency_bcd32_to_khz(in->nav2_freq_standby);
	out->nav2.sound = in->nav2_sound;

	out->adf.freq_hz = adf_frequency_bcd32_to_hz(in->adf_freq_active);
	out->adf.sound = in->adf_sound;

	out->dme.sound = in->dme_sound;

	out->xpdr.squawk = bcd_to_decimal(in->xpdr_squawk);

	out->lights.nav = (in->light_states & 0x0001);
	out->lights.beacon = (in->light_states & 0x0002);
	out->lights.landing = (in->light_states & 0x0004);
	out->lights.taxi = (in->light_states & 0x0008);
	out->lights.strobe = (in->light_states & 0x0010);

	out->misc.qnh_millibar = round(in->qnh_millibar);
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

static void CALLBACK simconnect_callback(SIMCONNECT_RECV* pData, DWORD cbData, void *vcontext) {
	struct simconnect_context_t *context = (struct simconnect_context_t*)vcontext;
	if (pData->dwID == SIMCONNECT_RECV_ID_EVENT) {
		SIMCONNECT_RECV_EVENT *evt = (SIMCONNECT_RECV_EVENT*)pData;
		enum simconnect_event_t event_id = (enum simconnect_event_t)evt->uEventID;
		if (event_id == EVENT_SIM_START) {
			printf("Simulation started.\n");
			SimConnect_RequestDataOnSimObjectType(context->simconnect_handle, REQUEST_DATADEF_INFO, DATADEF_INFO, 0, SIMCONNECT_SIMOBJECT_TYPE_USER);
			SimConnect_RequestDataOnSimObjectType(context->simconnect_handle, REQUEST_DATADEF_INSTRUMENTS, DATADEF_INSTRUMENTS, 0, SIMCONNECT_SIMOBJECT_TYPE_USER);
		} else if (evt->uGroupID == EVENTGROUP_INSTRUMENT_CHANGED) {
			//printf("Instrument changed: %s\n", get_event_enum_name(event_id));
			SimConnect_RequestDataOnSimObjectType(context->simconnect_handle, REQUEST_DATADEF_INSTRUMENTS, DATADEF_INSTRUMENTS, 0, SIMCONNECT_SIMOBJECT_TYPE_USER);
		} else {
			printf("Unhandled event received: 0x%lx\n", evt->uEventID);
		}
	} else if (pData->dwID == SIMCONNECT_RECV_ID_SIMOBJECT_DATA_BYTYPE) {
		SIMCONNECT_RECV_SIMOBJECT_DATA_BYTYPE *pObjData = (SIMCONNECT_RECV_SIMOBJECT_DATA_BYTYPE*)pData;
		DWORD object_id = pObjData->dwObjectID;
		if (pObjData->dwRequestID == REQUEST_DATADEF_INFO) {
			struct simconnect_datatype_information_t *fdata = (struct simconnect_datatype_information_t*)&pObjData->dwData;
			fdata->title[255] = 0;
			printf("Receive data about object %ld: %s\n", object_id, fdata->title);
		} else if (pObjData->dwRequestID == REQUEST_DATADEF_INSTRUMENTS) {
			struct simconnect_datatype_instruments_t *simconnect_data = (struct simconnect_datatype_instruments_t*)&pObjData->dwData;
			if (SIMCONNECT_DATATYPE_INSTRUMENTS_DATUM_COUNT != pObjData->dwDefineCount) {
				printf("Warning: expected %u datums, but received %lu.\n", SIMCONNECT_DATATYPE_INSTRUMENTS_DATUM_COUNT, pObjData->dwDefineCount);
			}
			struct instrument_data_t abstract_data;
			memset(&abstract_data, 0, sizeof(abstract_data));
			simconnect_instrument_to_abstract(simconnect_data, &abstract_data);
			fsconnection_incoming_data(&abstract_data);
		} else {
			printf("Recevied unhandled data for request 0x%lx\n", pObjData->dwRequestID);
		}
	} else if (pData->dwID == SIMCONNECT_RECV_ID_QUIT) {
		context->loop_running = false;
		printf("Received termination signal.\n");
	} else {
		printf("Unhandled callback dwID 0x%lx\n", pData->dwID);
	}
}

void simconnect_event_loop(struct flightsim_connection_t *fsconnection) {
	struct simconnect_context_t *context = (struct simconnect_context_t*)fsconnection;
	context->loop_running = true;
	SimConnect_RequestDataOnSimObjectType(context->simconnect_handle, REQUEST_DATADEF_INFO, DATADEF_INFO, 0, SIMCONNECT_SIMOBJECT_TYPE_USER);
	SimConnect_RequestDataOnSimObjectType(context->simconnect_handle, REQUEST_DATADEF_INSTRUMENTS, DATADEF_INSTRUMENTS, 0, SIMCONNECT_SIMOBJECT_TYPE_USER);
	while (context->loop_running) {
		SimConnect_CallDispatch(context->simconnect_handle, simconnect_callback, context);
		usleep(10 * 1000);
	}
}

struct flightsim_connection_t* simconnect_init(void) {
	/* Allocate memory for opaque structure first */
	struct simconnect_context_t *context = (struct simconnect_context_t*)calloc(1, sizeof(*context));
	if (!context) {
		fprintf(stderr, "calloc simconnect_context_t: %s\n", strerror(errno));
		return NULL;
	}

	if (SUCCEEDED(SimConnect_Open(&context->simconnect_handle, "Flight Panel", NULL, 0, 0, 0))) {
		printf("Successfully connected to flight simulator.\n");
		simconnect_datadefs_register_information(context->simconnect_handle, DATADEF_INFO);
		simconnect_datadefs_register_instruments(context->simconnect_handle, DATADEF_INSTRUMENTS);
		SimConnect_SubscribeToSystemEvent(context->simconnect_handle, EVENT_SIM_START, "SimStart");

		/* Register all enum values which have flag 1 for instrument changed events */
		int event_enum = 0;
		const struct simconnect_event_enum_data_t *event_data;
		while ((event_data = get_simconnect_event_enum_data((simconnect_event_t)event_enum)) != NULL) {
			if (event_data->flags == 1) {
				/* Instrument changed group */
				SimConnect_MapClientEventToSimEvent(context->simconnect_handle, event_enum, event_data->name);
				SimConnect_AddClientEventToNotificationGroup(context->simconnect_handle, EVENTGROUP_INSTRUMENT_CHANGED, event_enum);
			}
			event_enum++;
		}
	} else {
		fprintf(stderr, "Connection to flight simulator failed.\n");
		free(context);
		return NULL;
	}

	return (struct flightsim_connection_t*)context;
}

void simconnect_close(struct flightsim_connection_t *fsconnection) {
	struct simconnect_context_t *context = (struct simconnect_context_t*)fsconnection;
	SimConnect_Close(context->simconnect_handle);
	free(context);
}

