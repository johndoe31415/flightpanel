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

.syntax unified
.cpu cortex-m4
.fpu softvfp
.thumb

.thumb_func
.type atomic_inc, %function
atomic_inc:
	retry_inc:
		ldrex r2, [r0]
		cmp r1, r2
		ble limit_reached

		adds r2, $1
		strex r3, r2, [r0]

		cmp r3, $0
		bne retry_inc

		mov r0, $1
		bx lr

	limit_reached:
		mov r0, $0
		clrex
		bx lr
.size atomic_inc, .-atomic_inc
.global atomic_inc

.thumb_func
.type atomic_dec, %function
atomic_dec:
	retry_dec:
		ldrex r2, [r0]
		cbz r2, already_zero

		subs r2, $1
		strex r3, r2, [r0]

		cmp r3, $0
		bne retry_dec

		mov r0, $1
		bx lr

	already_zero:
		mov r0, $0
		clrex
		bx lr
.size atomic_dec, .-atomic_dec
.global atomic_dec

