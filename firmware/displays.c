#include <string.h>
#include "ssd1306.h"
#include "displays.h"
#include "pinmap.h"
#include "timer.h"

static struct ssd1306_display display[DISPLAY_COUNT];
//static uint8_t refresh_display[DISPLAY_COUNT];

void displays_dma_finished(void) {
}

void displays_update_via_dma(uint8_t display_index) {
}

void init_displays(void) {
	/* Reset all displays */
	Display_RESET_SetHIGH();
	delay_millis(1);
	Display_RESET_SetLOW();

	memset(display, 0, sizeof(display));
	for (int did = 0; did < DISPLAY_COUNT; did++) {
		for (int i = 0; i < 100; i++) {
			display[0].display_data[i] = i;
		}
		ssd1306_init(&display[0]);
	}
}
