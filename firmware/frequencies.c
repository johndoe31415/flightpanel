#include "frequencies.h"

static const uint8_t vhf_frequency_100khz_divisions[VHF_DIVISIONS_PER_100_KHZ] = {
	0, 5, 10, 15, 25, 30, 35, 40, 50, 55, 60, 65, 75, 80, 85, 90
};

uint32_t vhf_index_to_frequency_khz(int vhf_index) {
	vhf_index = vhf_index % VHF_DIVISIONS;
	int mhz = vhf_index / VHF_DIVISIONS_PER_MHZ;
	int division = vhf_index % VHF_DIVISIONS_PER_MHZ;
	int khz100 = division / VHF_DIVISIONS_PER_100_KHZ;
	int khz_index = division % VHF_DIVISIONS_PER_100_KHZ;
	uint32_t result = VHF_MIN_FREQUENCY_KHZ + (1000 * mhz) + (100 * khz100) + vhf_frequency_100khz_divisions[khz_index];
	debug("%d -> %d %d %d -> %u\n", vhf_index, mhz, khz100, khz_index, result);
	return result;
}

uint32_t nav_index_to_frequency_khz(int nav_index) {
	return NAV_MIN_FREQUENCY_KHZ + (nav_index * 50);
}
