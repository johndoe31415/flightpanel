#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <usbd_def.h>
#include <usbd_core.h>
#include <usbd_conf.h>
#include "usbd_hid.h"

typedef struct anon USB_OTG_GlobalTypeDef;
#include "stm32f4xx_hal.h"
#include "usb.h"

extern PCD_HandleTypeDef hpcd;
static USBD_HandleTypeDef USBD_Device;

void OTG_FS_WKUP_IRQHandler(void) {
	printf("Unhandled: OTG FS Wakeup IRQ\n");
	while (true);
}

void OTG_FS_IRQHandler(void) {
	HAL_PCD_IRQHandler(&hpcd);
}

void init_usb_late(void) {
	/* Init Device Library */
	USBD_Init(&USBD_Device, &FlightPanelUSBDescriptors, 0);

	/* Add Supported Class */
	USBD_RegisterClass(&USBD_Device, &FlightPanelUSBHIDClass);

	/* Start Device Process */
	USBD_Start(&USBD_Device);

#if 0
	struct hid_report_t report = { 0 };
	while (false) {
		report.value++;
		if (report.value == 101) {
			report.value = 0;
		}

		USBD_HID_SendReport(&USBD_Device, &report);
		for (volatile int i = 0; i < 1000000; i++) { }
		printf("Report %d\n", report.value);
	}
#endif
}

