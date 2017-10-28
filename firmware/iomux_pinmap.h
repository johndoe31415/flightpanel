#ifndef __IOMUX_PINMAP_H__
#define __IOMUX_PINMAP_H__

struct iomux_pin_t {
	int pinno;
	const char *name;
};

#define IOMUX_MULTIPLEXER_COUNT	9
#define IOMUX_IO_COUNT	(IOMUX_MULTIPLEXER_COUNT * 8)	// 72

// 61 inputs (11 free). ID range 0 - 71:
#define IOMUX_INPUT_COUNT	61
#define IOMUX_IN_Rotary_COM1_A		27
#define IOMUX_IN_Rotary_COM1_B		28
#define IOMUX_IN_Rotary_COM1_Button		23
#define IOMUX_IN_Rotary_COM2_A		22
#define IOMUX_IN_Rotary_COM2_B		16
#define IOMUX_IN_Rotary_COM2_Button		17
#define IOMUX_IN_Rotary_ATM_A		13
#define IOMUX_IN_Rotary_ATM_B		10
#define IOMUX_IN_Rotary_ATM_Button		9
#define IOMUX_IN_Rotary_NAV1_A		36
#define IOMUX_IN_Rotary_NAV1_B		34
#define IOMUX_IN_Rotary_NAV1_Button		35
#define IOMUX_IN_Rotary_NAV2_A		6
#define IOMUX_IN_Rotary_NAV2_B		0
#define IOMUX_IN_Rotary_NAV2_Button		1
#define IOMUX_IN_Rotary_OBS_A		33
#define IOMUX_IN_Rotary_OBS_B		38
#define IOMUX_IN_Rotary_OBS_Button		37
#define IOMUX_IN_Rotary_DME_ADF_A		11
#define IOMUX_IN_Rotary_DME_ADF_B		12
#define IOMUX_IN_Rotary_DME_ADF_Button		7
#define IOMUX_IN_Rotary_AP_ALT_A		39
#define IOMUX_IN_Rotary_AP_ALT_B		43
#define IOMUX_IN_Rotary_AP_ALT_Button		32
#define IOMUX_IN_Rotary_AP_HDG_A		42
#define IOMUX_IN_Rotary_AP_HDG_B		45
#define IOMUX_IN_Rotary_AP_HDG_Button		44
#define IOMUX_IN_Rotary_AP_IAS_A		29
#define IOMUX_IN_Rotary_AP_IAS_B		25
#define IOMUX_IN_Rotary_AP_IAS_Button		26
#define IOMUX_IN_Rotary_AP_RATE_A		24
#define IOMUX_IN_Rotary_AP_RATE_B		31
#define IOMUX_IN_Rotary_AP_RATE_Button		30
#define IOMUX_IN_Button_Radio_COM1		54
#define IOMUX_IN_Button_Radio_NAV1		48
#define IOMUX_IN_Button_Radio_DME		55
#define IOMUX_IN_Button_Radio_COM2		50
#define IOMUX_IN_Button_Radio_NAV2		53
#define IOMUX_IN_Button_Radio_ADF		49
#define IOMUX_IN_Button_AP		40
#define IOMUX_IN_Button_AP_NAV		47
#define IOMUX_IN_Button_AP_APR		51
#define IOMUX_IN_Button_AP_REV		52
#define IOMUX_IN_Button_XPDR_Mode		41
#define IOMUX_IN_Button_NavSrc		46
#define IOMUX_IN_Button_XPDR_0		62
#define IOMUX_IN_Button_XPDR_1		56
#define IOMUX_IN_Button_XPDR_2		63
#define IOMUX_IN_Button_XPDR_3		67
#define IOMUX_IN_Button_XPDR_4		68
#define IOMUX_IN_Button_XPDR_5		59
#define IOMUX_IN_Button_XPDR_6		60
#define IOMUX_IN_Button_XPDR_7		58
#define IOMUX_IN_Button_XPDR_VFR		61
#define IOMUX_IN_Button_XPDR_IDENT		57
#define IOMUX_IN_Switch_BCN		71
#define IOMUX_IN_Switch_LAND		64
#define IOMUX_IN_Switch_TAXI		70
#define IOMUX_IN_Switch_NAV		65
#define IOMUX_IN_Switch_STRB		69
#define IOMUX_IN_Switch_MASTER		66


// 19 outputs (53 free). ID range 48 - 71:
#define IOMUX_OUTPUT_COUNT	19
#define IOMUX_OUT_Radio_ADF		58
#define IOMUX_OUT_Radio_NAV2		59
#define IOMUX_OUT_Radio_COM2		60
#define IOMUX_OUT_Radio_DME		61
#define IOMUX_OUT_Radio_NAV1		62
#define IOMUX_OUT_Radio_COM1		63
#define IOMUX_OUT_NavSrc_GPS		64
#define IOMUX_OUT_NavSrc_NAV		65
#define IOMUX_OUT_XPDR_C		66
#define IOMUX_OUT_XPDR_STBY		67
#define IOMUX_OUT_AP_ALT_Green		52
#define IOMUX_OUT_AP_ALT_Red		53
#define IOMUX_OUT_AP_HDG_Green		50
#define IOMUX_OUT_AP_HDG_Red		51
#define IOMUX_OUT_AP_IAS_Green		54
#define IOMUX_OUT_AP_IAS_Red		55
#define IOMUX_OUT_AP_RATE_Green		48
#define IOMUX_OUT_AP_RATE_Red		49
#define IOMUX_OUT_AP_MASTER		71

const struct iomux_pin_t *iomux_get_input_description(int pin_no);
const struct iomux_pin_t *iomux_get_output_description(int pin_no);

#endif
