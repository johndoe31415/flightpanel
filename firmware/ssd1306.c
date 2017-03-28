#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include "ssd1306.h"
#include "pinmap.h"

// Fundamental commands
#define CMD_SET_CONTRAST_CONTROL(value)											0x02, 0x81, (value)
#define CMD_ENTIRE_DISPLAY_ON(on_off)											0x01, 0xa4 | ((on_off) & 0x01)
#define CMD_SET_INVERSE_DISPLAY(on_off)											0x01, 0xa6 | ((on_off) & 0x01)
#define CMD_SET_DISPLAY_ON(on_off)												0x01, 0xae | ((on_off) & 0x01)

// Addressing commands
#define CMD_SET_LOWER_COLUMN_START_ADDR(addr)									0x01, 0x00 | ((value) & 0x0f)
#define CMD_SET_HIGHER_COLUMN_START_ADDR(addr)									0x01, 0x10 | ((value) & 0x0f)
#define CMD_SET_MEMORY_ADDRESSING_MODE_HORIZONTAL								0x02, 0x20, 0x00
#define CMD_SET_MEMORY_ADDRESSING_MODE_VERTICAL									0x02, 0x20, 0x01
#define CMD_SET_MEMORY_ADDRESSING_MODE_PAGE_ADDR								0x02, 0x20, 0x02
#define CMD_SET_COLUMN_START_END_ADDRESS(start, end)							0x03, 0x21, ((start) & 0x7f), ((end) & 0x7f)
#define CMD_SET_PAGE_START_END_ADDRESS(start, end)								0x03, 0x22, ((start) & 0x07), ((end) & 0x07)
#define CMD_SET_PAGE_START_ADDR(addr)											0x01, 0xb0 | ((addr) & 0x07)

// Scrolling commands
#define CMD_DEACTIVATE_SCROLL													0x01, 0x2e
#define CMD_ACTIVATE_SCROLL														0x01, 0x2f

// Hardware configuration
#define CMD_SET_DISPLAY_START_LINE(line)										0x01, 0x40 | ((line) & 0x3f)
#define CMD_SET_SEGMENT_REMAP(on_off)											0x01, 0xa0 | ((on_off) & 0x01)
#define CMD_SET_MULTIPLEX_RATIO(n)												0x02, 0xa8, (((n) - 1) & 0x3f)
#define CMD_SET_COM_OUTPUT_SCAN_DIRECTION_NORMAL								0x01, 0xc0
#define CMD_SET_COM_OUTPUT_SCAN_DIRECTION_REMAPPED								0x01, 0xc8
#define CMD_SET_DISPLAY_OFFSET(offset)											0x02, 0xd3, ((offset) & 0x3f)
#define CMD_SET_COM_PINS_HW_CONFIG(alt_com_pin_on_off, com_lr_remap_on_off)		0x02, 0xda, (0x02 | (((alt_com_pin_on_off) & 0x01) << 4) | (((com_lr_remap_on_off) & 0x01) << 5))
#define CMD_SET_DISPLAY_CLK_DIVIDE(divider, frequency)							0x02, 0xd5, ((((divider) - 1) & 0x0f) | (((frequency) & 0x0f) << 4))
#define CMD_SET_PRECHARGE_PERIOD(phase1, phase2)								0x02, 0xd9, (((phase1) & 0x0f) | (((phase2) & 0x0f) << 4))
#define CMD_SET_VCOMH_DESELECT_LVL_065Vcc										0x02, 0xdb, (0x00 << 4)
#define CMD_SET_VCOMH_DESELECT_LVL_077Vcc										0x02, 0xdb, (0x02 << 4)
#define CMD_SET_VCOMH_DESELECT_LVL_083Vcc										0x02, 0xdb, (0x03 << 4)
#define CMD_NOP																	0x01, 0xe3

// Charge pump command
#define CMD_SET_CHARGEPUMP(on_off)												0x02, 0x8d, (0x10 | ((on_off) & 0x01) << 2)

static const uint8_t initialization_routine[] = {
	CMD_SET_MEMORY_ADDRESSING_MODE_HORIZONTAL,			// 20 00
	CMD_SET_MULTIPLEX_RATIO(0),							// a8 3f

	CMD_SET_DISPLAY_OFFSET(0),							// d3 00
	CMD_SET_DISPLAY_START_LINE(0),						// 40

	CMD_SET_SEGMENT_REMAP(1),							// a1
	CMD_SET_COM_OUTPUT_SCAN_DIRECTION_REMAPPED,			// c8

	CMD_SET_COM_PINS_HW_CONFIG(1, 0),					// da 12

	CMD_SET_CONTRAST_CONTROL(0x7f),						// 81 7f

	CMD_ENTIRE_DISPLAY_ON(0),							// a4
	CMD_SET_INVERSE_DISPLAY(0),							// a6

	CMD_SET_DISPLAY_CLK_DIVIDE(1, 0x8),					// d5 80

	CMD_SET_CHARGEPUMP(1),								// 8d 14

	CMD_SET_DISPLAY_ON(1),								// af
};

static void ssd1306_send_data(const struct ssd1306_display *display) {
	/* D/#C high */
	GPIO_SetBits(Display_DC_GPIO, Display_DC_Pin);
}

static void ssd1306_send_control(const struct ssd1306_display *display) {
	/* D/#C low */
	GPIO_ResetBits(Display_DC_GPIO, Display_DC_Pin);
}

static void ssd1306_send_command(const struct ssd1306_display *display, const uint8_t *data, int length) {
	ssd1306_send_control(display);
	GPIO_ResetBits(display->nSS_GPIO, display->nSS_Pin);
	for (int i = 0; i < length; i++) {
		printf("%02x ", data[i]);
	}
	printf("\n");
	GPIO_SetBits(display->nSS_GPIO, display->nSS_Pin);
}

void ssd1306_init(const struct ssd1306_display *display) {
	for (int i = 0; i < sizeof(initialization_routine); i++) {
		uint8_t length = initialization_routine[i];
		ssd1306_send_command(display, initialization_routine + i + 1, length);
		i += length;
	}
}

