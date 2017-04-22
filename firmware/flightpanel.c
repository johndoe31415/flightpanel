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

#if 0
uint32_t x32 = 314159265;
static uint32_t xorshift32(void)
{
  x32 ^= x32 << 13;
  x32 ^= x32 >> 17;
  x32 ^= x32 << 5;
  return x32;
}
#endif

int main(void) {
	printf("Reset successful.\n");
	init_usb_late();
	printf("USB initialized.\n");


	init_displays();

	while (true) {
		iomux_dump_iochange();
		delay_millis(100);
	}

	instruments_idle_loop();


#if 0
	while (true) {
		xorshift32();
		Dbg1_SetTo(x32 & 0x100);
		for (volatile int i = 0; i < 100; i++) { }
		Dbg2_SetTo(x32 & 1);
		for (volatile int i = 0; i < 100; i++) { }
	}

#endif
//	struct configuration config;
//	read_configuration(&config);
//	eeprom_write(20, "Test", 4);
//	ssd1306_init();
//
#if 0
	struct hid_report_t hid_report = { };
	while (true) {
		hid_report.squawk++;
		usb_submit_report(&hid_report);
		//i2c_scanbus(I2C1);
//		eeprom_dump(10);
		delay_millis(100);
	}
#endif
	return 0;
}


