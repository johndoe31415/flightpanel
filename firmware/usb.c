/**
 *	flightpanel - A Cortex-M4 based USB flight panel for flight simulators.
 *	Copyright (C) 2017-2017 Johannes Bauer
 *
 *	This file is part of flightpanel.
 *
 *	flightpanel is free software; you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation; this program is ONLY licensed under
 *	version 3 of the License, later versions are explicitly excluded.
 *
 *	flightpanel is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with flightpanel; if not, write to the Free Software
 *	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *	Johannes Bauer <JohannesBauer@gmx.de>
**/

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
static struct hid_report_t send_report;

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
}

/* Make a copy of the incoming report (so the caller can allocate it on the
 * stack) */
void usb_submit_report(const struct hid_report_t *report) {
	memcpy(&send_report, report, sizeof(struct hid_report_t));
	send_report.report_id = 1;
	USBD_HID_SendReport(&USBD_Device, &send_report);
}
