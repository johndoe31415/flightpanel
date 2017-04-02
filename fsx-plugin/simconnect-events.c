#include <stdio.h>
#include <stdint.h>
#include "simconnect-events.h"

static const struct simconnect_event_enum_data_t event_enum_data[] = {
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
	[EVENT_YAW_DAMPER_TOGGLE] = {
		.name = "YAW_DAMPER_TOGGLE",
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
	[EVENT_AP_ATT_HOLD_ON] = {
		.name = "AP_ATT_HOLD_ON",
		.flags = 1,
	},
	[EVENT_AP_LOC_HOLD_ON] = {
		.name = "AP_LOC_HOLD_ON",
		.flags = 1,
	},
	[EVENT_AP_APR_HOLD_ON] = {
		.name = "AP_APR_HOLD_ON",
		.flags = 1,
	},
	[EVENT_AP_HDG_HOLD_ON] = {
		.name = "AP_HDG_HOLD_ON",
		.flags = 1,
	},
	[EVENT_AP_ALT_HOLD_ON] = {
		.name = "AP_ALT_HOLD_ON",
		.flags = 1,
	},
	[EVENT_AP_WING_LEVELER_ON] = {
		.name = "AP_WING_LEVELER_ON",
		.flags = 1,
	},
	[EVENT_AP_BC_HOLD_ON] = {
		.name = "AP_BC_HOLD_ON",
		.flags = 1,
	},
	[EVENT_AP_NAV1_HOLD_ON] = {
		.name = "AP_NAV1_HOLD_ON",
		.flags = 1,
	},
	[EVENT_AP_ATT_HOLD_OFF] = {
		.name = "AP_ATT_HOLD_OFF",
		.flags = 1,
	},
	[EVENT_AP_LOC_HOLD_OFF] = {
		.name = "AP_LOC_HOLD_OFF",
		.flags = 1,
	},
	[EVENT_AP_APR_HOLD_OFF] = {
		.name = "AP_APR_HOLD_OFF",
		.flags = 1,
	},
	[EVENT_AP_HDG_HOLD_OFF] = {
		.name = "AP_HDG_HOLD_OFF",
		.flags = 1,
	},
	[EVENT_AP_ALT_HOLD_OFF] = {
		.name = "AP_ALT_HOLD_OFF",
		.flags = 1,
	},
	[EVENT_AP_WING_LEVELER_OFF] = {
		.name = "AP_WING_LEVELER_OFF",
		.flags = 1,
	},
	[EVENT_AP_BC_HOLD_OFF] = {
		.name = "AP_BC_HOLD_OFF",
		.flags = 1,
	},
	[EVENT_AP_NAV1_HOLD_OFF] = {
		.name = "AP_NAV1_HOLD_OFF",
		.flags = 1,
	},
	[EVENT_AP_AIRSPEED_HOLD] = {
		.name = "AP_AIRSPEED_HOLD",
		.flags = 1,
	},
	[EVENT_AUTO_THROTTLE_ARM] = {
		.name = "AUTO_THROTTLE_ARM",
		.flags = 1,
	},
	[EVENT_AUTO_THROTTLE_TO_GA] = {
		.name = "AUTO_THROTTLE_TO_GA",
		.flags = 1,
	},
	[EVENT_HEADING_BUG_INC] = {
		.name = "HEADING_BUG_INC",
		.flags = 1,
	},
	[EVENT_HEADING_BUG_DEC] = {
		.name = "HEADING_BUG_DEC",
		.flags = 1,
	},
	[EVENT_HEADING_BUG_SET] = {
		.name = "HEADING_BUG_SET",
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
	[EVENT_AP_VS_VAR_SET_ENGLISH] = {
		.name = "AP_VS_VAR_SET_ENGLISH",
		.flags = 1,
	},
	[EVENT_AP_SPD_VAR_SET] = {
		.name = "AP_SPD_VAR_SET",
		.flags = 1,
	},
	[EVENT_YAW_DAMPER_ON] = {
		.name = "YAW_DAMPER_ON",
		.flags = 1,
	},
	[EVENT_YAW_DAMPER_OFF] = {
		.name = "YAW_DAMPER_OFF",
		.flags = 1,
	},
	[EVENT_YAW_DAMPER_SET] = {
		.name = "YAW_DAMPER_SET",
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
	[EVENT_AP_AIRSPEED_SET] = {
		.name = "AP_AIRSPEED_SET",
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
	[EVENT_AP_PANEL_ALTITUDE_SET] = {
		.name = "AP_PANEL_ALTITUDE_SET",
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
	[EVENT_AP_PANEL_HEADING_SET] = {
		.name = "AP_PANEL_HEADING_SET",
		.flags = 1,
	},
	[EVENT_AP_PANEL_SPEED_ON] = {
		.name = "AP_PANEL_SPEED_ON",
		.flags = 1,
	},
	[EVENT_AP_PANEL_SPEED_OFF] = {
		.name = "AP_PANEL_SPEED_OFF",
		.flags = 1,
	},
	[EVENT_AP_PANEL_SPEED_SET] = {
		.name = "AP_PANEL_SPEED_SET",
		.flags = 1,
	},
	[EVENT_AP_ALT_VAR_SET_ENGLISH] = {
		.name = "AP_ALT_VAR_SET_ENGLISH",
		.flags = 1,
	},
	[EVENT_AP_PANEL_SPEED_HOLD_TOGGLE] = {
		.name = "AP_PANEL_SPEED_HOLD_TOGGLE",
		.flags = 1,
	},
	[EVENT_AP_NAV_SELECT_SET] = {
		.name = "AP_NAV_SELECT_SET",
		.flags = 1,
	},
	[EVENT_HEADING_BUG_SELECT] = {
		.name = "HEADING_BUG_SELECT",
		.flags = 1,
	},
	[EVENT_ALTITUDE_BUG_SELECT] = {
		.name = "ALTITUDE_BUG_SELECT",
		.flags = 1,
	},
	[EVENT_VSI_BUG_SELECT] = {
		.name = "VSI_BUG_SELECT",
		.flags = 1,
	},
	[EVENT_AIRSPEED_BUG_SELECT] = {
		.name = "AIRSPEED_BUG_SELECT",
		.flags = 1,
	},
	[EVENT_AP_PITCH_REF_INC_UP] = {
		.name = "AP_PITCH_REF_INC_UP",
		.flags = 1,
	},
	[EVENT_AP_PITCH_REF_INC_DN] = {
		.name = "AP_PITCH_REF_INC_DN",
		.flags = 1,
	},
	[EVENT_AP_PITCH_REF_SELECT] = {
		.name = "AP_PITCH_REF_SELECT",
		.flags = 1,
	},
	[EVENT_AP_ATT_HOLD] = {
		.name = "AP_ATT_HOLD",
		.flags = 1,
	},
	[EVENT_AP_LOC_HOLD] = {
		.name = "AP_LOC_HOLD",
		.flags = 1,
	},
	[EVENT_AP_APR_HOLD] = {
		.name = "AP_APR_HOLD",
		.flags = 1,
	},
	[EVENT_AP_HDG_HOLD] = {
		.name = "AP_HDG_HOLD",
		.flags = 1,
	},
	[EVENT_AP_ALT_HOLD] = {
		.name = "AP_ALT_HOLD",
		.flags = 1,
	},
	[EVENT_AP_WING_LEVELER] = {
		.name = "AP_WING_LEVELER",
		.flags = 1,
	},
	[EVENT_AP_BC_HOLD] = {
		.name = "AP_BC_HOLD",
		.flags = 1,
	},
	[EVENT_AP_NAV1_HOLD] = {
		.name = "AP_NAV1_HOLD",
		.flags = 1,
	},
	[EVENT_AP_MAX_BANK_INC] = {
		.name = "AP_MAX_BANK_INC",
		.flags = 1,
	},
	[EVENT_AP_MAX_BANK_DEC] = {
		.name = "AP_MAX_BANK_DEC",
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
	[EVENT_STROBES_TOGGLE] = {
		.name = "STROBES_TOGGLE",
		.flags = 1,
	},
	[EVENT_ALL_LIGHTS_TOGGLE] = {
		.name = "ALL_LIGHTS_TOGGLE",
		.flags = 1,
	},
	[EVENT_PANEL_LIGHTS_TOGGLE] = {
		.name = "PANEL_LIGHTS_TOGGLE",
		.flags = 1,
	},
	[EVENT_LANDING_LIGHTS_TOGGLE] = {
		.name = "LANDING_LIGHTS_TOGGLE",
		.flags = 1,
	},
	[EVENT_LANDING_LIGHT_UP] = {
		.name = "LANDING_LIGHT_UP",
		.flags = 1,
	},
	[EVENT_LANDING_LIGHT_DOWN] = {
		.name = "LANDING_LIGHT_DOWN",
		.flags = 1,
	},
	[EVENT_LANDING_LIGHT_LEFT] = {
		.name = "LANDING_LIGHT_LEFT",
		.flags = 1,
	},
	[EVENT_LANDING_LIGHT_RIGHT] = {
		.name = "LANDING_LIGHT_RIGHT",
		.flags = 1,
	},
	[EVENT_LANDING_LIGHT_HOME] = {
		.name = "LANDING_LIGHT_HOME",
		.flags = 1,
	},
	[EVENT_STROBES_ON] = {
		.name = "STROBES_ON",
		.flags = 1,
	},
	[EVENT_STROBES_OFF] = {
		.name = "STROBES_OFF",
		.flags = 1,
	},
	[EVENT_STROBES_SET] = {
		.name = "STROBES_SET",
		.flags = 1,
	},
	[EVENT_PANEL_LIGHTS_ON] = {
		.name = "PANEL_LIGHTS_ON",
		.flags = 1,
	},
	[EVENT_PANEL_LIGHTS_OFF] = {
		.name = "PANEL_LIGHTS_OFF",
		.flags = 1,
	},
	[EVENT_PANEL_LIGHTS_SET] = {
		.name = "PANEL_LIGHTS_SET",
		.flags = 1,
	},
	[EVENT_LANDING_LIGHTS_ON] = {
		.name = "LANDING_LIGHTS_ON",
		.flags = 1,
	},
	[EVENT_LANDING_LIGHTS_OFF] = {
		.name = "LANDING_LIGHTS_OFF",
		.flags = 1,
	},
	[EVENT_LANDING_LIGHTS_SET] = {
		.name = "LANDING_LIGHTS_SET",
		.flags = 1,
	},
	[EVENT_TOGGLE_BEACON_LIGHTS] = {
		.name = "TOGGLE_BEACON_LIGHTS",
		.flags = 1,
	},
	[EVENT_TOGGLE_TAXI_LIGHTS] = {
		.name = "TOGGLE_TAXI_LIGHTS",
		.flags = 1,
	},
	[EVENT_TOGGLE_LOGO_LIGHTS] = {
		.name = "TOGGLE_LOGO_LIGHTS",
		.flags = 1,
	},
	[EVENT_TOGGLE_RECOGNITION_LIGHTS] = {
		.name = "TOGGLE_RECOGNITION_LIGHTS",
		.flags = 1,
	},
	[EVENT_TOGGLE_WING_LIGHTS] = {
		.name = "TOGGLE_WING_LIGHTS",
		.flags = 1,
	},
	[EVENT_TOGGLE_NAV_LIGHTS] = {
		.name = "TOGGLE_NAV_LIGHTS",
		.flags = 1,
	},
	[EVENT_TOGGLE_CABIN_LIGHTS] = {
		.name = "TOGGLE_CABIN_LIGHTS",
		.flags = 1,
	},
	[EVENT_KOHLSMAN_INC] = {
		.name = "KOHLSMAN_INC",
		.flags = 1,
	},
	[EVENT_KOHLSMAN_DEC] = {
		.name = "KOHLSMAN_DEC",
		.flags = 1,
	},
	[EVENT_KOHLSMAN_SET] = {
		.name = "KOHLSMAN_SET",
		.flags = 1,
	},
	[EVENT_TOGGLE_GPS_DRIVES_NAV1] = {
		.name = "TOGGLE_GPS_DRIVES_NAV1",
		.flags = 1,
	},
};

const struct simconnect_event_enum_data_t *get_simconnect_event_enum_data(enum simconnect_event_t value) {
	if ((value >= 0) && (value < EVENT_LAST)) {
		return &event_enum_data[value];
	} else {
		return NULL;
	}
}

const char *get_simconnect_event_enum_name(enum simconnect_event_t value) {
	const struct simconnect_event_enum_data_t *data = get_simconnect_event_enum_data(value);
	if (data) {
		return data->name;
	} else {
		return NULL;
	}
}
