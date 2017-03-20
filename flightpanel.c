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

#define xAF			(1 << 10)
#define xADDR		(1 << 1)

int main(void) {
	printf("Reset successful.\n");
	init_usb_late();
	printf("USB initialized.\n");

	/*
	struct rotary_encoder_t rotary = {
		.value = 0,
		.max_value = 1200,
		.wrap_around = true,
	};
*/
//	volatile uint32_t i = 0;
	uint8_t addr = 0;
	while (true) {
		delay_millis(100);

		addr += 0x10;
		printf("Addressing 0x%x\n", addr);

		I2C1->SR1 &= ~xAF;
		I2C_AcknowledgeConfig(I2C1, ENABLE);
		I2C_GenerateSTART(I2C1, ENABLE);

		/* Test on SB Flag */
		uint32_t timeout = 50000;
		while ((!I2C_GetFlagStatus(I2C1, I2C_FLAG_SB)) && timeout) {
			timeout--;
		}
		if (timeout == 0) {
			printf("Timeout generating START.\n");
			continue;
		}

		/* Send IOExpander address for read */
		I2C_Send7bitAddress(I2C1, addr | 1, I2C_Direction_Receiver);

		/* Wait for address to be sent out */
		while (!(I2C1->SR1 & (xADDR | xAF)));

		if (I2C1->SR1 & xAF) {
			/* ACK fail */
			I2C_GenerateSTOP(I2C1, ENABLE);
			continue;
		}

		/* Then wait for address ACK */
		timeout = 50000;
		while ((!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)) && timeout) {
			timeout--;
		}
		if (timeout == 0) {
			I2C_GenerateSTOP(I2C1, ENABLE);
			continue;
		}


		while ((!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED)) && timeout) {
			timeout--;
		}
		if (timeout == 0) {
			printf("Timeout at receiving.\n");
			I2C_GenerateSTOP(I2C1, ENABLE);
			continue;
		}

//		while (!I2C_GetFlagStatus(I2C1, I2C_FLAG_BTF)) { }

		I2C_AcknowledgeConfig(I2C1, DISABLE);
		I2C_GenerateSTOP(I2C1, ENABLE);
		while( !I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED) );
		uint8_t data = I2C_ReceiveData(I2C1);
		printf("READ %x\n", data);



#if 0
		i++;
		if (i == 0x800) {
			GPIOD->ODR ^= GPIO_Pin_12;

			bool v1 = (GPIOD->IDR & GPIO_Pin_10) != 0;
			bool v2 = (GPIOD->IDR & GPIO_Pin_11) != 0;
			if (rotary_encoder_update(&rotary, v1, v2)) {
				printf("Value: %d\n", rotary.value);
			}
			i = 0;

			//spi_tx_data("muhkuh", 6);
			//while (DMA_GetFlagStatus(DMA2_Stream3, QSPI_DMA_FLAG_TC) == RESET);
//			spi_tx_data_dma("muhkuh", 6);

		}

#endif
	}

	return 0;
}


