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
#include <string.h>
#include <stm32f4xx_crc.h>
#include "configuration.h"
#include "eeprom.h"
#include "frequencies.h"
#include "usb_hidreport.h"

static const struct configuration default_configuration = {
	.xpdr = {
		.ident_timeout_milliseconds = 3000,
		.edit_timeout_milliseconds = 2000,
		.vfr_squawk = 7000,
	},
	.instruments = {
		.tx_radio_id = 1,
		.com_frequency_divisions = COM_RANGE_25KHZ,
		.nav_frequency_divisions = NAV_RANGE,
		.nav_ident_inhibit_timeout = 3000,
		.ap = {
			.altitude = 2000,
			.ias = 120,
			.climbrate = 700,
		},
		.com1 = {
			.active_frequency_khz = 118000,
			.standby_frequency_khz = 119000,
		},
		.com2 = {
			.active_frequency_khz = 120000,
			.standby_frequency_khz = 121000,
		},
		.nav1 = {
			.active_frequency_khz = 108000,
			.standby_frequency_khz = 109000,
		},
		.nav2 = {
			.active_frequency_khz = 110000,
			.standby_frequency_khz = 111000,
		},
		.squawk = 7000,
		.qnh = 1000,
	},
	.misc = {
		.time_to_blank_milliseconds = 15 * 60 * 1000,
	},
};
_Static_assert((sizeof(default_configuration) % 4) == 0, "Configuration size alignment mismatch.");

struct configuration active_configuration;

static uint32_t calculate_config_crc(const struct configuration *config) {
	CRC_ResetDR();
	return CRC_CalcBlockCRC((uint32_t*)config, (sizeof(default_configuration) / 4) - 1);
}

void write_configuration(struct configuration *config) {
	config->crc32 = calculate_config_crc(config);
	eeprom_write(0, config, sizeof(struct configuration));
}

bool read_configuration(struct configuration *config) {
	bool from_eeprom = true;
	eeprom_read(0, config, sizeof(struct configuration));
	if (config->crc32 != calculate_config_crc(config)) {
		memcpy(config, &default_configuration, sizeof(struct configuration));
		write_configuration(config);
		from_eeprom = false;
	}
	return from_eeprom;
}

void init_configuration(void) {
	read_configuration(&active_configuration);
}

