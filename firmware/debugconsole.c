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
#include <unistd.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "debugconsole.h"
#include "pinmap.h"
#include "rs232.h"
#include "debug.h"
#include "stm32f4xx_reset.h"
#include "iomux.h"
#include "displays.h"
#include "surface.h"
#include "font.h"
#include "vcr-osd-mono-20.h"
#include "vcr-osd-mono-30.h"
#include "eeprom.h"
#include "stm32f407_adc.h"

#define iabs(x)				(((x) < 0) ? -(x) : (x))
#define CMD_BUFFER_SIZE		32
#define KEY_BACKSPACE		0x7f

static const struct gpio_definition_t known_gpios[] = {
	Display1_CS_GPIO_Definition,
	Display2_CS_GPIO_Definition,
	Display3_CS_GPIO_Definition,
	Display4_CS_GPIO_Definition,
	Display5_CS_GPIO_Definition,
	Display6_CS_GPIO_Definition,
	Display7_CS_GPIO_Definition,
	Display8_CS_GPIO_Definition,
	Display9_CS_GPIO_Definition,
	Display10_CS_GPIO_Definition,
	Display11_CS_GPIO_Definition,
	Display12_CS_GPIO_Definition,
};
#define KNOWN_GPIO_COUNT		(sizeof(known_gpios) / sizeof(known_gpios[0]))

enum debugmode_t {
	DEBUG_DISABLED = 0,
	DEBUG_RS232_ISR,
	DEBUG_RS232_ECHO,
	DEBUG_GPIO_OUTPUTS,
	DEBUG_IOMUX_INPUTS,
	DEBUG_IOMUX_OUTPUTS,
	DEBUG_DISPLAY,
	DEBUG_DELAY,
	DEBUG_ADC_TELEMETRY,
};

static enum debugmode_t debug_mode;
static char cmd_input[CMD_BUFFER_SIZE];
static uint8_t cmd_length;
static int debug_accu;
static uint8_t iomux_last_inputs[IOMUX_BYTECOUNT];

static void iomux_check_inputs(void) {
	uint8_t iomux_inputs[IOMUX_BYTECOUNT];
	memcpy(iomux_inputs, iomux_input_array(), IOMUX_BYTECOUNT);
	if (memcmp(iomux_inputs, iomux_last_inputs, IOMUX_BYTECOUNT)) {
		/* Nothing changed. */
		return;
	}

	for (int i = 0; i < IOMUX_OUTPUTS; i++) {
		bool new_value = iomux_get_input_from(iomux_inputs, i);
		if (i && ((i % 8) == 0)) {
			printf("  ");
		}
		printf("%c", new_value ? '1' : '0');
	}
	printf("  ");
	for (int i = 0; i < IOMUX_OUTPUTS; i++) {
		bool old_value = iomux_get_input_from(iomux_last_inputs, i);
		bool new_value = iomux_get_input_from(iomux_inputs, i);
		if (old_value != new_value) {
			printf("%c%d", new_value ? '+' : '-', i);
		}
	}
	printf("\n");
}

static void iomux_set_outputs(void) {
	const int id_bit_count = 7;
	const int id_tick_count = (id_bit_count * 3);
	const int total_tick_count = id_tick_count * 3;
	debug_accu = (debug_accu + 1) % total_tick_count;
	if (debug_accu < id_tick_count) {
		/* Output IDs */
		int bitno = debug_accu / 3;
		int bitpos = debug_accu % 3;
		if (bitpos == 0) {
			iomux_output_setall(0xff);
		} else if (bitpos == 1) {
			/* Output big endian */
			bitno = (id_bit_count - 1) - bitno;
			for (int i = 0; i < IOMUX_OUTPUTS; i++) {
				iomux_output_set(i, i & (1 << bitno));
			}
		} else {
			iomux_output_setall(0);
		}
	} else {
		/* Pause */
		iomux_output_setall(0);
	}
}

static void display_debug(void) {
	static uint8_t counter;
	debug_accu = (debug_accu + 1) % DISPLAY_COUNT;

	const struct surface_t *surface = displays_get_surface(debug_accu);

	surface_clear(surface);
	surface_draw_border(surface);

	struct cursor_t cursor = { 5, 35 };
	char text[16];
	sprintf(text, "%2d", debug_accu);
	blit_string_to_cursor(&font_vcr_osd_mono_30, text, surface, &cursor);

	sprintf(text, "%3d", counter++);
	cursor = (struct cursor_t){ 5, 60 };
	blit_string_to_cursor(&font_vcr_osd_mono_20, text, surface, &cursor);

	display_mark_surface_dirty(debug_accu);
}

