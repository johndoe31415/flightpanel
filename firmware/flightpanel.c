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

#include "eeprom.h"
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
#include "iomux_pinmap.h"
#include "pinmap.h"
#include "debug.h"
#include "dsr_tasks.h"
#include "stm32f4xx_debug.h"
#include "debugconsole.h"
#include "fault.h"

extern uint8_t _sflash;

int main(void) {
	printf("Reset successful. Running from %p.\n", &_sflash);
	init_usb_late();
	printf("USB initialized.\n");

	debug_show_all();
#ifndef RELEASE_BUILD
	printf("Enabling Cortex-M debug unit.\n");
	timing_init();
#endif

	RGB_LED_Green_set_ACTIVE();
	init_configuration();
	init_displays();
	instruments_init();
	dsr_mark_pending(DSR_TASK_IDLE);
//	debugmode_set(DEBUG_DISPLAY);
	execute_dsr_loop();
	return 0;
}
