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
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include <stm32f4xx_gpio.h>
#include <stm32f4xx_spi.h>
#include <stm32f4xx_dma.h>
#include <stm32f4xx_i2c.h>

#include "rs232.h"
#include "rotary.h"
#include "spi.h"
#include "usb.h"
#include "timer.h"
#include "i2c.h"
#include "eeprom.h"
#include "configuration.h"
#include "usbd_hid.h"
#include "usb.h"
#include "displays.h"
#include "instruments.h"
#include "iomux.h"
#include "pinmap.h"
#include "stm32f4xx_devid.h"
#include "stm32f4xx_stackpaint.h"

int main(void) {
#if 0
	char x = 'A';
	while (true) {
		while (UserButton_GetValue()) {
			LEDGreen_SetHIGH();
		}
		LEDGreen_SetLOW();
		rs232_transmitchar(x);
		x++;
		if (x > 'Z') {
			rs232_transmitchar('\r');
			rs232_transmitchar('\n');
			x = 'A';
		}
	}
#endif

	printf("Reset successful.\n");
	init_usb_late();
	printf("USB initialized.\n");

	char ascii_device_id[32];
	stm32fxx_get_ascii_devid(ascii_device_id);
	printf("STM32 Device ID: %s\n", ascii_device_id);
	printf("Compiled with: gcc " __VERSION__ " newlib " _NEWLIB_VERSION " on " BUILD_TIMESTAMP_UTC " UTC\n");

	struct stackpaint_result_t stackpaint = stm32fxx_get_stackpaint();
	printf("%d bytes of heap used, %d bytes of stack used.\n", stackpaint.heap_used, stackpaint.stack_used);

	init_displays();
	instruments_idle_loop();
	return 0;
}


