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
	DEBUG_RS232_BUFFER,
	DEBUG_GPIO_OUTPUTS,
};

static enum debugmode_t debug_mode;
static char cmd_input[CMD_BUFFER_SIZE];
static uint8_t cmd_length;
static int debug_accu;

static void debugconsole_print_prompt(void) {
	fprintf(stderr, "[");
	switch (debug_mode) {
		case DEBUG_DISABLED:
			fprintf(stderr, "off");
			break;

		case DEBUG_RS232_BUFFER:
			fprintf(stderr, "RS232");
			break;

		case DEBUG_GPIO_OUTPUTS:
			fprintf(stderr, "GPIO P%s = %s", known_gpios[debug_accu].pin_name, known_gpios[debug_accu].name);
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
			// Just here to omit warning
			return;

		case DEBUG_RS232_BUFFER:
			rs232_debug_setleds();
			break;

		case DEBUG_GPIO_OUTPUTS:
			known_gpios[debug_accu].gpio->ODR ^= known_gpios[debug_accu].pin;
			break;
	}
}

static void debugconsole_print_gpio(void) {
	printf("Now debugging GPIO output P%s (%s).\n", known_gpios[debug_accu].pin_name, known_gpios[debug_accu].name);
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
		printf("    rs232      Turn on RS232 debug\n");
		printf("    listio     List supported GPIOs\n");
		printf("    gpio-out   Toggle GPIO outputs\n");
		printf("    memory     Show memory statistics\n");
		printf("    reset      Reset the MCU entirely\n");
	} else if (!strcmp(cmd_input, "off")) {
		debug_mode = DEBUG_DISABLED;
	} else if (!strcmp(cmd_input, "info")) {
		debug_show_all();
	} else if (!strcmp(cmd_input, "rs232")) {
		debug_mode = DEBUG_RS232_BUFFER;
	} else if (!strcmp(cmd_input, "listio")) {
		printf("%d known GPIOs:\n", KNOWN_GPIO_COUNT);
		for (int i = 0; i < KNOWN_GPIO_COUNT; i++) {
			printf("  %2d: P%-3s %s\n", i, known_gpios[i].pin_name, known_gpios[i].name);
		}
	} else if (!strcmp(cmd_input, "gpio-out")) {
		debug_accu = 0;
		debug_mode = DEBUG_GPIO_OUTPUTS;
		debugconsole_print_gpio();
	} else if (!strcmp(cmd_input, "memory")) {
		debug_show_memory();
	} else if (!strcmp(cmd_input, "reset")) {
		// TODO: implement reset
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
//	fprintf(stderr, "[%02x]", rxchar);
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

