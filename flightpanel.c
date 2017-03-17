#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include <stm32f4xx_gpio.h>
#if 0
#include <usbd_desc.h>
#include <usbd_core.h>
#include <usbd_hid.h>
#endif

#include "rs232.h"

int main(void) {
	int i = 0;
	printf("Reset successful.\n");
#if 0
	USBD_HandleTypeDef USBD_Device;

	/* Init Device Library */
	USBD_Init(&USBD_Device, &HID_Desc, 0);

	/* Add Supported Class */
	USBD_RegisterClass(&USBD_Device, USBD_HID_CLASS);

	/* Start Device Process */
	USBD_Start(&USBD_Device);
#endif

	while (1) {
		i++;
		if (i & 0x10000) {
			GPIOD->ODR ^= GPIO_Pin_12;
		}
		if (i & 0x20000) {
			GPIOD->ODR ^= GPIO_Pin_13;
		}
		if (i & 0x40000) {
			GPIOD->ODR ^= GPIO_Pin_14;
		}
		if (i & 0x80000) {
			GPIOD->ODR ^= GPIO_Pin_15;
			printf("toggle %d\n", i);
		}
	}
	return 0;
}


