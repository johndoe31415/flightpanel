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
#include <stm32f4xx_dma.h>
#include <stm32f4xx_spi.h>

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
#include "inconsolata-30.h"
#include "eeprom.h"
#include "stm32f407_adc.h"
#include "bitwise.h"
#include "iomux_pinmap.h"
#include "dsr_tasks.h"
#include "stm32f4xx_debug.h"
#include "usb_hidreport.h"

#define iabs(x)				(((x) < 0) ? -(x) : (x))
#define CMD_BUFFER_SIZE		32
#define KEY_BACKSPACE		0x7f

static const struct gpio_definition_t known_gpio_outputs[] = {
	Display_SCK_GPIO_Definition,
	Display_MOSI_GPIO_Definition,
	Display_RESET_GPIO_Definition,
	Display_DC_GPIO_Definition,
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
	IOMux_SCK_GPIO_Definition,
	IOMux_Out_OE_GPIO_Definition,
	IOMux_Out_STCP_GPIO_Definition,
	IOMux_MOSI_GPIO_Definition,
};
#define KNOWN_GPIO_OUTPUT_COUNT		(sizeof(known_gpio_outputs) / sizeof(known_gpio_outputs[0]))

static const struct gpio_definition_t known_gpio_inputs[] = {
	IOMux_MISO_GPIO_Definition,
	UserButton_GPIO_Definition,
};
#define KNOWN_GPIO_INPUT_COUNT		(sizeof(known_gpio_inputs) / sizeof(known_gpio_inputs[0]))

extern const uint8_t _sflash, _sram, _eram, _ebss;
static enum debugmode_t debug_mode;
static char cmd_input[CMD_BUFFER_SIZE];
static uint8_t cmd_length;
static uint8_t last_cmd_length;
static int debug_accu;
static uint8_t iomux_last_inputs[IOMUX_BYTECOUNT];
static bool execution_running;

void debugmode_set(enum debugmode_t new_mode) {
	debug_mode = new_mode;
	debug_accu = 0;
}

static void dump_debug_registers(void) {
	const int loopcount = 200;
	timing_start();
	delay_loopcnt(loopcount);
	uint32_t cycles = timing_end();
	printf("%lu cycles per delay_loopcnt(%d), i.e., %ld cycles/loopcnt\n", cycles, loopcount, cycles / loopcount);
}

static void iomux_check_inputs(void) {
	uint8_t iomux_inputs[IOMUX_BYTECOUNT];
	memcpy(iomux_inputs, iomux_input_array(), IOMUX_BYTECOUNT);
	if (!memcmp(iomux_inputs, iomux_last_inputs, IOMUX_BYTECOUNT)) {
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
			printf(" %c%d", new_value ? '+' : '-', i);
			const struct iomux_pin_t *pin = iomux_get_input_description(i);
			if (pin) {
				printf(" (%s)", pin->name);
			}
		}
	}
	printf("\n");
	memcpy(iomux_last_inputs, iomux_inputs, IOMUX_BYTECOUNT);
}

