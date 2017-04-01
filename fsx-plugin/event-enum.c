#include <stdint.h>
#include "event-enum.h"

static const struct event_enum_data_t event_enum_data[] = {
	[EVENT_SIM_START] = {
		.name = "SIM_START",
		.flags = 0,
	},
	[EVENT_AP_MASTER] = {
		.name = "AP_MASTER",
		.flags = 1,
	},
	[EVENT_AUTOPILOT_OFF] = {
		.name = "AUTOPILOT_OFF",
		.flags = 1,
	},
	[EVENT_AUTOPILOT_ON] = {
		.name = "AUTOPILOT_ON",
		.flags = 1,
	},
	[EVENT_AP_PANEL_HEADING_HOLD] = {
		.name = "AP_PANEL_HEADING_HOLD",
		.flags = 1,
	},
	[EVENT_AP_PANEL_ALTITUDE_HOLD] = {
		.name = "AP_PANEL_ALTITUDE_HOLD",
		.flags = 1,
	},
	[EVENT_AP_AIRSPEED_HOLD] = {
		.name = "AP_AIRSPEED_HOLD",
		.flags = 1,
	},
	[EVENT_AP_PANEL_SPEED_HOLD] = {
		.name = "AP_PANEL_SPEED_HOLD",
		.flags = 1,
	},
	[EVENT_AP_ALT_VAR_INC] = {
		.name = "AP_ALT_VAR_INC",
		.flags = 1,
	},
	[EVENT_AP_ALT_VAR_DEC] = {
		.name = "AP_ALT_VAR_DEC",
		.flags = 1,
	},
	[EVENT_AP_VS_VAR_INC] = {
		.name = "AP_VS_VAR_INC",
		.flags = 1,
	},
	[EVENT_AP_VS_VAR_DEC] = {
		.name = "AP_VS_VAR_DEC",
		.flags = 1,
	},
	[EVENT_AP_SPD_VAR_INC] = {
		.name = "AP_SPD_VAR_INC",
		.flags = 1,
	},
	[EVENT_AP_SPD_VAR_DEC] = {
		.name = "AP_SPD_VAR_DEC",
		.flags = 1,
	},
	[EVENT_AP_ALT_VAR_SET_METRIC] = {
		.name = "AP_ALT_VAR_SET_METRIC",
		.flags = 1,
	},
	[EVENT_AP_VS_VAR_SET_ENGLISH] = {
		.name = "AP_VS_VAR_SET_ENGLISH",
		.flags = 1,
	},
	[EVENT_AP_SPD_VAR_SET] = {
		.name = "AP_SPD_VAR_SET",
		.flags = 1,
	},
	[EVENT_AP_AIRSPEED_ON] = {
		.name = "AP_AIRSPEED_ON",
		.flags = 1,
	},
	[EVENT_AP_AIRSPEED_OFF] = {
		.name = "AP_AIRSPEED_OFF",
		.flags = 1,
	},
	[EVENT_AP_MACH_ON] = {
		.name = "AP_MACH_ON",
		.flags = 1,
	},
	[EVENT_AP_MACH_OFF] = {
		.name = "AP_MACH_OFF",
		.flags = 1,
	},
	[EVENT_AP_PANEL_ALTITUDE_ON] = {
		.name = "AP_PANEL_ALTITUDE_ON",
		.flags = 1,
	},
	[EVENT_AP_PANEL_ALTITUDE_OFF] = {
		.name = "AP_PANEL_ALTITUDE_OFF",
		.flags = 1,
	},
	[EVENT_AP_PANEL_HEADING_ON] = {
		.name = "AP_PANEL_HEADING_ON",
		.flags = 1,
	},
	[EVENT_AP_PANEL_HEADING_OFF] = {
		.name = "AP_PANEL_HEADING_OFF",
		.flags = 1,
	},
	[EVENT_COM_RADIO_WHOLE_DEC] = {
		.name = "COM_RADIO_WHOLE_DEC",
		.flags = 1,
	},
	[EVENT_COM_RADIO_WHOLE_INC] = {
		.name = "COM_RADIO_WHOLE_INC",
		.flags = 1,
	},
	[EVENT_COM_RADIO_FRACT_DEC] = {
		.name = "COM_RADIO_FRACT_DEC",
		.flags = 1,
	},
	[EVENT_COM_RADIO_FRACT_INC] = {
		.name = "COM_RADIO_FRACT_INC",
		.flags = 1,
	},
	[EVENT_NAV1_RADIO_WHOLE_DEC] = {
		.name = "NAV1_RADIO_WHOLE_DEC",
		.flags = 1,
	},
	[EVENT_NAV1_RADIO_WHOLE_INC] = {
		.name = "NAV1_RADIO_WHOLE_INC",
		.flags = 1,
	},
	[EVENT_NAV1_RADIO_FRACT_DEC] = {
		.name = "NAV1_RADIO_FRACT_DEC",
		.flags = 1,
	},
	[EVENT_NAV1_RADIO_FRACT_INC] = {
		.name = "NAV1_RADIO_FRACT_INC",
		.flags = 1,
	},
	[EVENT_NAV2_RADIO_WHOLE_DEC] = {
		.name = "NAV2_RADIO_WHOLE_DEC",
		.flags = 1,
	},
	[EVENT_NAV2_RADIO_WHOLE_INC] = {
		.name = "NAV2_RADIO_WHOLE_INC",
		.flags = 1,
	},
	[EVENT_NAV2_RADIO_FRACT_DEC] = {
		.name = "NAV2_RADIO_FRACT_DEC",
		.flags = 1,
	},
	[EVENT_NAV2_RADIO_FRACT_INC] = {
		.name = "NAV2_RADIO_FRACT_INC",
		.flags = 1,
	},
	[EVENT_ADF_100_INC] = {
		.name = "ADF_100_INC",
		.flags = 1,
	},
	[EVENT_ADF_10_INC] = {
		.name = "ADF_10_INC",
		.flags = 1,
	},
	[EVENT_ADF_1_INC] = {
		.name = "ADF_1_INC",
		.flags = 1,
	},
	[EVENT_XPNDR_1000_INC] = {
		.name = "XPNDR_1000_INC",
		.flags = 1,
	},
	[EVENT_XPNDR_100_INC] = {
		.name = "XPNDR_100_INC",
		.flags = 1,
	},
	[EVENT_XPNDR_10_INC] = {
		.name = "XPNDR_10_INC",
		.flags = 1,
	},
	[EVENT_XPNDR_1_INC] = {
		.name = "XPNDR_1_INC",
		.flags = 1,
	},
	[EVENT_VOR1_OBI_DEC] = {
		.name = "VOR1_OBI_DEC",
		.flags = 1,
	},
	[EVENT_VOR1_OBI_INC] = {
		.name = "VOR1_OBI_INC",
		.flags = 1,
	},
	[EVENT_VOR2_OBI_DEC] = {
		.name = "VOR2_OBI_DEC",
		.flags = 1,
	},
	[EVENT_VOR2_OBI_INC] = {
		.name = "VOR2_OBI_INC",
		.flags = 1,
	},
	[EVENT_ADF_100_DEC] = {
		.name = "ADF_100_DEC",
		.flags = 1,
	},
	[EVENT_ADF_10_DEC] = {
		.name = "ADF_10_DEC",
		.flags = 1,
	},
	[EVENT_ADF_1_DEC] = {
		.name = "ADF_1_DEC",
		.flags = 1,
	},
	[EVENT_COM_RADIO_SET] = {
		.name = "COM_RADIO_SET",
		.flags = 1,
	},
	[EVENT_NAV1_RADIO_SET] = {
		.name = "NAV1_RADIO_SET",
		.flags = 1,
	},
	[EVENT_NAV2_RADIO_SET] = {
		.name = "NAV2_RADIO_SET",
		.flags = 1,
	},
	[EVENT_ADF_SET] = {
		.name = "ADF_SET",
		.flags = 1,
	},
	[EVENT_XPNDR_SET] = {
		.name = "XPNDR_SET",
		.flags = 1,
	},
	[EVENT_VOR1_SET] = {
		.name = "VOR1_SET",
		.flags = 1,
	},
	[EVENT_VOR2_SET] = {
		.name = "VOR2_SET",
		.flags = 1,
	},
	[EVENT_DME1_TOGGLE] = {
		.name = "DME1_TOGGLE",
		.flags = 1,
	},
	[EVENT_DME2_TOGGLE] = {
		.name = "DME2_TOGGLE",
		.flags = 1,
	},
	[EVENT_RADIO_VOR1_IDENT_DISABLE] = {
		.name = "RADIO_VOR1_IDENT_DISABLE",
		.flags = 1,
	},
	[EVENT_RADIO_VOR2_IDENT_DISABLE] = {
		.name = "RADIO_VOR2_IDENT_DISABLE",
		.flags = 1,
	},
	[EVENT_RADIO_DME1_IDENT_DISABLE] = {
		.name = "RADIO_DME1_IDENT_DISABLE",
		.flags = 1,
	},
	[EVENT_RADIO_DME2_IDENT_DISABLE] = {
		.name = "RADIO_DME2_IDENT_DISABLE",
		.flags = 1,
	},
	[EVENT_RADIO_ADF_IDENT_DISABLE] = {
		.name = "RADIO_ADF_IDENT_DISABLE",
		.flags = 1,
	},
	[EVENT_RADIO_VOR1_IDENT_ENABLE] = {
		.name = "RADIO_VOR1_IDENT_ENABLE",
		.flags = 1,
	},
	[EVENT_RADIO_VOR2_IDENT_ENABLE] = {
		.name = "RADIO_VOR2_IDENT_ENABLE",
		.flags = 1,
	},
	[EVENT_RADIO_DME1_IDENT_ENABLE] = {
		.name = "RADIO_DME1_IDENT_ENABLE",
		.flags = 1,
	},
	[EVENT_RADIO_DME2_IDENT_ENABLE] = {
		.name = "RADIO_DME2_IDENT_ENABLE",
		.flags = 1,
	},
	[EVENT_RADIO_ADF_IDENT_ENABLE] = {
		.name = "RADIO_ADF_IDENT_ENABLE",
		.flags = 1,
	},
	[EVENT_RADIO_VOR1_IDENT_TOGGLE] = {
		.name = "RADIO_VOR1_IDENT_TOGGLE",
		.flags = 1,
	},
	[EVENT_RADIO_VOR2_IDENT_TOGGLE] = {
		.name = "RADIO_VOR2_IDENT_TOGGLE",
		.flags = 1,
	},
	[EVENT_RADIO_DME1_IDENT_TOGGLE] = {
		.name = "RADIO_DME1_IDENT_TOGGLE",
		.flags = 1,
	},
	[EVENT_RADIO_DME2_IDENT_TOGGLE] = {
		.name = "RADIO_DME2_IDENT_TOGGLE",
		.flags = 1,
	},
	[EVENT_RADIO_ADF_IDENT_TOGGLE] = {
		.name = "RADIO_ADF_IDENT_TOGGLE",
		.flags = 1,
	},
	[EVENT_RADIO_VOR1_IDENT_SET] = {
		.name = "RADIO_VOR1_IDENT_SET",
		.flags = 1,
	},
	[EVENT_RADIO_VOR2_IDENT_SET] = {
		.name = "RADIO_VOR2_IDENT_SET",
		.flags = 1,
	},
	[EVENT_RADIO_DME1_IDENT_SET] = {
		.name = "RADIO_DME1_IDENT_SET",
		.flags = 1,
	},
	[EVENT_RADIO_DME2_IDENT_SET] = {
		.name = "RADIO_DME2_IDENT_SET",
		.flags = 1,
	},
	[EVENT_RADIO_ADF_IDENT_SET] = {
		.name = "RADIO_ADF_IDENT_SET",
		.flags = 1,
	},
	[EVENT_ADF_CARD_INC] = {
		.name = "ADF_CARD_INC",
		.flags = 1,
	},
	[EVENT_ADF_CARD_DEC] = {
		.name = "ADF_CARD_DEC",
		.flags = 1,
	},
	[EVENT_ADF_CARD_SET] = {
		.name = "ADF_CARD_SET",
		.flags = 1,
	},
	[EVENT_TOGGLE_DME] = {
		.name = "TOGGLE_DME",
		.flags = 1,
	},
	[EVENT_AVIONICS_MASTER_SET] = {
		.name = "AVIONICS_MASTER_SET",
		.flags = 1,
	},
	[EVENT_TOGGLE_AVIONICS_MASTER] = {
		.name = "TOGGLE_AVIONICS_MASTER",
		.flags = 1,
	},
	[EVENT_COM_STBY_RADIO_SET] = {
		.name = "COM_STBY_RADIO_SET",
		.flags = 1,
	},
	[EVENT_COM_STBY_RADIO_SWAP] = {
		.name = "COM_STBY_RADIO_SWAP",
		.flags = 1,
	},
	[EVENT_COM_RADIO_FRACT_DEC_CARRY] = {
		.name = "COM_RADIO_FRACT_DEC_CARRY",
		.flags = 1,
	},
	[EVENT_COM_RADIO_FRACT_INC_CARRY] = {
		.name = "COM_RADIO_FRACT_INC_CARRY",
		.flags = 1,
	},
	[EVENT_COM2_RADIO_WHOLE_DEC] = {
		.name = "COM2_RADIO_WHOLE_DEC",
		.flags = 1,
	},
	[EVENT_COM2_RADIO_WHOLE_INC] = {
		.name = "COM2_RADIO_WHOLE_INC",
		.flags = 1,
	},
	[EVENT_COM2_RADIO_FRACT_DEC] = {
		.name = "COM2_RADIO_FRACT_DEC",
		.flags = 1,
	},
	[EVENT_COM2_RADIO_FRACT_DEC_CARRY] = {
		.name = "COM2_RADIO_FRACT_DEC_CARRY",
		.flags = 1,
	},
	[EVENT_COM2_RADIO_FRACT_INC] = {
		.name = "COM2_RADIO_FRACT_INC",
		.flags = 1,
	},
	[EVENT_COM2_RADIO_FRACT_INC_CARRY] = {
		.name = "COM2_RADIO_FRACT_INC_CARRY",
		.flags = 1,
	},
	[EVENT_COM2_RADIO_SET] = {
		.name = "COM2_RADIO_SET",
		.flags = 1,
	},
	[EVENT_COM2_STBY_RADIO_SET] = {
		.name = "COM2_STBY_RADIO_SET",
		.flags = 1,
	},
	[EVENT_COM2_RADIO_SWAP] = {
		.name = "COM2_RADIO_SWAP",
		.flags = 1,
	},
	[EVENT_NAV1_RADIO_FRACT_DEC_CARRY] = {
		.name = "NAV1_RADIO_FRACT_DEC_CARRY",
		.flags = 1,
	},
	[EVENT_NAV1_RADIO_FRACT_INC_CARRY] = {
		.name = "NAV1_RADIO_FRACT_INC_CARRY",
		.flags = 1,
	},
	[EVENT_NAV1_STBY_SET] = {
		.name = "NAV1_STBY_SET",
		.flags = 1,
	},
	[EVENT_NAV1_RADIO_SWAP] = {
		.name = "NAV1_RADIO_SWAP",
		.flags = 1,
	},
	[EVENT_NAV2_RADIO_FRACT_DEC_CARRY] = {
		.name = "NAV2_RADIO_FRACT_DEC_CARRY",
		.flags = 1,
	},
	[EVENT_NAV2_RADIO_FRACT_INC_CARRY] = {
		.name = "NAV2_RADIO_FRACT_INC_CARRY",
		.flags = 1,
	},
	[EVENT_NAV2_STBY_SET] = {
		.name = "NAV2_STBY_SET",
		.flags = 1,
	},
	[EVENT_NAV2_RADIO_SWAP] = {
		.name = "NAV2_RADIO_SWAP",
		.flags = 1,
	},
	[EVENT_ADF1_RADIO_TENTHS_DEC] = {
		.name = "ADF1_RADIO_TENTHS_DEC",
		.flags = 1,
	},
	[EVENT_ADF1_RADIO_TENTHS_INC] = {
		.name = "ADF1_RADIO_TENTHS_INC",
		.flags = 1,
	},
	[EVENT_XPNDR_1000_DEC] = {
		.name = "XPNDR_1000_DEC",
		.flags = 1,
	},
	[EVENT_XPNDR_100_DEC] = {
		.name = "XPNDR_100_DEC",
		.flags = 1,
	},
	[EVENT_XPNDR_10_DEC] = {
		.name = "XPNDR_10_DEC",
		.flags = 1,
	},
	[EVENT_XPNDR_1_DEC] = {
		.name = "XPNDR_1_DEC",
		.flags = 1,
	},
	[EVENT_XPNDR_DEC_CARRY] = {
		.name = "XPNDR_DEC_CARRY",
		.flags = 1,
	},
	[EVENT_XPNDR_INC_CARRY] = {
		.name = "XPNDR_INC_CARRY",
		.flags = 1,
	},
	[EVENT_ADF_FRACT_DEC_CARRY] = {
		.name = "ADF_FRACT_DEC_CARRY",
		.flags = 1,
	},
	[EVENT_ADF_FRACT_INC_CARRY] = {
		.name = "ADF_FRACT_INC_CARRY",
		.flags = 1,
	},
	[EVENT_COM1_TRANSMIT_SELECT] = {
		.name = "COM1_TRANSMIT_SELECT",
		.flags = 1,
	},
	[EVENT_COM2_TRANSMIT_SELECT] = {
		.name = "COM2_TRANSMIT_SELECT",
		.flags = 1,
	},
	[EVENT_COM_RECEIVE_ALL_TOGGLE] = {
		.name = "COM_RECEIVE_ALL_TOGGLE",
		.flags = 1,
	},
	[EVENT_COM_RECEIVE_ALL_SET] = {
		.name = "COM_RECEIVE_ALL_SET",
		.flags = 1,
	},
	[EVENT_MARKER_SOUND_TOGGLE] = {
		.name = "MARKER_SOUND_TOGGLE",
		.flags = 1,
	},
	[EVENT_ADF_COMPLETE_SET] = {
		.name = "ADF_COMPLETE_SET",
		.flags = 1,
	},
	[EVENT_ADF1_WHOLE_INC] = {
		.name = "ADF1_WHOLE_INC",
		.flags = 1,
	},
	[EVENT_ADF1_WHOLE_DEC] = {
		.name = "ADF1_WHOLE_DEC",
		.flags = 1,
	},
	[EVENT_ADF2_100_INC] = {
		.name = "ADF2_100_INC",
		.flags = 1,
	},
	[EVENT_ADF2_10_INC] = {
		.name = "ADF2_10_INC",
		.flags = 1,
	},
	[EVENT_ADF2_1_INC] = {
		.name = "ADF2_1_INC",
		.flags = 1,
	},
	[EVENT_ADF2_RADIO_TENTHS_INC] = {
		.name = "ADF2_RADIO_TENTHS_INC",
		.flags = 1,
	},
	[EVENT_ADF2_100_DEC] = {
		.name = "ADF2_100_DEC",
		.flags = 1,
	},
	[EVENT_ADF2_10_DEC] = {
		.name = "ADF2_10_DEC",
		.flags = 1,
	},
	[EVENT_ADF2_1_DEC] = {
		.name = "ADF2_1_DEC",
		.flags = 1,
	},
	[EVENT_ADF2_RADIO_TENTHS_DEC] = {
		.name = "ADF2_RADIO_TENTHS_DEC",
		.flags = 1,
	},
	[EVENT_ADF2_WHOLE_INC] = {
		.name = "ADF2_WHOLE_INC",
		.flags = 1,
	},
	[EVENT_ADF2_WHOLE_DEC] = {
		.name = "ADF2_WHOLE_DEC",
		.flags = 1,
	},
	[EVENT_ADF2_FRACT_DEC_CARRY] = {
		.name = "ADF2_FRACT_DEC_CARRY",
		.flags = 1,
	},
	[EVENT_ADF2_FRACT_INC_CARRY] = {
		.name = "ADF2_FRACT_INC_CARRY",
		.flags = 1,
	},
	[EVENT_ADF2_COMPLETE_SET] = {
		.name = "ADF2_COMPLETE_SET",
		.flags = 1,
	},
	[EVENT_RADIO_ADF2_IDENT_DISABLE] = {
		.name = "RADIO_ADF2_IDENT_DISABLE",
		.flags = 1,
	},
	[EVENT_RADIO_ADF2_IDENT_ENABLE] = {
		.name = "RADIO_ADF2_IDENT_ENABLE",
		.flags = 1,
	},
	[EVENT_RADIO_ADF2_IDENT_TOGGLE] = {
		.name = "RADIO_ADF2_IDENT_TOGGLE",
		.flags = 1,
	},
	[EVENT_RADIO_ADF2_IDENT_SET] = {
		.name = "RADIO_ADF2_IDENT_SET",
		.flags = 1,
	},
	[EVENT_FREQUENCY_SWAP] = {
		.name = "FREQUENCY_SWAP",
		.flags = 1,
	},
	[EVENT_DME_SELECT] = {
		.name = "DME_SELECT",
		.flags = 1,
	},
	[EVENT_RADIO_SELECTED_DME_IDENT_ENABLE] = {
		.name = "RADIO_SELECTED_DME_IDENT_ENABLE",
		.flags = 1,
	},
	[EVENT_RADIO_SELECTED_DME_IDENT_DISABLE] = {
		.name = "RADIO_SELECTED_DME_IDENT_DISABLE",
		.flags = 1,
	},
	[EVENT_RADIO_SELECTED_DME_IDENT_SET] = {
		.name = "RADIO_SELECTED_DME_IDENT_SET",
		.flags = 1,
	},
	[EVENT_RADIO_SELECTED_DME_IDENT_TOGGLE] = {
		.name = "RADIO_SELECTED_DME_IDENT_TOGGLE",
		.flags = 1,
	},
};

const struct event_enum_data_t *get_event_enum_data(enum event_t value) {
	if ((value >= 0) && (value < EVENT_LAST)) {
		return &event_enum_data[value];
	} else {
		return NULL;
	}
}

const char *get_event_enum_name(enum event_t value) {
	const struct event_enum_data_t *data = get_event_enum_data(value);
	if (data) {
		return data->name;
	} else {
		return NULL;
	}
}
