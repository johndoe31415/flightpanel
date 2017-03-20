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


	while (true) {
		i2c_scanbus();
		delay_millis(1000);
	}
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

	return 0;
}


