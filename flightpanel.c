#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include <stm32f4xx_gpio.h>
#include <stm32f4xx_tim.h>

#include "rs232.h"
#include "rotary.h"

int main(void) {
	printf("Reset successful.\n");

	struct rotary_encoder_t rotary = {
		.value = 0,
		.max_value = 1200,
		.wrap_around = true,
	};

	volatile uint32_t i = 0;
	while (1) {
		i++;
		if (i == 0x800) {
			GPIOD->ODR ^= GPIO_Pin_12;

			bool v1 = (GPIOD->IDR & GPIO_Pin_10) != 0;
			bool v2 = (GPIOD->IDR & GPIO_Pin_11) != 0;
			if (rotary_encoder_update(&rotary, v1, v2)) {
				printf("Value: %d\n", rotary.value);
			}
			i = 0;
		}
	}

	return 0;
}


