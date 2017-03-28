#include "instruments.h"
#include "usb.h"

static struct instrument_state {
	uint16_t vhf1, vhf2;
	uint16_t nav1, nav2;
	uint16_t ap_altitude;
	uint16_t ap_climbrate;
	uint16_t ap_ias;
	uint16_t squawk;
	bool transponder_charly;
	uint16_t qnh;
	bool ap_active;
	bool ap_hold_altitude;
	bool ap_hold_navigation;
	bool ap_hold_reverse;
	bool ap_hold_approach;
	bool gps_nav;
};

static struct flight_data {
	uint16_t ias;
	uint16_t altitude;
};

