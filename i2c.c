#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stm32f4xx_i2c.h>
#include "i2c.h"

void i2c_stop(I2C_TypeDef *I2Cx) {
	I2C_GenerateSTOP(I2Cx, ENABLE);
}

bool i2c_start(I2C_TypeDef *I2Cx, int address, bool do_read) {
	I2C_ClearFlag(I2Cx, I2C_FLAG_AF);
	I2C_AcknowledgeConfig(I2Cx, ENABLE);
	I2C_GenerateSTART(I2Cx, ENABLE);

	// Check for fulfilled START condition
	int timeout = 5000;
	while ((!I2C_GetFlagStatus(I2Cx, I2C_FLAG_SB)) && timeout) {
		timeout--;
	}
	if (timeout == 0) {
		return false;
	}

	// Check for ACKed START
	timeout = 5000;
	while ((!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT)) && timeout) {
		timeout--;
	}
	if (timeout == 0) {
		return false;
	}

	// Try to select slave
	I2C_Send7bitAddress(I2Cx, (address & 0xfe) | (do_read ? 1 : 0), do_read ? I2C_Direction_Receiver : I2C_Direction_Transmitter);

	if (do_read) {
		/* Wait for either addressing or acknowledge failure */
		while (!(I2C_GetFlagStatus(I2Cx, I2C_FLAG_ADDR) || I2C_GetFlagStatus(I2Cx, I2C_FLAG_AF)));

		if (I2C_GetFlagStatus(I2Cx, I2C_FLAG_AF)) {
			/* ACK failure, STOP */
			I2C_GenerateSTOP(I2Cx, ENABLE);
			return false;
		}
	}

	/* Wait for receiver or transmitter mode */
	timeout = 5000;
	if (do_read) {
		while ((!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)) && timeout--);
	} else {
		while ((!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) && timeout--);
	}
	if (timeout == 0) {
		i2c_stop(I2Cx);
		return false;
	}
	return true;
}

int i2c_receive_byte(I2C_TypeDef *I2Cx, bool ack_byte) {
	int timeout = 5000;
	I2C_AcknowledgeConfig(I2Cx, ack_byte);
	while ((!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED)) && timeout) {
		timeout--;
	}
	if (timeout == 0) {
		i2c_stop(I2Cx);
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

void i2c_transmit_byte(I2C_TypeDef *I2Cx, uint8_t data) {
	I2C_SendData(I2Cx, data);
	while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
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
