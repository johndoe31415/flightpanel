#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

#include "i2c.h"
#include "eeprom.h"

#define EEPROM_ADDR				0xa0

static bool eeprom_set_address(uint16_t address) {
	if (!i2c_start(I2C1, EEPROM_ADDR, false)) {
		return false;
	}
	i2c_transmit_byte(I2C1, (address >> 8) & 0xff);
	i2c_transmit_byte(I2C1, (address >> 0) & 0xff);
	i2c_stop(I2C1);
	return true;
}

bool eeprom_write(uint16_t address, const void *data, uint16_t length) {
	if (!i2c_start(I2C1, EEPROM_ADDR, false)) {
		return false;
	}
	i2c_transmit_byte(I2C1, (address >> 8) & 0xff);
	i2c_transmit_byte(I2C1, (address >> 0) & 0xff);
	for (int i = 0; i < length; i++) {
		i2c_transmit_byte(I2C1, ((const uint8_t*)data)[i]);
	}
	i2c_stop(I2C1);
	return true;
}

bool eeprom_read(uint16_t address, void *data, uint16_t length) {
	if (!eeprom_set_address(address)) {
		return false;
	}
	if (!i2c_start(I2C1, EEPROM_ADDR, true)) {
		return false;
	}
	return i2c_receive_bytes(I2C1, (uint8_t*)data, length);
}

bool eeprom_dump(int pagecnt) {
	uint8_t buffer[32];
	int address = 0;
	for (int page = 0; page < pagecnt; page++) {
		if (!eeprom_read(address, buffer, sizeof(buffer))) {
			return false;
		}
		printf("%4x: ", address);
		for (int i = 0; i < sizeof(buffer); i++) {
			printf("%02x ", buffer[i]);
		}
		printf("\n");
		address += sizeof(buffer);
	}
	return true;
}
