#ifndef __BOUNDEDBUFFER_H__
#define __BOUNDEDBUFFER_H__

#include <stdint.h>
#include <stdbool.h>

typedef uint16_t bb_size_t;

struct bounded_buffer_t {
	bb_size_t bufsize;
	bb_size_t fill;
	bb_size_t rd_index;
	bb_size_t wr_index;
	uint8_t *data;
};

/*************** AUTO GENERATED SECTION FOLLOWS ***************/
bool boundedbuffer_get(struct bounded_buffer_t *buffer, void *data, bb_size_t length);
bool boundedbuffer_put(struct bounded_buffer_t *buffer, const void *data, bb_size_t length);
bool boundedbuffer_putbyte(struct bounded_buffer_t *buffer, uint8_t data);
int16_t boundedbuffer_getbyte(struct bounded_buffer_t *buffer);
bb_size_t boundedbuffer_put_head(struct bounded_buffer_t *buffer, const void *data, bb_size_t length);
void boundedbuffer_init(struct bounded_buffer_t *buffer);
/***************  AUTO GENERATED SECTION ENDS   ***************/

#endif
