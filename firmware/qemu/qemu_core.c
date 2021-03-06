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
#include <stdlib.h>
#include <unistd.h>
#include "qemu_core.h"
#include "../atomic2.h"

uint8_t foobar[128];
int mookoo = 0x12345678;

void rs232_flush_buffer(void);
void rs232_flush_buffer(void) {
}

void NMI_Handler(void) {
	while (true);
}

void MemManage_Handler(void) {
	while (true);
}

void SVC_Handler(void) {
	while (true);
}

void PWR_EnterSTANDBYMode(void) {
}

void cause_fault(void);
void cause_fault_setregs(void);

void SystemInit(void) {
	printf("SystemInit()\n");

	atomic_t atomic = 0;
	printf("Value %ld\n", atomic);
	printf("INC: %d\n", atomic_inc(&atomic, 1));
	printf("Value %ld\n", atomic);
	printf("INC: %d\n", atomic_inc(&atomic, 1));
	printf("Value %ld\n", atomic);
	printf("INC: %d\n", atomic_inc(&atomic, 1));
	printf("Value %ld\n", atomic);


	/*
	foobar[0] = 0xaa;
	foobar[127] = 0x55;
	mookoo++;
	cause_fault_setregs();
	*/
	while (true);
}

int main(void) {
	while (true);
}

