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
#include <stdint.h>
#include <stdbool.h>
#include <stm32f4xx_i2c.h>
#include <stm32f4xx_gpio.h>
#include "i2c.h"

// Set this to 1 for prod
#define FACTOR		4

static bool i2c_wait_for_event(I2C_TypeDef *I2Cx, uint32_t event) {
	int timeout = 75 * FACTOR;
	bool success;
	while (!(success = I2C_CheckEvent(I2Cx, event))) {
		timeout--;
		if (timeout == 0) {
			break;
		}
	}
#ifdef DEBUG_I2C
	if (!success) {
		printf("I2C Timeout: ");
		if (event == I2C_EVENT_MASTER_MODE_SELECT) {
			printf("I2C_EVENT_MASTER_MODE_SELECT\n");
		} else if (event == I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED) {
			printf("I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED\n");
		} else if (event == I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED) {
			printf("I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED\n");
		} else if (event == I2C_EVENT_MASTER_BYTE_RECEIVED) {
			printf("I2C_EVENT_MASTER_BYTE_RECEIVED\n");
		} else if (event == I2C_EVENT_MASTER_BYTE_TRANSMITTED) {
			printf("I2C_EVENT_MASTER_BYTE_TRANSMITTED\n");
		} else {
			printf("%lx\n", event);
		}
	}
#endif
	return success;
}

#ifdef DEBUG_I2C
void i2c_dumpflags(I2C_TypeDef *I2Cx, const char *msg) {
	printf("Flags (%s): ", msg);
	printf("%s", I2C_GetFlagStatus(I2Cx, I2C_FLAG_AF) ? "AF " : "");
	printf("%s", I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY) ? "BUSY " : "");
	printf("%s", I2C_GetFlagStatus(I2Cx, I2C_FLAG_BTF) ? "BTF " : "");
	printf("\n");
}
#endif

bool i2c_start(I2C_TypeDef *I2Cx, int address, bool do_read) {
	I2C_ClearFlag(I2Cx, I2C_FLAG_AF);
	I2C_AcknowledgeConfig(I2Cx, ENABLE);

	// Generate and check for fulfilled START condition
	I2C_GenerateSTART(I2Cx, ENABLE);
	if (!i2c_wait_for_event(I2Cx, I2C_EVENT_MASTER_MODE_SELECT)) {
		// Could not acquire the bus.
		return false;
	}

	// Try to select slave and check if selction successful
	I2C_Send7bitAddress(I2Cx, (address & 0xfe) | (do_read ? 1 : 0), do_read ? I2C_Direction_Receiver : I2C_Direction_Transmitter);
	if (!i2c_wait_for_event(I2Cx, do_read ? I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED : I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) {
		i2c_stop(I2Cx);
		return false;
	}
	return true;
}

void i2c_stop(I2C_TypeDef *I2Cx) {
	I2C_GenerateSTOP(I2Cx, ENABLE);
}

int i2c_receive_byte(I2C_TypeDef *I2Cx, bool ack_byte) {
	I2C_AcknowledgeConfig(I2Cx, ack_byte ? ENABLE : DISABLE);
	if (!i2c_wait_for_event(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED)) {
		return -1;
	}
	if (!ack_byte) {
		i2c_stop(I2Cx);
	}
	return I2C_ReceiveData(I2Cx);
}

bool i2c_receive_bytes(I2C_TypeDef *I2Cx, uint8_t *buffer, int length) {
	for (int i = 0; i < length; i++) {
		bool is_last_byte = (i == (length - 1));
		int value = i2c_receive_byte(I2Cx, !is_last_byte);
		if (value == -1) {
			return false;
		}
		buffer[i] = value;
	}
	return true;
}

bool i2c_transmit_byte(I2C_TypeDef *I2Cx, uint8_t data) {
	I2C_SendData(I2Cx, data);
	return i2c_wait_for_event(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED);
}

void i2c_scanbus(I2C_TypeDef *I2Cx) {
	for (int address = 0; address < 256; address += 2) {
		if (!i2c_start(I2Cx, address, true)) {
			continue;
		}
		i2c_receive_byte(I2Cx, false);
		printf("Found: 0x%x\n", address);
	}
}
