#include <stdio.h>
#include <string.h>
#include <stm32f4xx_crc.h>
#include "configuration.h"
#include "eeprom.h"

static const struct configuration default_configuration = {
	.some_value = 123,
};
_Static_assert((sizeof(default_configuration) % 4) == 0, "Configuration size alignment mismatch.");

//static struct configuration active_configuration;

static uint32_t calculate_config_crc(const struct configuration *config) {
	CRC_ResetDR();
	return CRC_CalcBlockCRC((uint32_t*)config, (sizeof(default_configuration) / 4) - 1);
}

void write_configuration(struct configuration *config) {
	printf("WRITE\n");
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

