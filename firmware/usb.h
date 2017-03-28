#ifndef __USB_H__
#define __USB_H__

#include "usbd_hid.h"

/*************** AUTO GENERATED SECTION FOLLOWS ***************/
void OTG_FS_WKUP_IRQHandler(void);
void OTG_FS_IRQHandler(void);
void init_usb_late(void);
void usb_submit_report(const struct hid_report_t *report);
/***************  AUTO GENERATED SECTION ENDS   ***************/

#endif
