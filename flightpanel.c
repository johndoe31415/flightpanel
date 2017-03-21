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
#include "eeprom.h"
#include "configuration.h"
#include "usbd_hid.h"
#include "usb.h"

int main(void) {
	printf("Reset successful.\n");
	init_usb_late();
	printf("USB initialized.\n");

#if 0
	struct rotary_encoder_t rotary = {
		.value = 0,
		.max_value = 1200,
		.wrap_around = true,
	};
#endif
//	volatile uint32_t i = 0;

//	struct configuration config;
//	read_configuration(&config);
//	eeprom_write(20, "Test", 4);
	struct hid_report_t hid_report = { };
	while (true) {
		hid_report.squawk++;
		usb_submit_report(&hid_report);
		//i2c_scanbus(I2C1);
//		eeprom_dump(10);
		delay_millis(100);
	}

	return 0;
}


