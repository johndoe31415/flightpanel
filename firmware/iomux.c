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
#include "instruments.h"

static uint8_t output_transfer[IOMUX_BYTECOUNT];
static uint8_t output_modify[IOMUX_BYTECOUNT];
static uint8_t inputs[IOMUX_BYTECOUNT];
static bool iomux_disabled = false;
static bool iomux_clamped = false;

const uint8_t *iomux_input_array(void) {
	return inputs;
}

void iomux_disable(void) {
	iomux_disabled = true;
}

void iomux_clamp_all_outputs(bool do_clamp) {
	iomux_clamped = do_clamp;
}

/* Trigger IOMux transfer */
void iomux_trigger(void) {
	if (iomux_disabled) {
		return;
	}

	if (!spi_dma_tx_rx_ready(IOMuxSPI_DMAStream_TX, IOMuxSPI_DMAStream_RX)) {
		/* Last handling was lasting longer than expected, do silently discard
		 * this round. */
		return;
	}

	/* Change SCK to GPIO from AF */
	reinit_iomux_spi_sck_AF(false);

	/* Then issue a SCK pulse when PE is load, effectively parallel loading the
	 * HC166 chains */
	IOMux_In_PE_set_ACTIVE();
	delay_loopcnt(LOOPCOUNT_50NS);
	IOMux_SCK_pulse();
	IOMux_In_PE_set_INACTIVE();

	/* Switch back SCK to its original function */
	reinit_iomux_spi_sck_AF(true);

	/* Start the DMA transfer */
	memcpy(output_transfer, output_modify, sizeof(output_transfer));
	spi_tx_rx_data_dma(IOMuxSPI_SPI, IOMuxSPI_DMAStream_TX, output_transfer, IOMuxSPI_DMAStream_RX, inputs, sizeof(output_transfer));
}

void dsr_iomux_dma_finished(void) {
	IOMux_Out_STCP_pulse();
	IOMux_Out_OE_set_LOW();
	instruments_handle_inputs();
}

bool iomux_get_input_from(const uint8_t *iomux_array, int pin_id) {
	int byteno = pin_id / 8;
	int bitno = pin_id % 8;
	return ((iomux_array[byteno] >> bitno) & 1) != 0;
}

bool iomux_get_input(int pin_id) {
	return iomux_get_input_from(inputs, pin_id);
}

void iomux_output_setall(uint8_t byte_value) {
	memset(output_modify, byte_value, IOMUX_BYTECOUNT);
}

void iomux_output_toggle(int pin_id) {
	int byteno = pin_id / 8;
	int bitno = pin_id % 8;
	output_modify[byteno] ^= (1 << bitno);
}

void iomux_output_set(int pin_id, bool value) {
	int byteno = pin_id / 8;
	int bitno = pin_id % 8;
	if (value) {
		output_modify[byteno] |= (1 << bitno);
	} else {
		output_modify[byteno] &= ~(1 << bitno);
	}
}

