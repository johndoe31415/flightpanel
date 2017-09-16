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

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

#include <misc.h>
#include <core_cm4.h>
#include <stm32f4xx_pwr.h>
#include "fault.h"

static const char *exception_names[] = {
	"Illegal",
	"Reset",
	"NMI",
	"Hard Fault",
	"Mem Manage Fault",
	"Bus Fault",
	"Usage Fault",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"SVC",
	"Debug Monitor",
};

void soft_fault(const char *msg) {
	__disable_irq();
	printf("Soft fault: %s\n", msg);
	while (true);
}

void fail_assertion(const char *assertion, const char *filename, int lineno) {
	//printf("Assertion failed: %s (%s:%d)\n", assertion, filename, lineno);
	while (true);
}

void generic_fault_handler(uint32_t fault_id, const struct fault_stack_layout_t *stack_layout) {
	/* ARM manual specifies 6 bit (0x3f) but STM32 has 9 bit for this (0x1ff) */
	int exception_no = stack_layout->psr & 0x1ff;
	if (fault_id) {
		printf("~~~~~~~~~~~~~ Fault ID %lu: Exception %u (%s) ~~~~~~~~~~~~~\n", fault_id, exception_no, (exception_no < 13) ? exception_names[exception_no] : "?");
	} else {
		printf("~~~~~~~~~~~~~ Fault %u (%s) ~~~~~~~~~~~~~\n", exception_no, (exception_no < 13) ? exception_names[exception_no] : "?");
	}
#if 0
	for (int i = 0; i <= 26; i++) {
		printf("X%-2d = %8lx\n", i, ((uint32_t*)stack_layout)[i]);
	}
	printf("\n");
	printf("\n");
	printf("\n");
#endif
	for (int i = 0; i < 4; i++) {
		printf("r%-2d = %8lx   ", i, stack_layout->r0[i]);
	}
	printf("\n");
	for (int i = 0; i < 8; i++) {
		printf("r%-2d = %8lx   ", 4 + i, stack_layout->r4[i]);
		if (i == 3) {
			printf("\n");
		}
	}
	printf("\n");
	printf("r12 = %8lx   ", stack_layout->r12);
	printf("sp  = %8lx   ", (uint32_t)stack_layout + sizeof(struct fault_stack_layout_t));
	printf("lr  = %8lx   ", stack_layout->r14);
	printf("pc  = %8lx   ", stack_layout->pc);
	printf("\n");
	printf("xPSR= %8lx   ", stack_layout->xpsr);
	printf("PSR = %8lx   ", stack_layout->psr);
	printf("FAM = %8lx   ", stack_layout->faultmask);
	printf("CC  = [");
	printf("%c", (stack_layout->psr >> 31) & 0x1 ? 'N' : ' ');
	printf("%c", (stack_layout->psr >> 30) & 0x1 ? 'Z' : ' ');
	printf("%c", (stack_layout->psr >> 29) & 0x1 ? 'C' : ' ');
	printf("%c", (stack_layout->psr >> 28) & 0x1 ? 'V' : ' ');
	printf("]");
	printf("\n");
	{
		uint32_t hfsr = SCB->HFSR;
		printf("HFSR: ");
		printf("%s", ((hfsr >> 1) & 0x01) ? "VECTTBL ": "");
		printf("%s", ((hfsr >> 30) & 0x01) ? "FORCED ": "");
		printf("%s", ((hfsr >> 31) & 0x01) ? "DEBUGEVT ": "");
		printf("\n");
	}
	{
		uint32_t cfsr = SCB->CFSR;
		printf("BFSR: ");
		printf("%s", ((cfsr >> (8 + 0)) & 0x01) ? "IBUSERR ": "");
		printf("%s", ((cfsr >> (8 + 1)) & 0x01) ? "PRECISERR ": "");
		printf("%s", ((cfsr >> (8 + 2)) & 0x01) ? "IMPRECISERR ": "");
		printf("%s", ((cfsr >> (8 + 3)) & 0x01) ? "UNSTKERR ": "");
		printf("%s", ((cfsr >> (8 + 4)) & 0x01) ? "STKERR ": "");
		printf("%s", ((cfsr >> (8 + 7)) & 0x01) ? "BFARVALID ": "");
		printf("   ");
		printf("UFSR: ");
		printf("%s", ((cfsr >> (16 + 0)) & 0x01) ? "UNDEFINSTR ": "");
		printf("%s", ((cfsr >> (16 + 1)) & 0x01) ? "INVSTATE ": "");
		printf("%s", ((cfsr >> (16 + 2)) & 0x01) ? "INVPC ": "");
		printf("%s", ((cfsr >> (16 + 3)) & 0x01) ? "NOCP ": "");
		printf("%s", ((cfsr >> (16 + 8)) & 0x01) ? "UNALIGNED ": "");
		printf("%s", ((cfsr >> (16 + 9)) & 0x01) ? "DIVBYZERO ": "");
		printf("\n");
	}
	printf("IABR: ");
	for (int i = 0; i < 8; i++) {
		uint32_t iabr = NVIC->IABR[i];
		for (int j = 0; j < 32; j++) {
			if ((iabr >> j) & 0x1) {
				printf("IABR%d.%d ", i, j);
			}
		}
	}
	printf("\n");
	printf("~~~~~~~~~~~~~ Device stopped ~~~~~~~~~~~~~\n");
	for (volatile int i = 0; i < 100000; i++);
	PWR_EnterSTANDBYMode();
	while (true);
}