static void debugconsole_print_prompt(void) {
	fprintf(stderr, "[");
	switch (debug_mode) {
		case DEBUG_DISABLED:
			fprintf(stderr, "off");
			break;

		case DEBUG_RS232_ISR:
			fprintf(stderr, "RS232 ISR");
			break;

		case DEBUG_RS232_ECHO:
			fprintf(stderr, "RS232 echo");
			break;

		case DEBUG_GPIO_OUTPUTS:
			fprintf(stderr, "GPIO P%s = %s", known_gpios[debug_accu].pin_name, known_gpios[debug_accu].name);
			if (known_gpios[debug_accu].connect) {
				fprintf(stderr, " : %s", known_gpios[debug_accu].connect);
			}
			break;

		case DEBUG_IOMUX_INPUTS:
			fprintf(stderr, "IOMux IN");
			break;

		case DEBUG_IOMUX_OUTPUTS:
			fprintf(stderr, "IOMux OUT");
			break;

		case DEBUG_DISPLAY:
			fprintf(stderr, "Display");
			break;

		case DEBUG_DELAY:
			fprintf(stderr, "Delay");
			break;

		case DEBUG_ADC_TELEMETRY:
			fprintf(stderr, "ADC");
			break;

		default:
			fprintf(stderr, "? (%d)", debug_mode);
	}
	fprintf(stderr, "]> ");
}

void debugconsole_tick(void) {
	if (debug_mode == DEBUG_DISABLED) {
		return;
	}
	switch (debug_mode) {
		case DEBUG_DISABLED:
		case DEBUG_RS232_ECHO:
			// Just here to omit warning
			return;

		case DEBUG_RS232_ISR:
			rs232_debug_setleds();
			break;

		case DEBUG_GPIO_OUTPUTS:
			known_gpios[debug_accu].gpio->ODR ^= known_gpios[debug_accu].pin;
			break;

		case DEBUG_IOMUX_INPUTS:
			iomux_check_inputs();
			break;

		case DEBUG_IOMUX_OUTPUTS:
			iomux_set_outputs();
			break;

		case DEBUG_DISPLAY:
			display_debug();
			break;

		case DEBUG_DELAY:
			LEDBlue_set_ACTIVE();
			delay_loopcnt(10000);
			LEDBlue_set_INACTIVE();
			break;

		case DEBUG_ADC_TELEMETRY:
			debug_accu++;
			if (debug_accu == 1000) {
				debug_accu = 0;
				struct adc_telemetry_t telemetry = adc_get_telemetry();
				printf("Vdd = %lu.%03lu V, temperature %d.%d °C, %lu µV at 30°C, slope %lu µV per °C\n", telemetry.vref_millivolts / 1000, telemetry.vref_millivolts % 1000, telemetry.temperature_10th_deg_c / 10, iabs(telemetry.temperature_10th_deg_c) % 10, telemetry.tempsensor_offset_microvolts_at_30_deg_c, telemetry.tempsensor_slope_microvolts_per_deg_c);
			}
	}
}

static void debugconsole_print_gpio(void) {
	printf("Now debugging GPIO output P%s (%s).", known_gpios[debug_accu].pin_name, known_gpios[debug_accu].name);
	if (known_gpios[debug_accu].comment) {
		printf(" %s.", known_gpios[debug_accu].comment);
	}
	if (known_gpios[debug_accu].connect) {
		printf(" Connected to %s.", known_gpios[debug_accu].connect);
	}
	printf("\n");
}

