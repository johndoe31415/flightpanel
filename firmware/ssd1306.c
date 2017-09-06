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
#include "ssd1306.h"
#include "pinmap.h"
#include "spi.h"
#include "timer.h"
#include "fncmap.h"

// Fundamental commands
#define CMD_SET_CONTRAST_CONTROL(value)											0x81, (value)
#define CMD_ENTIRE_DISPLAY_ON(on_off)											0xa4 | ((on_off) & 0x01)
#define CMD_SET_INVERSE_DISPLAY(on_off)											0xa6 | ((on_off) & 0x01)
#define CMD_SET_DISPLAY_ON(on_off)												0xae | ((on_off) & 0x01)

// Addressing commands
#define CMD_SET_LOWER_COLUMN_START_ADDR(addr)									0x00 | ((value) & 0x0f)
#define CMD_SET_HIGHER_COLUMN_START_ADDR(addr)									0x10 | ((value) & 0x0f)
#define CMD_SET_MEMORY_ADDRESSING_MODE_HORIZONTAL								0x20, 0x00
#define CMD_SET_MEMORY_ADDRESSING_MODE_VERTICAL									0x20, 0x01
#define CMD_SET_MEMORY_ADDRESSING_MODE_PAGE_ADDR								0x20, 0x02
#define CMD_SET_COLUMN_START_END_ADDRESS(start, end)							0x21, ((start) & 0x7f), ((end) & 0x7f)
#define CMD_SET_PAGE_START_END_ADDRESS(start, end)								0x22, ((start) & 0x07), ((end) & 0x07)
#define CMD_SET_PAGE_START_ADDR(addr)											0xb0 | ((addr) & 0x07)

// Scrolling commands
#define CMD_DEACTIVATE_SCROLL													0x2e
#define CMD_ACTIVATE_SCROLL														0x2f

// Hardware configuration
#define CMD_SET_DISPLAY_START_LINE(line)										0x40 | ((line) & 0x3f)
#define CMD_SET_SEGMENT_REMAP(on_off)											0xa0 | ((on_off) & 0x01)
#define CMD_SET_MULTIPLEX_RATIO(n)												0xa8, (((n) - 1) & 0x3f)
#define CMD_SET_COM_OUTPUT_SCAN_DIRECTION_NORMAL								0xc0
#define CMD_SET_COM_OUTPUT_SCAN_DIRECTION_REMAPPED								0xc8
#define CMD_SET_DISPLAY_OFFSET(offset)											0xd3, ((offset) & 0x3f)
#define CMD_SET_COM_PINS_HW_CONFIG(alt_com_pin_on_off, com_lr_remap_on_off)		0xda, (0x02 | (((alt_com_pin_on_off) & 0x01) << 4) | (((com_lr_remap_on_off) & 0x01) << 5))
#define CMD_SET_DISPLAY_CLK_DIVIDE(divider, frequency)							0xd5, ((((divider) - 1) & 0x0f) | (((frequency) & 0x0f) << 4))
#define CMD_SET_PRECHARGE_PERIOD(phase1, phase2)								0xd9, (((phase1) & 0x0f) | (((phase2) & 0x0f) << 4))
#define CMD_SET_VCOMH_DESELECT_LVL_065Vcc										0xdb, (0x00 << 4)
#define CMD_SET_VCOMH_DESELECT_LVL_077Vcc										0xdb, (0x02 << 4)
#define CMD_SET_VCOMH_DESELECT_LVL_083Vcc										0xdb, (0x03 << 4)
#define CMD_NOP																	0xe3

// Charge pump command
#define CMD_SET_CHARGEPUMP(on_off)												0x8d, (0x10 | ((on_off) & 0x01) << 2)

static const uint8_t initialization_routine[] = {
	CMD_SET_MEMORY_ADDRESSING_MODE_HORIZONTAL,
	CMD_SET_MULTIPLEX_RATIO(0),

	CMD_SET_DISPLAY_OFFSET(0),
	CMD_SET_DISPLAY_START_LINE(0),

	CMD_SET_SEGMENT_REMAP(1),
	CMD_SET_COM_OUTPUT_SCAN_DIRECTION_REMAPPED,

	CMD_SET_COM_PINS_HW_CONFIG(1, 0),

	CMD_SET_CONTRAST_CONTROL(0x7f),					/* Higher is brighter */

	CMD_ENTIRE_DISPLAY_ON(0),
	CMD_SET_INVERSE_DISPLAY(0),

	CMD_SET_DISPLAY_CLK_DIVIDE(1, 0x8),

	CMD_SET_CHARGEPUMP(1),
};

void ssd1306_set_data(const struct ssd1306_display_t *display) {
	/* D/#C high */
	GPIO_SetBits(Display_DC_GPIO, Display_DC_Pin);
}

void ssd1306_set_control(const struct ssd1306_display_t *display) {
	/* D/#C low */
	GPIO_ResetBits(Display_DC_GPIO, Display_DC_Pin);
}

static void ssd1306_send_data(const struct ssd1306_display_t *display, const uint8_t *data, int length) {
	if (display->CS_GPIO) {
		GPIO_ResetBits(display->CS_GPIO, display->CS_Pin);
	}
	spi_tx_data(DisplaySPI_SPI, data, length);
	if (display->CS_GPIO) {
		GPIO_SetBits(display->CS_GPIO, display->CS_Pin);
	}
}

void ssd1306_init(const struct ssd1306_display_t *display) {
	/* Initialize everything, but don't turn display on just yet */
	ssd1306_set_control(display);
	ssd1306_send_data(display, initialization_routine, sizeof(initialization_routine));

	/* Then send complete content */
	ssd1306_set_data(display);
	ssd1306_send_data(display, display->surface->data, display->surface->width * display->surface->height / 8);

	/* Finally enable display */
	ssd1306_set_control(display);
	const uint8_t display_on_cmd[] = {
		CMD_SET_DISPLAY_ON(1),
	};
	ssd1306_send_data(display, display_on_cmd, sizeof(display_on_cmd));

	/* Set data line */
	ssd1306_set_data(display);
}
