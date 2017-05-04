/**
 *	flightpanel - A Cortex-M4 based USB flight panel for flight simulators.
 *	Copyright (C) 2017-2017 Johannes Bauer
 *
 *	This file is part of flightpanel.
 *
 *	flightpanel is free software; you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation; this program is ONLY licensed under
 *	version 3 of the License, later versions are explicitly excluded.
 *
 *	flightpanel is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with flightpanel; if not, write to the Free Software
 *	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *	Johannes Bauer <JohannesBauer@gmx.de>
**/

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