static void debugconsole_execute(void) {
	if (cmd_length <= CMD_BUFFER_SIZE - 1) {
		cmd_input[cmd_length] = 0;
	} else {
		cmd_input[CMD_BUFFER_SIZE - 1] = 0;
	}
	if (!strcmp(cmd_input, "?") || !strcmp(cmd_input, "help")) {
		printf("    off        Turn all debugging off\n");
		printf("    info       Show device information\n");
		printf("    rs232-isr  RS232 ISR debugging (green = TX in progress, blue = in ISR, red = buffer not empty)\n");
		printf("    rs232-echo Echo the recevied characters on the RS232 interface as hex\n");
		printf("    eeprom     Dump EEPROM contents\n");
		printf("    listio     List supported GPIOs\n");
		printf("    gpio-out   Toggle GPIO outputs\n");
		printf("    memory     Show memory statistics\n");
		printf("    iomux-in   Dump IOMultiplexer inputs\n");
		printf("    iomux-out  Output test pattern on IOMultiplexer outputs\n");
		printf("    display    Reset OLED displays and output test text\n");
		printf("    delay      Issue a 10000 count delay_loopcount() and output on blue LED (PD15)\n");
		printf("    adc        Gather environmental data (supply voltage, temperature) measured via ADC");
		printf("    reset      Reset the MCU entirely\n");
	} else if (!strcmp(cmd_input, "off")) {
		debug_mode = DEBUG_DISABLED;
	} else if (!strcmp(cmd_input, "info")) {
		debug_show_all();
	} else if (!strcmp(cmd_input, "rs232-isr")) {
		debug_mode = DEBUG_RS232_ISR;
	} else if (!strcmp(cmd_input, "rs232-echo")) {
		debug_mode = DEBUG_RS232_ECHO;
	} else if (!strcmp(cmd_input, "eeprom")) {
		bool success = eeprom_dump(4);
		printf("EEPROM dump %s.\n", success ? "successful" : "had a problem");
	} else if (!strcmp(cmd_input, "listio")) {
		printf("%d known GPIOs:\n", KNOWN_GPIO_COUNT);
		for (int i = 0; i < KNOWN_GPIO_COUNT; i++) {
			printf("  %2d: P%-3s %-14s %-10s %s\n", i, known_gpios[i].pin_name, known_gpios[i].name, known_gpios[i].comment ? known_gpios[i].comment : "", known_gpios[i].connect ? known_gpios[i].connect : "");
		}
	} else if (!strcmp(cmd_input, "gpio-out")) {
		debug_accu = 0;
		debug_mode = DEBUG_GPIO_OUTPUTS;
		debugconsole_print_gpio();
	} else if (!strcmp(cmd_input, "memory")) {
		debug_show_memory();
	} else if (!strcmp(cmd_input, "iomux-in")) {
		debug_mode = DEBUG_IOMUX_INPUTS;
	} else if (!strcmp(cmd_input, "iomux-out")) {
		debug_accu = 0;
		debug_mode = DEBUG_IOMUX_OUTPUTS;
	} else if (!strcmp(cmd_input, "display")) {
		init_displays();
		debug_accu = 0;
		debug_mode = DEBUG_DISPLAY;
	} else if (!strcmp(cmd_input, "delay")) {
		debug_mode = DEBUG_DELAY;
	} else if (!strcmp(cmd_input, "adc")) {
		debug_accu = 0;
		debug_mode = DEBUG_ADC_TELEMETRY;
	} else if (!strcmp(cmd_input, "reset")) {
		stm32f4xx_reset();
	} else if (cmd_length == 0) {
		if (debug_mode == DEBUG_GPIO_OUTPUTS) {
			debug_accu = (debug_accu + 1) % KNOWN_GPIO_COUNT;
			debugconsole_print_gpio();
		}
	} else {
		printf("Unrecognized command '%s'.\n", cmd_input);
	}
	cmd_length = 0;
}

void debugconsole_rxchar(uint8_t rxchar) {
	if (debug_mode == DEBUG_RS232_ECHO) {
		fprintf(stderr, "[%02x]", rxchar);
	}
	if (rxchar == '\r') {
		printf("\n");
		debugconsole_execute();
		debugconsole_print_prompt();
	} else if (rxchar == KEY_BACKSPACE) {
		if (cmd_length > 0) {
			cmd_length--;
			fprintf(stderr, "\x08 \x08");			/* Remove character */
		}
	} else if ((rxchar >= 32) && (rxchar <= 128)) {
		if (cmd_length < CMD_BUFFER_SIZE) {
			cmd_input[cmd_length] = rxchar;
			cmd_length++;
			rs232_transmitchar(rxchar);				/* Local echo */
		}
	}
}

