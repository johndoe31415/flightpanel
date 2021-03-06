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
#include <string.h>
#include "ssd1306.h"
#include "displays.h"
#include "pinmap.h"
#include "timer.h"
#include "surface.h"
#include "spi.h"
#include "fncmap.h"
#include "font.h"
#include "atomic.h"

static bool inhibit;
static atomic_t dma_running;
static uint8_t last_updated_display;

static const struct ssd1306_display_t displays[DISPLAY_COUNT] = {
	[0] = {
		.CS_GPIO = Display1_CS_GPIO,
		.CS_Pin = Display1_CS_Pin,
		.surface = &(const struct surface_t) {
			.width = 128,
			.height = 64,
			.data = (uint8_t[128 * 64 / 8]) { },
		},
	},
	[1] = {
		.CS_GPIO = Display2_CS_GPIO,
		.CS_Pin = Display2_CS_Pin,
		.surface = &(const struct surface_t) {
			.width = 128,
			.height = 64,
			.data = (uint8_t[128 * 64 / 8]) { },
		},
	},
	[2] = {
		.CS_GPIO = Display3_CS_GPIO,
		.CS_Pin = Display3_CS_Pin,
		.surface = &(const struct surface_t) {
			.width = 128,
			.height = 64,
			.data = (uint8_t[128 * 64 / 8]) { },
		},
	},
	[3] = {
		.CS_GPIO = Display4_CS_GPIO,
		.CS_Pin = Display4_CS_Pin,
		.surface = &(const struct surface_t) {
			.width = 128,
			.height = 64,
			.data = (uint8_t[128 * 64 / 8]) { },
		},
	},
	[4] = {
		.CS_GPIO = Display5_CS_GPIO,
		.CS_Pin = Display5_CS_Pin,
		.surface = &(const struct surface_t) {
			.width = 128,
			.height = 64,
			.data = (uint8_t[128 * 64 / 8]) { },
		},
	},
	[5] = {
		.CS_GPIO = Display6_CS_GPIO,
		.CS_Pin = Display6_CS_Pin,
		.surface = &(const struct surface_t) {
			.width = 128,
			.height = 64,
			.data = (uint8_t[128 * 64 / 8]) { },
		},
	},
	[6] = {
		.CS_GPIO = Display7_CS_GPIO,
		.CS_Pin = Display7_CS_Pin,
		.surface = &(const struct surface_t) {
			.width = 128,
			.height = 64,
			.data = (uint8_t[128 * 64 / 8]) { },
		},
	},
	[7] = {
		.CS_GPIO = Display8_CS_GPIO,
		.CS_Pin = Display8_CS_Pin,
		.surface = &(const struct surface_t) {
			.width = 128,
			.height = 64,
			.data = (uint8_t[128 * 64 / 8]) { },
		},
	},
	[8] = {
		.CS_GPIO = Display9_CS_GPIO,
		.CS_Pin = Display9_CS_Pin,
		.surface = &(const struct surface_t) {
			.width = 128,
			.height = 64,
			.data = (uint8_t[128 * 64 / 8]) { },
		},
	},
	[9] = {
		.CS_GPIO = Display10_CS_GPIO,
		.CS_Pin = Display10_CS_Pin,
		.surface = &(const struct surface_t) {
			.width = 128,
			.height = 64,
			.data = (uint8_t[128 * 64 / 8]) { },
		},
	},
	[10] = {
		.CS_GPIO = Display11_CS_GPIO,
		.CS_Pin = Display11_CS_Pin,
		.surface = &(const struct surface_t) {
			.width = 128,
			.height = 64,
	.data = (uint8_t[128 * 64 / 8]) { },
		},
	},
	[11] = {
		.CS_GPIO = Display12_CS_GPIO,
		.CS_Pin = Display12_CS_Pin,
		.surface = &(const struct surface_t) {
			.width = 128,
			.height = 64,
			.data = (uint8_t[128 * 64 / 8]) { },
		},
	},
};
static bool surface_dirty[DISPLAY_COUNT];

static void displays_enable_cs(int display_index) {
	GPIO_ResetBits(displays[display_index].CS_GPIO, displays[display_index].CS_Pin);
}

static void displays_disable_cs(int display_index) {
	GPIO_SetBits(displays[display_index].CS_GPIO, displays[display_index].CS_Pin);
}

static void display_dma_start(int display_index) {
	/* Only start if no DMA is running right now */
	if (!inhibit && atomic_lock(&dma_running)) {
		surface_dirty[display_index] = false;
		last_updated_display = display_index;
		displays_enable_cs(display_index);
		spi_tx_rx_data_dma(DisplaySPI_SPI, DisplaySPI_DMAStream_TX, displays[display_index].surface->data, DisplaySPI_DMAStream_RX, NULL, 128 * 64 / 8);
	}
}

void isr_display_dma_finished(void) {
	displays_disable_cs(last_updated_display);
}

void dsr_display_dma_finished(void) {
	atomic_unlock(&dma_running);
	displays_check_dma_schedule();
}

void displays_check_dma_schedule(void) {
	if (!spi_dma_tx_rx_ready(DisplaySPI_DMAStream_TX, DisplaySPI_DMAStream_RX)) {
		/* Transmission in progress. */
		return;
	}
	for (int i = 0; i < DISPLAY_COUNT; i++) {
		int display_index = (i + last_updated_display) % DISPLAY_COUNT;
		if (surface_dirty[display_index]) {
			display_dma_start(display_index);
			return;
		}
	}
}

void display_mark_surface_dirty(int display_index) {
	surface_dirty[display_index] = true;
	displays_check_dma_schedule();
}

const struct surface_t* displays_get_surface(int display_index) {
	return displays[display_index].surface;
}

void init_displays(void) {
	/* First disable all updates */
	inhibit = true;

	/* Then wait for any DMA to stop that might be running right now */
	while (!spi_dma_tx_rx_ready(DisplaySPI_DMAStream_TX, DisplaySPI_DMAStream_RX));

	/* Reset all displays. Do busy waiting here because this might be triggered
	 * from ISR and therefore delay_millis() would deadlock. */
	Display_RESET_set_ACTIVE();
	delay_loopcnt(LOOPCOUNT_1MS);
	Display_RESET_set_INACTIVE();

	for (int did = 0; did < DISPLAY_COUNT; did++) {
		surface_clear(displays[did].surface);
		ssd1306_init(&displays[did]);
	}

	/* Then allow display updates again */
	inhibit = false;
}
