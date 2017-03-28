#ifndef __CONFIGURATION_H__
#define __CONFIGURATION_H__

#include <stdint.h>
#include <stdbool.h>

struct configuration {
	int some_value;
	uint32_t crc32;
};

/*************** AUTO GENERATED SECTION FOLLOWS ***************/
void write_configuration(struct configuration *config);
bool read_configuration(struct configuration *config);
/***************  AUTO GENERATED SECTION ENDS   ***************/

#endif
