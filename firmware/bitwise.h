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

#ifndef __BITWISE_H__
#define __BITWISE_H__

#define BIT_MASK(bit_length)											((1 << (bit_length)) - 1)
#define SHIFTED_BIT_MASK(bit_length, bit_offset)						(BIT_MASK(bit_length) << (bit_offset))
#define BIT_PATCH(value, bit_offset, bit_length, patch_value)			(((value) & ~SHIFTED_BIT_MASK(bit_length, bit_offset)) | (((patch_value) & BIT_MASK(bit_length)) << (bit_offset)))

#define BIT_PATCH_REGISTER(lvalue, bit_offset, bit_length, patch_value)	(lvalue) = BIT_PATCH((lvalue), (bit_offset), (bit_length), (patch_value))

#define GET_BITS(value, bit_offset, bit_length)							(((value) >> (bit_offset)) & BIT_MASK(bit_length))

#endif
