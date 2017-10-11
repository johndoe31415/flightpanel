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

static const struct configuration default_configuration = {
	.xpdr = {
		.ident_timeout_milliseconds = 3000,
		.edit_timeout_milliseconds = 2000,
		.vfr_squawk = 7000,
	},
	.instruments = {
		.ap = {
			.ias = 120,
			.altitude = 20,
			.climbrate = 7,
		},
		.squawk = 7000,
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

