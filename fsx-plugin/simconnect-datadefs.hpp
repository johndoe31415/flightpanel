#ifndef __SIMCONNECT_DATADEFS_HPP__
#define __SIMCONNECT_DATADEFS_HPP__

#include <stdint.h>

struct simconnect_datatype_information_t {
	char title[256];
} __attribute__((packed));
static_assert(sizeof(struct simconnect_datatype_information_t) == 256, "struct simconnect_datatype_information_t incorrectly packed.");

struct simconnect_datatype_instruments_t {
	double vhf1_freq_active;
	double vhf1_freq_standby;
	double vhf2_freq_active;
	double vhf2_freq_standby;
	double nav1_freq_active;
	double nav1_freq_standby;
	double nav2_freq_active;
	double nav2_freq_standby;
	double adf_freq_active;
	double qnh_millibar;
	int32_t vhf1_tx;
	int32_t vhf2_tx;
	int32_t vhf_all_rx;
	int32_t nav1_sound;
	int32_t nav2_sound;
	int32_t adf_sound;
	int32_t dme_sound;
	int32_t xpdr_squawk;
	int32_t light_states;
} __attribute__((packed));
static_assert(sizeof(struct simconnect_datatype_instruments_t) == 116, "struct simconnect_datatype_instruments_t incorrectly packed.");


void simconnect_datadefs_register_information(void *hSimConnect, int group_id);
void simconnect_datadefs_register_instruments(void *hSimConnect, int group_id);

#endif
