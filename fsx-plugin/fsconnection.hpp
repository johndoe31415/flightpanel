#ifndef __FSCONNECTION_HPP__
#define __FSCONNECTION_HPP__

#include <stdint.h>
#include <stdbool.h>

/* Opaque type, only referenced via pointer */
struct flightsim_connection_t;

struct vhf_data_t {
	uint32_t freq_active_khz;
	uint32_t freq_standby_khz;
	bool rx, tx;
};

struct nav_data_t {
	uint32_t freq_active_khz;
	uint32_t freq_standby_khz;
	uint16_t obs;
	char ident[8];
	bool sound;
};

struct adf_data_t {
	uint32_t freq_hz;
	char ident[8];
	uint16_t compass_rose;
	bool sound;
};

struct dme_data_t {
	bool available;
	uint8_t nav_id;
	uint16_t distance_nm_tenths;
	uint16_t speed_kt;
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
	int16_t climbrate_ft_per_min;
	uint16_t heading;
	bool hdg_hold, nav_hold, alt_hold, rev_hold, apr_hold, ias_hold;
};

struct transponder_data_t {
	uint16_t squawk;
};

struct misc_data_t {
	uint16_t ias_kt;
	uint16_t indicated_alt_ft;
	uint16_t qnh_millibar;
	bool guide_gps;
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