static void iomux_set_outputs(void) {
	const int id_bit_count = 7;
	const int id_tick_count = (id_bit_count * 4);
	const int total_tick_count = id_tick_count * 3;
	debug_accu = (debug_accu + 1) % total_tick_count;
	if (debug_accu < id_tick_count) {
		/* Output IDs */
		int bitno = debug_accu / 4;
		int bitpos = debug_accu % 4;
		if (bitpos == 0) {
			iomux_output_setall(0xff);
		} else if (bitpos == 1) {
			/* Output big endian */
			bitno = (id_bit_count - 1) - bitno;
			for (int i = 0; i < IOMUX_OUTPUTS; i++) {
				bool bit_value = (i & (1 << bitno)) != 0;
				iomux_output_set(i, bit_value);
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
	blit_string_to_cursor(&font_inconsolata_30, text, surface, &cursor, false);

	sprintf(text, "%3d", counter++);
	cursor = (struct cursor_t){ 5, 60 };
	blit_string_to_cursor(&font_vcr_osd_mono_20, text, surface, &cursor, false);

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
			fprintf(stderr, "GPIO P%s = %s", known_gpio_outputs[debug_accu].pin_name, known_gpio_outputs[debug_accu].name);
			if (known_gpio_outputs[debug_accu].connect) {
				fprintf(stderr, " : %s", known_gpio_outputs[debug_accu].connect);
			}
			break;

		case DEBUG_IOMUX_INPUTS:
			fprintf(stderr, "IOMux IN");
			break;

		case DEBUG_IOMUX_OUTPUTS:
			fprintf(stderr, "IOMux OUT %d", debug_accu);
			break;

		case DEBUG_IOMUX_OUTPUTS_BLINK:
			fprintf(stderr, "IOMux OUT Blink");
			break;

		case DEBUG_IOMUX_OUTPUT_PATTERN:
			fprintf(stderr, "IOMux Pattern");
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
			known_gpio_outputs[debug_accu].gpio->ODR ^= (1 << known_gpio_outputs[debug_accu].pin_source);
			break;

		case DEBUG_IOMUX_INPUTS:
			iomux_check_inputs();
			break;

		case DEBUG_IOMUX_OUTPUTS:
			iomux_output_toggle(debug_accu);
			break;

		case DEBUG_IOMUX_OUTPUTS_BLINK:
			debug_accu++;
			if (debug_accu == 1000) {
				debug_accu = 0;
			}
			iomux_output_setall(debug_accu > 500 ? 0xff : 0x00);
			break;

		case DEBUG_IOMUX_OUTPUT_PATTERN:
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

static void dump_dma_status(const char *name, DMA_TypeDef *dma, DMA_Stream_TypeDef *dma_streams) {
	printf("Status of controller %s (%p, streams at %p):\n", name, dma, dma_streams);
	for (int i = 0; i < 8; i++) {
		void *peripheral_addr = (void*)dma_streams[i].PAR;
		void *memory_addr = (void*)dma_streams[i].M0AR;
		bool peripheral_inc = GET_BIT(dma_streams[i].CR, 9);
		bool memory_inc = GET_BIT(dma_streams[i].CR, 10);
		uint8_t direction = GET_BITS(dma_streams[i].CR, 6, 2);
		uint8_t channel = GET_BITS(dma_streams[i].CR, 25, 3);
		const char *direction_str = "?";
		if (direction == 0) {
			direction_str = "->";
		} else if (direction == 1) {
			direction_str = "<-";
		} else if (direction == 2) {
			direction_str = "MM";
		}
		printf("  DMA1_Stream%d Ch%d: %-8s %p%s %s %p%s", i, channel, (dma_streams[i].CR & 1) ? "Enabled" : "Disabled", peripheral_addr, peripheral_inc ? "++" : "", direction_str, memory_addr, memory_inc ? "++" : "");

		uint32_t status = (i < 4) ? dma->LISR : dma->HISR;
		uint8_t bit_offset = (i % 4) * 6;
		if (bit_offset >= 12) {
			bit_offset += 4;
		}
		bool FEIF = GET_BIT(status, bit_offset + 0);
		bool DMEIF = GET_BIT(status, bit_offset + 2);
		bool TEIF = GET_BIT(status, bit_offset + 3);
		bool HTIF = GET_BIT(status, bit_offset + 4);
		bool TCIF = GET_BIT(status, bit_offset + 5);
		if (FEIF) printf(" FE");
		if (DMEIF) printf(" DME");
		if (TEIF) printf(" TE");
		if (HTIF) printf(" HT");
		if (TCIF) printf(" TC");
		printf("\n");
	}
}

static void dump_spi_status(const char *name, SPI_TypeDef *spi) {
	printf("%s:", name);
	if (spi->SR & (1 << 8)) {
		printf(" FRE");
	}
	if (spi->SR & SPI_I2S_FLAG_BSY) {
		printf(" BSY");
	}
	if (spi->SR & SPI_I2S_FLAG_OVR) {
		printf(" OVR");
	}
	if (spi->SR & (1 << 5)) {
		printf(" MODF");
	}
	if (spi->SR & (1 << 4)) {
		printf(" CRCERR");
	}
	if (spi->SR & (1 << 3)) {
		printf(" UDR");
	}
	if (spi->SR & SPI_I2S_FLAG_TXE) {
		printf(" TXE");
	}
	if (spi->SR & SPI_I2S_FLAG_RXNE) {
		printf(" RXNE");
	}
	printf("\n");
}

static void debugconsole_reset_gpios(const struct gpio_definition_t *gpios, unsigned int io_count, bool make_output) {
	for (int i = 0; i < io_count; i++) {
		/* Change possible AF setting of this GPIO to regular output */
		BIT_PATCH_REGISTER(gpios[i].gpio->MODER, 2 * gpios[i].pin_source, 2, make_output ? 1 : 0);

		/* Also change the speed rating to slow */
		BIT_PATCH_REGISTER(gpios[i].gpio->OSPEEDR, 2 * gpios[i].pin_source, 2, 0);
	}
}

static void debugconsole_print_gpio(void) {
	printf("%d: Now debugging GPIO output P%s (%s).", debug_accu, known_gpio_outputs[debug_accu].pin_name, known_gpio_outputs[debug_accu].name);
	if (known_gpio_outputs[debug_accu].comment) {
		printf(" %s.", known_gpio_outputs[debug_accu].comment);
	}
	if (known_gpio_outputs[debug_accu].connect) {
		printf(" Connected to %s.", known_gpio_outputs[debug_accu].connect);
	}
	printf("\n");
}

static const char *get_moder_str(uint8_t moder) {
	switch (moder) {
		case 0: return "In";
		case 1: return "Out";
		case 2: return "AF";
		case 3: return "Ana";
		default: return "?";
	}
}

static const char *get_speed_str(uint8_t speed) {
	switch (speed) {
		case 0: return "2 MHz";
		case 1: return "25 MHz";
		case 2: return "50 MHz";
		case 3: return "100 MHz";
		default: return "?";
	}
}

static void dump_io(const struct gpio_definition_t *gpios, unsigned int io_count, const char *text, bool dump_outputs) {
	printf("%d known GPIO %ss:\n", io_count, text);
	for (int i = 0; i < io_count; i++) {
		uint8_t moder = GET_BITS(gpios[i].gpio->MODER, 2 * gpios[i].pin_source, 2);
		uint8_t speed = GET_BITS(gpios[i].gpio->OSPEEDR, 2 * gpios[i].pin_source, 2);
		bool state;
		if (dump_outputs) {
			state = GET_BIT(gpios[i].gpio->ODR, gpios[i].pin_source);
		} else {
			state = GET_BIT(gpios[i].gpio->IDR, gpios[i].pin_source);
		}
		printf("  %2d: P%-3s %3s %7s %s %-14s %-15s %s\n", i, gpios[i].pin_name, get_moder_str(moder), get_speed_str(speed), state ? "HI" : "lo", gpios[i].name, gpios[i].comment ? gpios[i].comment : "", gpios[i].connect ? gpios[i].connect : "");
	}
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
		printf("    gpio-out   Toggle GPIO outputs. Repeat command to switch active output.\n");
		printf("    memory     Show memory statistics\n");
		printf("    iomux-in   Dump IOMultiplexer inputs\n");
		printf("    iomux-out  Debug IOMux outputs one-by-one. Repeat command to switch active output.\n");
		printf("    iomux-blnk Debug IOMux outputs by blinking everything.\n");
		printf("    iomux-ptr  Output test pattern on IOMultiplexer outputs\n");
		printf("    display    Reset OLED displays and output test text\n");
		printf("    delay      Issue a 10000 count delay_loopcount() and output on blue LED (PD15)\n");
		printf("    adc        Gather environmental data (supply voltage, temperature) measured via ADC\n");
		printf("    spi        Show overview of SPI status\n");
		printf("    dma        Show overview of DMA status\n");
		printf("    usb        Show some minimal information about USB subunit\n");
		printf("    debug      Dump debug register contents\n");
		printf("    reset      Reset the MCU entirely\n");
	} else if (!strcmp(cmd_input, "off")) {
		debugmode_set(DEBUG_DISABLED);
	} else if (!strcmp(cmd_input, "info")) {
		debug_show_all();
	} else if (!strcmp(cmd_input, "rs232-isr")) {
		debugmode_set(DEBUG_RS232_ISR);
	} else if (!strcmp(cmd_input, "rs232-echo")) {
		debugmode_set(DEBUG_RS232_ECHO);
	} else if (!strcmp(cmd_input, "eeprom")) {
		bool success = eeprom_dump(4);
		printf("EEPROM dump %s.\n", success ? "successful" : "had a problem");
	} else if (!strcmp(cmd_input, "listio")) {
		dump_io(known_gpio_outputs, KNOWN_GPIO_OUTPUT_COUNT, "output", true);
		dump_io(known_gpio_inputs, KNOWN_GPIO_INPUT_COUNT, "input", false);
	} else if (!strcmp(cmd_input, "gpio-out")) {
		if (debug_mode != DEBUG_GPIO_OUTPUTS) {
			printf("Warning: Changing pin functionality to OUTPUT, i.e., disabling alternate function.\n");
			printf("Also all DMA transfers will be stopped; neither AF nor DMA will be restored.\n");
			printf("You *will* need to reset the MCU after this test.\n");
			printf("\n");
			iomux_disable();
			debugconsole_reset_gpios(known_gpio_inputs, KNOWN_GPIO_INPUT_COUNT, false);
			debugconsole_reset_gpios(known_gpio_outputs, KNOWN_GPIO_OUTPUT_COUNT, true);
			debugmode_set(DEBUG_GPIO_OUTPUTS);
		} else {
			/* Increase pin */
			debug_accu = (debug_accu + 1) % KNOWN_GPIO_OUTPUT_COUNT;
		}
		debugconsole_print_gpio();
	} else if (!strcmp(cmd_input, "memory")) {
		debug_show_memory();
		printf("Program memory (_sflash) = %p, start of RAM (_sram) = %p, end of RAM (_eram) = %p\n", &_sflash, &_sram, &_eram);
		printf("_ebss = %p, Sbrk = %p (malloc used %d bytes of memory) -> %d bytes of heap total\n", &_ebss, sbrk(0), (int)sbrk(0) - (int)&_ebss, (int)sbrk(0) - (int)&_sram);
	} else if (!strcmp(cmd_input, "iomux-in")) {
		debugmode_set(DEBUG_IOMUX_INPUTS);
	} else if (!strcmp(cmd_input, "iomux-out")) {
		if (debug_mode != DEBUG_IOMUX_OUTPUTS) {
			debugmode_set(DEBUG_IOMUX_OUTPUTS);
		} else {
			debug_accu = (debug_accu + 1) % IOMUX_OUTPUTS;
		}
		iomux_output_setall(0);
	} else if (!strcmp(cmd_input, "iomux-blnk")) {
		debugmode_set(DEBUG_IOMUX_OUTPUTS_BLINK);
	} else if (!strcmp(cmd_input, "iomux-ptr")) {
		debugmode_set(DEBUG_IOMUX_OUTPUT_PATTERN);
	} else if (!strcmp(cmd_input, "display")) {
//		init_displays();
		debugmode_set(DEBUG_DISPLAY);
	} else if (!strcmp(cmd_input, "delay")) {
		debugmode_set(DEBUG_DELAY);
	} else if (!strcmp(cmd_input, "adc")) {
		debugmode_set(DEBUG_ADC_TELEMETRY);
	} else if (!strcmp(cmd_input, "spi")) {
		dump_spi_status("SPI1", SPI1);
		dump_spi_status("SPI2", SPI2);
		dump_spi_status("SPI3", SPI3);
	} else if (!strcmp(cmd_input, "dma")) {
		dump_dma_status("DMA1", DMA1, DMA1_Stream0);
	} else if (!strcmp(cmd_input, "usb")) {
		printf("sizeof(struct hid_report_t) = %u bytes\n", sizeof(struct hid_report_t));
	} else if (!strcmp(cmd_input, "debug")) {
		dump_debug_registers();
	} else if (!strcmp(cmd_input, "reset")) {
		stm32f4xx_reset();
	} else if (!strcmp(cmd_input, "+")) {
		if (debug_mode == DEBUG_GPIO_OUTPUTS) {
			debug_accu = (debug_accu + 1) % KNOWN_GPIO_OUTPUT_COUNT;
			debugconsole_print_gpio();
		} else if (debug_mode == DEBUG_IOMUX_OUTPUTS) {
			debug_accu = (debug_accu + 1) % IOMUX_OUTPUTS;
			iomux_output_setall(0);
		}
	} else if (!strcmp(cmd_input, "-")) {
		if (debug_mode == DEBUG_GPIO_OUTPUTS) {
			debug_accu = (debug_accu + KNOWN_GPIO_OUTPUT_COUNT - 1) % KNOWN_GPIO_OUTPUT_COUNT;
			debugconsole_print_gpio();
		} else if (debug_mode == DEBUG_IOMUX_OUTPUTS) {
			debug_accu = (debug_accu + IOMUX_OUTPUTS - 1) % IOMUX_OUTPUTS;
			iomux_output_setall(0);
		}
	} else {
		printf("Unrecognized command '%s'.\n", cmd_input);
	}
	cmd_length = 0;
	execution_running = false;
}

void dsr_execute_debug_command(void) {
	debugconsole_execute();
	debugconsole_print_prompt();
}

void debugconsole_rxchar(uint8_t rxchar) {
	if (execution_running) {
		/* Do not accept new characters while execution in progress */
		return;
	}
	if (debug_mode == DEBUG_RS232_ECHO) {
		fprintf(stderr, "[%02x]", rxchar);
	}
	if (rxchar == '\r') {
		printf("\n");
		if (cmd_length == 0) {
			/* Repeat last command */
			cmd_length = last_cmd_length;
		} else {
			last_cmd_length = cmd_length;
		}
		execution_running = true;
		dsr_mark_pending(DSR_TASK_EXECUTE_DEBUG_COMMAND);
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

