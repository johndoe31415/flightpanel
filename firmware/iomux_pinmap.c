#include <stdio.h>
#include "iomux_pinmap.h"

static const struct iomux_pin_t input_pins[IOMUX_INPUT_COUNT] = {
	{
		.pinno = 27,
		.name = "Rotary_COM1_A",
	},
	{
		.pinno = 28,
		.name = "Rotary_COM1_B",
	},
	{
		.pinno = 23,
		.name = "Rotary_COM1_Button",
	},
	{
		.pinno = 22,
		.name = "Rotary_COM2_A",
	},
	{
		.pinno = 16,
		.name = "Rotary_COM2_B",
	},
	{
		.pinno = 17,
		.name = "Rotary_COM2_Button",
	},
	{
		.pinno = 11,
		.name = "Rotary_ATM_A",
	},
	{
		.pinno = 12,
		.name = "Rotary_ATM_B",
	},
	{
		.pinno = 7,
		.name = "Rotary_ATM_Button",
	},
	{
		.pinno = 36,
		.name = "Rotary_NAV1_A",
	},
	{
		.pinno = 34,
		.name = "Rotary_NAV1_B",
	},
	{
		.pinno = 35,
		.name = "Rotary_NAV1_Button",
	},
	{
		.pinno = 13,
		.name = "Rotary_NAV2_A",
	},
	{
		.pinno = 9,
		.name = "Rotary_NAV2_B",
	},
	{
		.pinno = 10,
		.name = "Rotary_NAV2_Button",
	},
	{
		.pinno = 33,
		.name = "Rotary_OBS_A",
	},
	{
		.pinno = 38,
		.name = "Rotary_OBS_B",
	},
	{
		.pinno = 37,
		.name = "Rotary_OBS_Button",
	},
	{
		.pinno = 8,
		.name = "Rotary_DME_ADF_A",
	},
	{
		.pinno = 15,
		.name = "Rotary_DME_ADF_B",
	},
	{
		.pinno = 14,
		.name = "Rotary_DME_ADF_Button",
	},
	{
		.pinno = 39,
		.name = "Rotary_AP_ALT_A",
	},
	{
		.pinno = 43,
		.name = "Rotary_AP_ALT_B",
	},
	{
		.pinno = 32,
		.name = "Rotary_AP_ALT_Button",
	},
	{
		.pinno = 42,
		.name = "Rotary_AP_HDG_A",
	},
	{
		.pinno = 45,
		.name = "Rotary_AP_HDG_B",
	},
	{
		.pinno = 44,
		.name = "Rotary_AP_HDG_Button",
	},
	{
		.pinno = 29,
		.name = "Rotary_AP_IAS_A",
	},
	{
		.pinno = 25,
		.name = "Rotary_AP_IAS_B",
	},
	{
		.pinno = 26,
		.name = "Rotary_AP_IAS_Button",
	},
	{
		.pinno = 24,
		.name = "Rotary_AP_RATE_A",
	},
	{
		.pinno = 31,
		.name = "Rotary_AP_RATE_B",
	},
	{
		.pinno = 30,
		.name = "Rotary_AP_RATE_Button",
	},
	{
		.pinno = 54,
		.name = "Button_Radio_COM1",
	},
	{
		.pinno = 48,
		.name = "Button_Radio_NAV1",
	},
	{
		.pinno = 55,
		.name = "Button_Radio_DME",
	},
	{
		.pinno = 50,
		.name = "Button_Radio_COM2",
	},
	{
		.pinno = 53,
		.name = "Button_Radio_NAV2",
	},
	{
		.pinno = 49,
		.name = "Button_Radio_ADF",
	},
	{
		.pinno = 40,
		.name = "Button_AP",
	},
	{
		.pinno = 47,
		.name = "Button_AP_NAV",
	},
	{
		.pinno = 51,
		.name = "Button_AP_APR",
	},
	{
		.pinno = 52,
		.name = "Button_AP_REV",
	},
	{
		.pinno = 41,
		.name = "Button_XPDR_Mode",
	},
	{
		.pinno = 46,
		.name = "Button_NavSrc",
	},
	{
		.pinno = 62,
		.name = "Button_XPDR_0",
	},
	{
		.pinno = 56,
		.name = "Button_XPDR_1",
	},
	{
		.pinno = 63,
		.name = "Button_XPDR_2",
	},
	{
		.pinno = 67,
		.name = "Button_XPDR_3",
	},
	{
		.pinno = 68,
		.name = "Button_XPDR_4",
	},
	{
		.pinno = 59,
		.name = "Button_XPDR_5",
	},
	{
		.pinno = 60,
		.name = "Button_XPDR_6",
	},
	{
		.pinno = 58,
		.name = "Button_XPDR_7",
	},
	{
		.pinno = 61,
		.name = "Button_XPDR_VFR",
	},
	{
		.pinno = 57,
		.name = "Button_XPDR_IDENT",
	},
	{
		.pinno = 71,
		.name = "Switch_BCN",
	},
	{
		.pinno = 64,
		.name = "Switch_LAND",
	},
	{
		.pinno = 70,
		.name = "Switch_TAXI",
	},
	{
		.pinno = 65,
		.name = "Switch_NAV",
	},
	{
		.pinno = 69,
		.name = "Switch_STRB",
	},
	{
		.pinno = 66,
		.name = "Switch_MASTER",
	},
};

static const struct iomux_pin_t output_pins[IOMUX_OUTPUT_COUNT] = {
	{
		.pinno = 58,
		.name = "Radio_ADF",
	},
	{
		.pinno = 59,
		.name = "Radio_NAV2",
	},
	{
		.pinno = 60,
		.name = "Radio_COM2",
	},
	{
		.pinno = 61,
		.name = "Radio_DME",
	},
	{
		.pinno = 62,
		.name = "Radio_NAV1",
	},
	{
		.pinno = 63,
		.name = "Radio_COM1",
	},
	{
		.pinno = 64,
		.name = "NavSrc_GPS",
	},
	{
		.pinno = 65,
		.name = "NavSrc_NAV",
	},
	{
		.pinno = 66,
		.name = "XPDR_C",
	},
	{
		.pinno = 67,
		.name = "XPDR_STBY",
	},
	{
		.pinno = 68,
		.name = "AP_IAS",
	},
	{
		.pinno = 69,
		.name = "AP_HDG",
	},
	{
		.pinno = 70,
		.name = "AP_ALT",
	},
	{
		.pinno = 71,
		.name = "AP_MASTER",
	},
};

const struct iomux_pin_t *iomux_get_input_description(int pin_no) {
	for (int i = 0; i < IOMUX_INPUT_COUNT; i++) {
		if (input_pins[i].pinno == pin_no) {
			return &input_pins[i];
		}
	}
	return NULL;
}

const struct iomux_pin_t *iomux_get_output_description(int pin_no) {
	for (int i = 0; i < IOMUX_OUTPUT_COUNT; i++) {
		if (output_pins[i].pinno == pin_no) {
			return &output_pins[i];
		}
	}
	return NULL;
}

