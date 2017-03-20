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
		addr++;
		printf("Addressing %x\n", addr);


		 I2C_AcknowledgeConfig(I2C1, ENABLE);
		I2C_GenerateSTART(I2C1, ENABLE);

		/* Test on SB Flag */
		uint32_t timeout = 50000;
		while ((!I2C_GetFlagStatus(I2C1, I2C_FLAG_SB)) && timeout) {
			timeout--;
		}
		if (timeout == 0) {
			printf("Timeout SB\n");
			continue;
		}

		/* Send IOExpander address for read */
		I2C_Send7bitAddress(I2C1, addr, I2C_Direction_Receiver);

		timeout = 50000;
		while ((!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)) && timeout) {
			timeout--;
		}
		if (timeout == 0) {
//			printf("Timeout EV\n");
			I2C_GenerateSTOP(I2C1, ENABLE);
			continue;
		}
		I2C_GenerateSTOP(I2C1, ENABLE);

		printf("SUCCESS 0x%x\n", addr);
		I2C_AcknowledgeConfig(I2C1, DISABLE);
		uint8_t data = I2C_ReceiveData(I2C1);
		printf("READ %x\n", data);

	//	while (!I2C_GetFlagStatus(I2C1, I2C_FLAG_BTF)) { }


#if 0
  /* Test on SB Flag */
  timeout = TIMEOUT_MAX;
  while (I2C_GetFlagStatus(I2C1,I2C_FLAG_SB) == RESET)
  {
    if (timeout-- == 0) return(IOE_TimeoutUserCallback());
  }

  /* Transmit the slave address and enable writing operation */
  I2C_Send7bitAddress(I2C1, DeviceAddr, I2C_Direction_Transmitter);

  /* Test on ADDR Flag */
  timeout = TIMEOUT_MAX;
  while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
  {
    if (timeout-- == 0) return(IOE_TimeoutUserCallback());
  }

	//	I2C_Send7bitAddress(I2C1, 0xaa,

		I2C_SendData(I2C1, RegisterAddr);

#endif
		delay_millis(1000);
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


