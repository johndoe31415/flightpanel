#ifndef __ROTARY_H__
#define __ROTARY_H__

#include <stdint.h>
#include <stdbool.h>

struct rotary_encoder_t {
	/* These are configuration variables */
	uint16_t value;
	uint16_t max_value;
	bool wrap_around;

	/* These are internal */
	uint8_t time_since_last_change;
	bool origin;
	bool armed;
	bool direction;
};

/*************** AUTO GENERATED SECTION FOLLOWS ***************/
bool rotary_encoder_update(struct rotary_encoder_t *rotary, bool value1, bool value2);
/***************  AUTO GENERATED SECTION ENDS   ***************/

#endif
