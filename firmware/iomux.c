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
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "iomux.h"
#include "fncmap.h"
#include "spi.h"
#include "pinmap.h"
#include "init.h"

#define BYTECOUNT		7
static uint8_t outputs[BYTECOUNT] = { 1, 3, 7, 10 };
static uint8_t inputs[BYTECOUNT];
static uint8_t last_inputs[BYTECOUNT];
static volatile bool read_new_data;

/* Trigger IOMux transfer */
void iomux_trigger(void) {
	//spi_tx_data(IOMuxSPI_SPI, outputs, sizeof(outputs));
	//spi_tx_data_dma(IOMuxSPI_SPI, IOMuxSPI_DMAStream_TX, outputs, sizeof(outputs));
	IOMux_In_PE_SetLOW();
	reinit_iomux_spi_sck_AF(false);
	IOMux_SCK_SetHIGH();
	for (volatile int i = 0; i < 5; i++);
	IOMux_SCK_SetLOW();
	reinit_iomux_spi_sck_AF(true);
	IOMux_In_PE_SetHIGH();

	spi_tx_rx_data_dma(IOMuxSPI_SPI, IOMuxSPI_DMAStream_TX, outputs, IOMuxSPI_DMAStream_RX, inputs, sizeof(outputs));
}

static void iomux_dump_array(const uint8_t *array, int length) {
	for (int i = 0; i < length; i++) {
		for (int j = 7; j >= 0; j--) {
			int bit = (array[i] >> j) & 1;
			printf("%u", bit);
		}
		printf(" ");
	}
}

void iomux_output_set(int pin_id, bool value) {
	int byteno = pin_id / 8;
	int bitno = pin_id % 8;
	if (value) {
		outputs[byteno] |= (1 << bitno);
	} else {
		outputs[byteno] &= ~(1 << bitno);
	}
}

static void iomux_diff_array(const uint8_t *array1, const uint8_t *array2, int length) {
	for (int i = 0; i < length; i++) {
		for (int j = 0; j < 8; j++) {
			bool bit1 = (array1[i] >> j) & 1;
			bool bit2 = (array2[i] >> j) & 1;
			if (bit1 != bit2) {
				int pin_id = (8 * i) + j;
				printf("%c%d ", bit2 ? '+' : '-', pin_id);
			}
		}
	}
}

static void iomux_dump(void) {
	printf("In: ");
	iomux_dump_array(inputs, BYTECOUNT);
	printf("  Out: ");
	iomux_dump_array(outputs, BYTECOUNT);
}

void iomux_dump_iochange(void) {
	if (!read_new_data) {
		return;
	}
	read_new_data = false;
	if (memcmp(last_inputs, inputs, BYTECOUNT)) {
		/* Some input has changed. Dump! */
		iomux_dump();
		printf("  ");
		iomux_diff_array(last_inputs, inputs, BYTECOUNT);
		printf("\n");
		memcpy(last_inputs, inputs, BYTECOUNT);
	}
}

void iomux_dma_finished(void) {
	read_new_data = true;
}

