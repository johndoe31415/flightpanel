#ifndef __FREQUENCIES_H__
#define __FREQUENCIES_H__

#include <stdint.h>

#define VHF_MIN_FREQUENCY_KHZ		118000
#define VHF_BANDWIDTH_MHZ			19
#define VHF_DIVISIONS_PER_100_KHZ	16
#define VHF_DIVISIONS_PER_MHZ		(VHF_DIVISIONS_PER_100_KHZ * 10)
#define VHF_DIVISIONS				(VHF_BANDWIDTH_MHZ * VHF_DIVISIONS_PER_MHZ)

#define NAV_MIN_FREQUENCY_KHZ		108000
#define NAV_BANDWIDTH_MHZ			10

/*************** AUTO GENERATED SECTION FOLLOWS ***************/
uint32_t vhf_index_to_frequency_khz(int vhf_index);
uint32_t nav_index_to_frequency_khz(int nav_index);
/***************  AUTO GENERATED SECTION ENDS   ***************/

#endif
