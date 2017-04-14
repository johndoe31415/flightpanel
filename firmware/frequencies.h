#ifndef __FREQUENCIES_H__
#define __FREQUENCIES_H__

#include <stdint.h>

#define VHF_MIN_FREQUENCY_KHZ		118000
#define VHF_BANDWIDTH_MHZ			19
#define VHF_DIVISIONS_PER_100_KHZ	16
#define VHF_DIVISIONS_PER_MHZ		(VHF_DIVISIONS_PER_100_KHZ * 10)
#define VHF_DIVISIONS				(VHF_BANDWIDTH_MHZ * VHF_DIVISIONS_PER_MHZ)

/*************** AUTO GENERATED SECTION FOLLOWS ***************/
uint32_t vhf_index_to_frequency_khz(int vhf_index);
/***************  AUTO GENERATED SECTION ENDS   ***************/

#endif
