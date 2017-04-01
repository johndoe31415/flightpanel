#ifndef __FSCONNECTION_HPP__
#define __FSCONNECTION_HPP__

#include <stdint.h>
#include <stdbool.h>

struct vhf_data_t {
	uint32_t freq_active_khz;
	uint32_t freq_standby_khz;
	bool rx, tx;
};

struct nav_data_t {
	uint32_t freq_active_khz;
	uint32_t freq_standby_khz;
	uint16_t obs;
	bool sound;
};

struct adf_data_t {
	uint32_t freq_hz;
	bool sound;
};

struct dme_data_t {
	bool sound;
};

struct light_data_t {
	bool beacon;
	bool landing;
	bool taxi;
	bool nav;
	bool strobe;
};

struct ap_data_t {
	bool active;
	uint32_t altitude_ft;
	uint16_t climbrate_ft_per_min;
	uint16_t heading;
	bool hdg_hold, nav_hold, alt_hold, rev_hold, apr_hold, ias_hold;
};

struct transponder_data_t {
	uint16_t squawk;
};

struct misc_data_t {
	uint16_t qnh_millibar;
};

struct instrument_data_t {
	struct vhf_data_t vhf1, vhf2;
	struct nav_data_t nav1, nav2;
	struct adf_data_t adf;
	struct dme_data_t dme;
	struct light_data_t lights;
	struct ap_data_t ap;
	struct transponder_data_t xpdr;
	struct misc_data_t misc;
};

/*************** AUTO GENERATED SECTION FOLLOWS ***************/
void dump_instrument_data(const struct instrument_data_t *data);
void fsconnection_incoming_data(const struct instrument_data_t *data);
/***************  AUTO GENERATED SECTION ENDS   ***************/

#endif
