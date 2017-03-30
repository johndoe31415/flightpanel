#include <stdio.h>
#include <string.h>
#include "ssd1306.h"
#include "displays.h"
#include "pinmap.h"
#include "timer.h"
#include "surface.h"
#include "spi.h"

#include "font.h"

static void displays_check_dma_schedule(void);

static int8_t current_dma_update = -1;
static const struct ssd1306_display_t displays[DISPLAY_COUNT] = {
	[0] = {
		.nSS_GPIO = Display1_nSS_GPIO,
		.nSS_Pin = Display1_nSS_Pin,
		.surface = &(const struct surface_t) {
			.width = 128,
			.height = 64,
			.data = (uint8_t[128 * 64 / 8]) { },
		},
	},
};
static bool surface_dirty[DISPLAY_COUNT];

static void displays_enable_cs(int display_index) {
	GPIO_ResetBits(displays[display_index].nSS_GPIO, displays[display_index].nSS_Pin);
}

static void displays_disable_cs(int display_index) {
	GPIO_SetBits(displays[display_index].nSS_GPIO, displays[display_index].nSS_Pin);
}

static void display_dma_start(int display_index) {
	current_dma_update = display_index;
	displays_enable_cs(display_index);
	spi_tx_data_dma(displays[display_index].surface->data, 128 * 64 / 8);
}

void display_dma_finished(void) {
	displays_disable_cs(current_dma_update);
	current_dma_update = -1;
	displays_check_dma_schedule();
}

static void displays_check_dma_schedule(void) {
	if (current_dma_update != -1) {
		/* DMA already in progress */
		return;
	}
	for (int i = 0; i < DISPLAY_COUNT; i++) {
		if (surface_dirty[i]) {
			surface_dirty[i] = false;
			display_dma_start(i);
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
	/* Reset all displays */
	Display_nRESET_SetLOW();
	delay_millis(1);
	Display_nRESET_SetHIGH();

	for (int did = 0; did < DISPLAY_COUNT; did++) {
		for (int i = 0; i < 100; i++) {
			surface_clear(displays[0].surface);
		}
		ssd1306_init(&displays[0]);
	}
}

