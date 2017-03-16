#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include <stm32f4xx_gpio.h>

int main(void) {
	int i = 0;
	while (1) {
		i++;
		if (i & 0x100000) {
			GPIOD->ODR ^= GPIO_Pin_12;
		}
		if (i & 0x200000) {
			GPIOD->ODR ^= GPIO_Pin_13;
		}
		if (i & 0x400000) {
			GPIOD->ODR ^= GPIO_Pin_14;
		}
		if (i & 0x800000) {
			GPIOD->ODR ^= GPIO_Pin_15;
		}
	}
	return 0;
}


