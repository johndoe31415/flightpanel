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

#ifndef __FAULT_H__
#define __FAULT_H__

/*
 r10 sl
 r11 fp
 r12 ip
 r13 sp
 r14 lr
 r15 pc
*/
struct fault_stack_layout_t {
	/* These are pushed by the program code */
	uint32_t faultmask;
	uint32_t psr;
	uint32_t r4[8];
	uint32_t frame_lr;

	/* These are pushed by the ISR handler */
	uint32_t r0[4];
	uint32_t r12;		// ip
	uint32_t r14;		// lr
	uint32_t pc;
	uint32_t xpsr;
};

/*************** AUTO GENERATED SECTION FOLLOWS ***************/
void soft_fault(const char *msg);
void fail_assertion(const char *assertion, const char *filename, int lineno);
void generic_fault_handler(uint32_t fault_id, const struct fault_stack_layout_t *stack_layout);
void cause_hard_fault(void);
/***************  AUTO GENERATED SECTION ENDS   ***************/

#endif
