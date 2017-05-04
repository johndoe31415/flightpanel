#include <string.h>
#include <stdbool.h>

#include "boundedbuffer.h"

bool boundedbuffer_get(struct bounded_buffer_t *buffer, void *data, bb_size_t length) {
	if (buffer->bufsize < length) {
		/* Not enough data available, reject. */
		return false;
	}

	bb_size_t bytes_until_eob = buffer->bufsize - buffer->rd_index;
	if (length <= bytes_until_eob) {
		/* Can fit in one go */
		memcpy(data, buffer->data + buffer->rd_index, length);
	} else {
		/* Wraparound */
		memcpy(data, buffer->data + buffer->rd_index, bytes_until_eob);
		memcpy(data + bytes_until_eob, buffer->data + 0, length - bytes_until_eob);
	}


	buffer->rd_index = (buffer->rd_index + length) % buffer->bufsize;
	buffer->fill -= length;
	return true;
}

bool boundedbuffer_put(struct bounded_buffer_t *buffer, const void *data, bb_size_t length) {
	if (length == 0) {
		return true;
	}

	bb_size_t bytes_free = buffer->bufsize - buffer->fill;
	if (length > bytes_free) {
		/* Not enough capacity, reject. */
		return false;
	}

	bb_size_t bytes_until_eob = buffer->bufsize - buffer->wr_index;
	if (length <= bytes_until_eob) {
		/* Can fit in one go */
		memcpy(buffer->data + buffer->wr_index, data, length);
	} else {
		/* Wraparound */
		memcpy(buffer->data + buffer->wr_index, data, bytes_until_eob);
		memcpy(buffer->data + 0, data + bytes_until_eob, length - bytes_until_eob);
	}

	buffer->wr_index = (buffer->wr_index + length) % buffer->bufsize;
	buffer->fill += length;
	return true;
}

bool boundedbuffer_putbyte(struct bounded_buffer_t *buffer, uint8_t data) {
	if (buffer->bufsize == buffer->fill) {
		return false;
	}
	buffer->data[buffer->wr_index] = data;
	buffer->wr_index++;
	if (buffer->wr_index == buffer->bufsize) {
		buffer->wr_index = 0;
	}
	return true;
}

int16_t boundedbuffer_getbyte(struct bounded_buffer_t *buffer) {
	if (buffer->fill == 0) {
		return -1;
	}
	int16_t retval = buffer->data[buffer->rd_index];
	buffer->rd_index++;
	if (buffer->rd_index == buffer->bufsize) {
		buffer->rd_index = 0;
	}
	return retval;
}

bb_size_t boundedbuffer_put_head(struct bounded_buffer_t *buffer, const void *data, bb_size_t length) {
	bb_size_t bytes_free = buffer->bufsize - buffer->fill;
	if (length > bytes_free) {
		boundedbuffer_put(buffer, data, bytes_free);
		return bytes_free;
	} else {
		boundedbuffer_put(buffer, data, length);
		return length;
	}
}

void boundedbuffer_init(struct bounded_buffer_t *buffer) {
	memset(buffer, 0, sizeof(struct bounded_buffer_t));
}

