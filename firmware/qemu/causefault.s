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

.type cause_fault, %function
cause_fault:
	bkpt #1
.size cause_fault, .-cause_fault
.global cause_fault

.type cause_fault_setregs, %function
cause_fault_setregs:
	ldr r0, =0xaabbcc00
	ldr r1, =0xaabbcc01
	ldr r2, =0xaabbcc02
	ldr r3, =0xaabbcc03
	ldr r4, =0xaabbcc04
	ldr r5, =0xaabbcc05
	ldr r6, =0xaabbcc06
	ldr r7, =0xaabbcc07
	ldr r8, =0xaabbcc08
	ldr r9, =0xaabbcc09
	ldr r10, =0xaabbcc10		// sl
	ldr r11, =0xaabbcc11		// fp 
	ldr r12, =0xaabbcc12		// ip
//	ldr r13, =0xaabbcc13		// sp
	ldr r14, =0xaabbcc14		// lr
//	ldr r15, =0xaabbcc15		// pc
	b cause_fault
.size cause_fault_setregs, .-cause_fault_setregs
.global cause_fault_setregs
