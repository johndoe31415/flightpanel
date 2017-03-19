#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <usbd_def.h>
#include <usbd_core.h>
#include <usbd_hid.h>
#include <usbd_conf.h>
//#include <stm32f4xx_hal.h>

#include "usb.h"

typedef PCD_HandleTypeDef;
extern PCD_HandleTypeDef hpcd;
static USBD_HandleTypeDef USBD_Device;
extern USBD_DescriptorsTypeDef HID_Desc;

void OTG_FS_WKUP_IRQHandler(void) {
	printf("Unhandled: OTG FS Wakeup IRQ\n");
	while (true);
}

void OTG_FS_IRQHandler(void) {
	HAL_PCD_IRQHandler(&hpcd);
}

void init_usb_late(void) {
  /* Init Device Library */
  USBD_Init(&USBD_Device, &HID_Desc, 0);

  /* Add Supported Class */
  USBD_RegisterClass(&USBD_Device, USBD_HID_CLASS);

  /* Start Device Process */
  USBD_Start(&USBD_Device);

}
