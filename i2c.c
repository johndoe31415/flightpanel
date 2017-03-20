#include <stdio.h>
#include <stdint.h>
#include <stm32f4xx_i2c.h>
#include "i2c.h"




void i2c_scanbus(I2C_TypeDef *I2Cx) {
	for (int addr = 0; addr < 256; addr += 2) {
		I2C_ClearFlag(I2Cx, I2C_FLAG_AF);
		I2C_AcknowledgeConfig(I2Cx, ENABLE);
		I2C_GenerateSTART(I2Cx, ENABLE);

		// Check for fulfilled START condition
		uint32_t timeout = 5000;
		while ((!I2C_GetFlagStatus(I2Cx, I2C_FLAG_SB)) && timeout) {
			timeout--;
		}
		if (timeout == 0) {
			printf("Timeout generating START. Bus busy?\n");
			continue;
		}

		// Try to select slave
		I2C_Send7bitAddress(I2Cx, addr | 1, I2C_Direction_Receiver);

		/* Wait for either addressing or acknowledge failure */
		while (!(I2C_GetFlagStatus(I2Cx, I2C_FLAG_ADDR) || I2C_GetFlagStatus(I2Cx, I2C_FLAG_AF)));

		if (I2C_GetFlagStatus(I2Cx, I2C_FLAG_AF)) {
			/* ACK failure, STOP */
			I2C_GenerateSTOP(I2Cx, ENABLE);
			continue;
		}

		timeout = 5000;
		I2C_AcknowledgeConfig(I2Cx, DISABLE);
		while ((!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED)) && timeout) {
			timeout--;
		}
		if (timeout == 0) {
			printf("Timeout at receiving.\n");
			I2C_GenerateSTOP(I2Cx, ENABLE);
			continue;
		}
		I2C_GenerateSTOP(I2Cx, ENABLE);
		I2C_ReceiveData(I2Cx);
		printf("Found: 0x%x\n", addr);
	}
}
