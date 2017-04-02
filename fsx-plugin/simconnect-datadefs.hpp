#ifndef __SIMCONNECT_DATADEFS_HPP__
#define __SIMCONNECT_DATADEFS_HPP__

#include <stdint.h>

struct simconnect_datatype_information_t {
	char title[256];
} __attribute__((packed));
static_assert(sizeof(struct simconnect_datatype_information_t) == 256, "struct simconnect_datatype_information_t incorrectly packed.");
#define SIMCONNECT_DATATYPE_INFORMATION_DATUM_COUNT			1

struct simconnect_datatype_instruments_t {
	char nav1_ident[8];
	double nav1_obs_deg;
	double nav2_obs_deg;
	char adf_ident[8];
	double adf_compass_rose_deg;
	double dme_speed;
	double dme_distance;
	double ap_altitude_ft;
	double ap_climbrate_ft_per_min;
	double ias;
	double indicated_alt_ft;
	double qnh_millibar;
	int32_t vhf1_freq_active;
	int32_t vhf1_freq_standby;
	int32_t vhf1_tx;
	int32_t vhf2_freq_active;
	int32_t vhf2_freq_standby;
	int32_t vhf2_tx;
	int32_t vhf_all_rx;
	int32_t nav1_freq_active;
	int32_t nav1_freq_standby;
	int32_t nav1_sound;
	int32_t nav2_freq_active;
	int32_t nav2_freq_standby;
	int32_t nav2_sound;
	int32_t adf_freq_active;
	int32_t adf_sound;
	int32_t dme_sound;
	int32_t xpdr_squawk;
	int32_t light_states;
	int32_t ap_master;
	int32_t ap_hdg_hold;
	int32_t ap_nav_hold;
	int32_t ap_alt_hold;
	int32_t ap_rev_hold;
	int32_t ap_apr_hold;
	int32_t ap_ias_hold;
} __attribute__((packed));
static_assert(sizeof(struct simconnect_datatype_instruments_t) == 196, "struct simconnect_datatype_instruments_t incorrectly packed.");
#define SIMCONNECT_DATATYPE_INSTRUMENTS_DATUM_COUNT			37


void simconnect_datadefs_register_information(void *hSimConnect, int group_id);
void simconnect_datadefs_register_instruments(void *hSimConnect, int group_id);

#endif
