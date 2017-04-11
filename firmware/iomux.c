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
#include "iomux.h"
#include "fncmap.h"
#include "spi.h"

#define BYTECOUNT		8
static uint8_t outputs[BYTECOUNT];
static uint8_t inputs[BYTECOUNT];

/* Trigger IOMux transfer */
void iomux_trigger(void) {
	spi_tx_rx_data_dma(IOMuxSPI_SPI, IOMuxSPI_DMAStream_TX, outputs, IOMuxSPI_DMAStream_RX, inputs, sizeof(outputs));
	//spi_tx_data_dma(IOMuxSPI_SPI, IOMuxSPI_DMAStream_TX, outputs, sizeof(outputs));
}

void iomux_dma_finished(void) {
#if 0
	for (int i = 0; i < sizeof(inputs); i++) {
		printf("%02x ", inputs[i]);
	}
	printf("\n");
#endif
}
