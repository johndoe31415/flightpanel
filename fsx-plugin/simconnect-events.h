#ifndef __SIMCONNECT_EVENTS_H__
#define __SIMCONNECT_EVENTS_H__

#include <stdint.h>

struct simconnect_event_enum_data_t {
	uint32_t flags;
	const char *name;
};

enum simconnect_event_t {
	EVENT_SIM_START,			// Start simulation
	EVENT_AP_MASTER,			// Toggles AP on/off
	EVENT_AUTOPILOT_OFF,			// Turns AP off
	EVENT_AUTOPILOT_ON,			// Turns AP on
	EVENT_YAW_DAMPER_TOGGLE,			// Toggles yaw damper on/off
	EVENT_AP_PANEL_HEADING_HOLD,			// Toggles heading hold mode on/off
	EVENT_AP_PANEL_ALTITUDE_HOLD,			// Toggles altitude hold mode on/off
	EVENT_AP_ATT_HOLD_ON,			// Turns on AP wing leveler and pitch hold mode
	EVENT_AP_LOC_HOLD_ON,			// Turns AP localizer hold on/armed and glide-slope hold mode off
	EVENT_AP_APR_HOLD_ON,			// Turns both AP localizer and glide-slope modes on/armed
	EVENT_AP_HDG_HOLD_ON,			// Turns heading hold mode on
	EVENT_AP_ALT_HOLD_ON,			// Turns altitude hold mode on
	EVENT_AP_WING_LEVELER_ON,			// Turns wing leveler mode on
	EVENT_AP_BC_HOLD_ON,			// Turns localizer back course hold mode on/armed
	EVENT_AP_NAV1_HOLD_ON,			// Turns lateral hold mode on
	EVENT_AP_ATT_HOLD_OFF,			// Turns off attitude hold mode
	EVENT_AP_LOC_HOLD_OFF,			// Turns off localizer hold mode
	EVENT_AP_APR_HOLD_OFF,			// Turns off approach hold mode
	EVENT_AP_HDG_HOLD_OFF,			// Turns off heading hold mode
	EVENT_AP_ALT_HOLD_OFF,			// Turns off altitude hold mode
	EVENT_AP_WING_LEVELER_OFF,			// Turns off wing leveler mode
	EVENT_AP_BC_HOLD_OFF,			// Turns off backcourse mode for localizer hold
	EVENT_AP_NAV1_HOLD_OFF,			// Turns off nav hold mode
	EVENT_AP_AIRSPEED_HOLD,			// Toggles airspeed hold mode
	EVENT_AUTO_THROTTLE_ARM,			// Toggles autothrottle arming mode
	EVENT_AUTO_THROTTLE_TO_GA,			// Toggles Takeoff/Go Around mode
	EVENT_HEADING_BUG_INC,			// Increments heading hold reference bug
	EVENT_HEADING_BUG_DEC,			// Decrements heading hold reference bug
	EVENT_HEADING_BUG_SET,			// Set heading hold reference bug (degrees)
	EVENT_AP_PANEL_SPEED_HOLD,			// Toggles airspeed hold mode
	EVENT_AP_ALT_VAR_INC,			// Increments reference altitude
	EVENT_AP_ALT_VAR_DEC,			// Decrements reference altitude
	EVENT_AP_VS_VAR_INC,			// Increments vertical speed reference
	EVENT_AP_VS_VAR_DEC,			// Decrements vertical speed reference
	EVENT_AP_SPD_VAR_INC,			// Increments airspeed hold reference
	EVENT_AP_SPD_VAR_DEC,			// Decrements airspeed hold reference
	EVENT_AP_VS_VAR_SET_ENGLISH,			// Sets reference vertical speed in feet per minute
	EVENT_AP_SPD_VAR_SET,			// Sets airspeed reference in knots
	EVENT_YAW_DAMPER_ON,			// Turns yaw damper on
	EVENT_YAW_DAMPER_OFF,			// Turns yaw damper off
	EVENT_YAW_DAMPER_SET,			// Sets yaw damper on/off (1,0)
	EVENT_AP_AIRSPEED_ON,			// Turns airspeed hold on
	EVENT_AP_AIRSPEED_OFF,			// Turns airspeed hold off
	EVENT_AP_AIRSPEED_SET,			// Sets airspeed hold on/off (1,0)
	EVENT_AP_PANEL_ALTITUDE_ON,			// Turns altitude hold mode on (without capturing current altitude)
	EVENT_AP_PANEL_ALTITUDE_OFF,			// Turns altitude hold mode off
	EVENT_AP_PANEL_ALTITUDE_SET,			// Sets altitude hold mode on/off (1,0)
	EVENT_AP_PANEL_HEADING_ON,			// Turns heading mode on (without capturing current heading)
	EVENT_AP_PANEL_HEADING_OFF,			// Turns heading mode off
	EVENT_AP_PANEL_HEADING_SET,			// Set heading mode on/off (1,0)
	EVENT_AP_PANEL_SPEED_ON,			// Turns on speed hold mode
	EVENT_AP_PANEL_SPEED_OFF,			// Turns off speed hold mode
	EVENT_AP_PANEL_SPEED_SET,			// Set speed hold mode on/off (1,0)
	EVENT_AP_ALT_VAR_SET_ENGLISH,			// Sets altitude reference in feet
	EVENT_AP_PANEL_SPEED_HOLD_TOGGLE,			// Turns airspeed hold mode on with current airspeed
	EVENT_AP_NAV_SELECT_SET,			// Sets the nav (1 or 2) which is used by the Nav hold modes
	EVENT_HEADING_BUG_SELECT,			// Selects the heading bug for use with +/-
	EVENT_ALTITUDE_BUG_SELECT,			// Selects the altitude reference for use with +/-
	EVENT_VSI_BUG_SELECT,			// Selects the vertical speed reference for use with +/-
	EVENT_AIRSPEED_BUG_SELECT,			// Selects the airspeed reference for use with +/-
	EVENT_AP_PITCH_REF_INC_UP,			// Increments the pitch reference for pitch hold mode
	EVENT_AP_PITCH_REF_INC_DN,			// Decrements the pitch reference for pitch hold mode
	EVENT_AP_PITCH_REF_SELECT,			// Selects pitch reference for use with +/-
	EVENT_AP_ATT_HOLD,			// Toggle attitude hold mode
	EVENT_AP_LOC_HOLD,			// Toggles localizer (only) hold mode
	EVENT_AP_APR_HOLD,			// Toggles approach hold (localizer and glide-slope)
	EVENT_AP_HDG_HOLD,			// Toggles heading hold mode
	EVENT_AP_ALT_HOLD,			// Toggles altitude hold mode
	EVENT_AP_WING_LEVELER,			// Toggles wing leveler mode
	EVENT_AP_BC_HOLD,			// Toggles the backcourse mode for the localizer hold
	EVENT_AP_NAV1_HOLD,			// Toggles the nav hold mode
	EVENT_AP_MAX_BANK_INC,			// Autopilot max bank angle increment.
	EVENT_AP_MAX_BANK_DEC,			// Autopilot max bank angle decrement.
	EVENT_COM_RADIO_WHOLE_DEC,			// Decrements COM by one MHz
	EVENT_COM_RADIO_WHOLE_INC,			// Increments COM by one MHz
	EVENT_COM_RADIO_FRACT_DEC,			// Decrements COM by 25 KHz
	EVENT_COM_RADIO_FRACT_INC,			// Increments COM by 25 KHz
	EVENT_NAV1_RADIO_WHOLE_DEC,			// Decrements Nav 1 by one MHz
	EVENT_NAV1_RADIO_WHOLE_INC,			// Increments Nav 1 by one MHz
	EVENT_NAV1_RADIO_FRACT_DEC,			// Decrements Nav 1 by 25 KHz
	EVENT_NAV1_RADIO_FRACT_INC,			// Increments Nav 1 by 25 KHz
	EVENT_NAV2_RADIO_WHOLE_DEC,			// Decrements Nav 2 by one MHz
	EVENT_NAV2_RADIO_WHOLE_INC,			// Increments Nav 2 by one MHz
	EVENT_NAV2_RADIO_FRACT_DEC,			// Decrements Nav 2 by 25 KHz
	EVENT_NAV2_RADIO_FRACT_INC,			// Increments Nav 2 by 25 KHz
	EVENT_ADF_100_INC,			// Increments ADF by 100 KHz
	EVENT_ADF_10_INC,			// Increments ADF by 10 KHz
	EVENT_ADF_1_INC,			// Increments ADF by 1 KHz
	EVENT_XPNDR_1000_INC,			// Increments first digit of transponder
	EVENT_XPNDR_100_INC,			// Increments second digit of transponder
	EVENT_XPNDR_10_INC,			// Increments third digit of transponder
	EVENT_XPNDR_1_INC,			// Increments fourth digit of transponder
	EVENT_VOR1_OBI_DEC,			// Decrements the VOR 1 OBS setting
	EVENT_VOR1_OBI_INC,			// Increments the VOR 1 OBS setting
	EVENT_VOR2_OBI_DEC,			// Decrements the VOR 2 OBS setting
	EVENT_VOR2_OBI_INC,			// Increments the VOR 2 OBS setting
	EVENT_ADF_100_DEC,			// Decrements ADF by 100 KHz
	EVENT_ADF_10_DEC,			// Decrements ADF by 10 KHz
	EVENT_ADF_1_DEC,			// Decrements ADF by 1 KHz
	EVENT_COM_RADIO_SET,			// Sets COM frequency (BCD Hz)
	EVENT_NAV1_RADIO_SET,			// Sets NAV 1 frequency (BCD Hz)
	EVENT_NAV2_RADIO_SET,			// Sets NAV 2 frequency (BCD Hz)
	EVENT_ADF_SET,			// Sets ADF frequency (BCD Hz)
	EVENT_XPNDR_SET,			// Sets transponder code (BCD)
	EVENT_VOR1_SET,			// Sets OBS 1 (0 to 360)
	EVENT_VOR2_SET,			// Sets OBS 2 (0 to 360)
	EVENT_DME1_TOGGLE,			// Sets DME display to Nav 1
	EVENT_DME2_TOGGLE,			// Sets DME display to Nav 2
	EVENT_RADIO_VOR1_IDENT_DISABLE,			// Turns NAV 1 ID off
	EVENT_RADIO_VOR2_IDENT_DISABLE,			// Turns NAV 2 ID off
	EVENT_RADIO_DME1_IDENT_DISABLE,			// Turns DME 1 ID off
	EVENT_RADIO_DME2_IDENT_DISABLE,			// Turns DME 2 ID off
	EVENT_RADIO_ADF_IDENT_DISABLE,			// Turns ADF 1 ID off
	EVENT_RADIO_VOR1_IDENT_ENABLE,			// Turns NAV 1 ID on
	EVENT_RADIO_VOR2_IDENT_ENABLE,			// Turns NAV 2 ID on
	EVENT_RADIO_DME1_IDENT_ENABLE,			// Turns DME 1 ID on
	EVENT_RADIO_DME2_IDENT_ENABLE,			// Turns DME 2 ID on
	EVENT_RADIO_ADF_IDENT_ENABLE,			// Turns ADF 1 ID on
	EVENT_RADIO_VOR1_IDENT_TOGGLE,			// Toggles NAV 1 ID
	EVENT_RADIO_VOR2_IDENT_TOGGLE,			// Toggles NAV 2 ID
	EVENT_RADIO_DME1_IDENT_TOGGLE,			// Toggles DME 1 ID
	EVENT_RADIO_DME2_IDENT_TOGGLE,			// Toggles DME 2 ID
	EVENT_RADIO_ADF_IDENT_TOGGLE,			// Toggles ADF 1 ID
	EVENT_RADIO_VOR1_IDENT_SET,			// Sets NAV 1 ID (on/off)
	EVENT_RADIO_VOR2_IDENT_SET,			// Sets NAV 2 ID (on/off)
	EVENT_RADIO_DME1_IDENT_SET,			// Sets DME 1 ID (on/off)
	EVENT_RADIO_DME2_IDENT_SET,			// Sets DME 2 ID (on/off)
	EVENT_RADIO_ADF_IDENT_SET,			// Sets ADF 1 ID (on/off)
	EVENT_ADF_CARD_INC,			// Increments ADF card
	EVENT_ADF_CARD_DEC,			// Decrements ADF card
	EVENT_ADF_CARD_SET,			// Sets ADF card (0-360)
	EVENT_TOGGLE_DME,			// Toggles between NAV 1 and NAV 2
	EVENT_AVIONICS_MASTER_SET,			// Sets the avionics master switch
	EVENT_TOGGLE_AVIONICS_MASTER,			// Toggles the avionics master switch
	EVENT_COM_STBY_RADIO_SET,			// Sets COM 1 standby frequency (BCD Hz)
	EVENT_COM_STBY_RADIO_SWAP,			// Swaps COM 1 frequency with standby
	EVENT_COM_RADIO_FRACT_DEC_CARRY,			// Decrement COM 1 frequency by 25 KHz, and carry when digit wraps
	EVENT_COM_RADIO_FRACT_INC_CARRY,			// Increment COM 1 frequency by 25 KHz, and carry when digit wraps
	EVENT_COM2_RADIO_WHOLE_DEC,			// Decrement COM 2 frequency by 1 MHz, with no carry when digit wraps
	EVENT_COM2_RADIO_WHOLE_INC,			// Increment COM 2 frequency by 1 MHz, with no carry when digit wraps
	EVENT_COM2_RADIO_FRACT_DEC,			// Decrement COM 2 frequency by 25 KHz, with no carry when digit wraps
	EVENT_COM2_RADIO_FRACT_DEC_CARRY,			// Decrement COM 2 frequency by 25 KHz, and carry when digit wraps
	EVENT_COM2_RADIO_FRACT_INC,			// Increment COM 2 frequency by 25 KHz, with no carry when digit wraps
	EVENT_COM2_RADIO_FRACT_INC_CARRY,			// Increment COM 2 frequency by 25 KHz, and carry when digit wraps
	EVENT_COM2_RADIO_SET,			// Sets COM 2 frequency (BCD Hz)
	EVENT_COM2_STBY_RADIO_SET,			// Sets COM 2 standby frequency (BCD Hz)
	EVENT_COM2_RADIO_SWAP,			// Swaps COM 2 frequency with standby
	EVENT_NAV1_RADIO_FRACT_DEC_CARRY,			// Decrement NAV 1 frequency by 50 KHz, and carry when digit wraps
	EVENT_NAV1_RADIO_FRACT_INC_CARRY,			// Increment NAV 1 frequency by 50 KHz, and carry when digit wraps
	EVENT_NAV1_STBY_SET,			// Sets NAV 1 standby frequency (BCD Hz)
	EVENT_NAV1_RADIO_SWAP,			// Swaps NAV 1 frequency with standby
	EVENT_NAV2_RADIO_FRACT_DEC_CARRY,			// Decrement NAV 2 frequency by 50 KHz, and carry when digit wraps
	EVENT_NAV2_RADIO_FRACT_INC_CARRY,			// Increment NAV 2 frequency by 50 KHz, and carry when digit wraps
	EVENT_NAV2_STBY_SET,			// Sets NAV 2 standby frequency (BCD Hz)
	EVENT_NAV2_RADIO_SWAP,			// Swaps NAV 2 frequency with standby
	EVENT_ADF1_RADIO_TENTHS_DEC,			// Decrements ADF 1 by 0.1 KHz.
	EVENT_ADF1_RADIO_TENTHS_INC,			// Increments ADF 1 by 0.1 KHz.
	EVENT_XPNDR_1000_DEC,			// Decrements first digit of transponder
	EVENT_XPNDR_100_DEC,			// Decrements second digit of transponder
	EVENT_XPNDR_10_DEC,			// Decrements third digit of transponder
	EVENT_XPNDR_1_DEC,			// Decrements fourth digit of transponder
	EVENT_XPNDR_DEC_CARRY,			// Decrements fourth digit of transponder, and with carry.
	EVENT_XPNDR_INC_CARRY,			// Increments fourth digit of transponder, and with carry.
	EVENT_ADF_FRACT_DEC_CARRY,			// Decrements ADF 1 frequency by 0.1 KHz, with carry
	EVENT_ADF_FRACT_INC_CARRY,			// Increments ADF 1 frequency by 0.1 KHz, with carry
	EVENT_COM1_TRANSMIT_SELECT,			// Selects COM 1 to transmit
	EVENT_COM2_TRANSMIT_SELECT,			// Selects COM 2 to transmit
	EVENT_COM_RECEIVE_ALL_TOGGLE,			// Toggles all COM radios to receive on
	EVENT_COM_RECEIVE_ALL_SET,			// Sets whether to receive on all COM radios (1,0)
	EVENT_MARKER_SOUND_TOGGLE,			// Toggles marker beacon sound on/off
	EVENT_ADF_COMPLETE_SET,			// Sets ADF 1 frequency (BCD Hz)
	EVENT_ADF1_WHOLE_INC,			// Increments ADF 1 by 1 KHz, with carry as digits wrap.
	EVENT_ADF1_WHOLE_DEC,			// Decrements ADF 1 by 1 KHz, with carry as digits wrap.
	EVENT_ADF2_100_INC,			// Increments the ADF 2 frequency 100 digit, with wrapping
	EVENT_ADF2_10_INC,			// Increments the ADF 2 frequency 10 digit, with wrapping
	EVENT_ADF2_1_INC,			// Increments the ADF 2 frequency 1 digit, with wrapping
	EVENT_ADF2_RADIO_TENTHS_INC,			// Increments ADF 2 frequency 1/10 digit, with wrapping
	EVENT_ADF2_100_DEC,			// Decrements the ADF 2 frequency 100 digit, with wrapping
	EVENT_ADF2_10_DEC,			// Decrements the ADF 2 frequency 10 digit, with wrapping
	EVENT_ADF2_1_DEC,			// Decrements the ADF 2 frequency 1 digit, with wrapping
	EVENT_ADF2_RADIO_TENTHS_DEC,			// Decrements ADF 2 frequency 1/10 digit, with wrapping
	EVENT_ADF2_WHOLE_INC,			// Increments ADF 2 by 1 KHz, with carry as digits wrap.
	EVENT_ADF2_WHOLE_DEC,			// Decrements ADF 2 by 1 KHz, with carry as digits wrap.
	EVENT_ADF2_FRACT_DEC_CARRY,			// Decrements ADF 2 frequency by 0.1 KHz, with carry
	EVENT_ADF2_FRACT_INC_CARRY,			// Increments ADF 2 frequency by 0.1 KHz, with carry
	EVENT_ADF2_COMPLETE_SET,			// Sets ADF 1 frequency (BCD Hz)
	EVENT_RADIO_ADF2_IDENT_DISABLE,			// Turns ADF 2 ID off
	EVENT_RADIO_ADF2_IDENT_ENABLE,			// Turns ADF 2 ID on
	EVENT_RADIO_ADF2_IDENT_TOGGLE,			// Toggles ADF 2 ID
	EVENT_RADIO_ADF2_IDENT_SET,			// Sets ADF 2 ID on/off (1,0)
	EVENT_FREQUENCY_SWAP,			// Swaps frequency with standby on whichever NAV or COM radio is selected.
	EVENT_DME_SELECT,			// Selects one of the two DME systems (1,2).
	EVENT_RADIO_SELECTED_DME_IDENT_ENABLE,			// Turns on the identification sound for the selected DME.
	EVENT_RADIO_SELECTED_DME_IDENT_DISABLE,			// Turns off the identification sound for the selected DME.
	EVENT_RADIO_SELECTED_DME_IDENT_SET,			// Sets the DME identification sound to the given filename.
	EVENT_RADIO_SELECTED_DME_IDENT_TOGGLE,			// Turns on or off the identification sound for the selected DME.
	EVENT_STROBES_TOGGLE,			// Toggle strobe lights
	EVENT_ALL_LIGHTS_TOGGLE,			// Toggle all lights
	EVENT_PANEL_LIGHTS_TOGGLE,			// Toggle panel lights
	EVENT_LANDING_LIGHTS_TOGGLE,			// Toggle landing lights
	EVENT_LANDING_LIGHT_UP,			// Rotate landing light up
	EVENT_LANDING_LIGHT_DOWN,			// Rotate landing light down
	EVENT_LANDING_LIGHT_LEFT,			// Rotate landing light left
	EVENT_LANDING_LIGHT_RIGHT,			// Rotate landing light right
	EVENT_LANDING_LIGHT_HOME,			// Return landing light to default position
	EVENT_STROBES_ON,			// Turn strobe lights on
	EVENT_STROBES_OFF,			// Turn strobe light off
	EVENT_STROBES_SET,			// Set strobe lights on/off (1,0)
	EVENT_PANEL_LIGHTS_ON,			// Turn panel lights on
	EVENT_PANEL_LIGHTS_OFF,			// Turn panel lights off
	EVENT_PANEL_LIGHTS_SET,			// Set panel lights on/off (1,0)
	EVENT_LANDING_LIGHTS_ON,			// Turn landing lights on
	EVENT_LANDING_LIGHTS_OFF,			// Turn landing lights off
	EVENT_LANDING_LIGHTS_SET,			// Set landing lights on/off (1,0)
	EVENT_TOGGLE_BEACON_LIGHTS,			// Toggle beacon lights
	EVENT_TOGGLE_TAXI_LIGHTS,			// Toggle taxi lights
	EVENT_TOGGLE_LOGO_LIGHTS,			// Toggle logo lights
	EVENT_TOGGLE_RECOGNITION_LIGHTS,			// Toggle recognition lights
	EVENT_TOGGLE_WING_LIGHTS,			// Toggle wing lights
	EVENT_TOGGLE_NAV_LIGHTS,			// Toggle navigation lights
	EVENT_TOGGLE_CABIN_LIGHTS,			// Toggle cockpit/cabin lights
	EVENT_LAST
};

#ifdef __cplusplus
extern "C" {
#endif

const struct simconnect_event_enum_data_t *get_simconnect_event_enum_data(enum simconnect_event_t value);
const char *get_simconnect_event_enum_name(enum simconnect_event_t value);

#ifdef __cplusplus
};
#endif

#endif
