#ifndef __USBD_HID_H__
#define __USBD_HID_H__

#include "usbd_def.h"

struct hid_report_t {
	uint16_t vhf1, vhf1_backup;
	uint16_t vhf2, vhf2_backup;
	uint16_t backup1, backup1_backup;
	uint16_t backup2, backup2_backup;
	uint16_t squawk;
	uint16_t buttons;
} __attribute__ ((packed));

#define UINT16(x)											((x) >> 0) & 0xff, ((x) >> 8) & 0xff

#define HID_EPIN_ADDR										0x81
#define HID_EPIN_SIZE										sizeof(struct hid_report_t)

#define HID_EPOUT_ADDR										0x01
#define HID_EPOUT_SIZE										0x10

#define HID_REQ_SET_PROTOCOL								0x0b
#define HID_REQ_GET_PROTOCOL								0x03
#define HID_REQ_SET_IDLE									0x0a
#define HID_REQ_GET_IDLE									0x02
#define HID_REQ_SET_REPORT									0x09
#define HID_REQ_GET_REPORT									0x01

#define DESCRIPTOR_TYPE_DEVICE								1
#define DESCRIPTOR_TYPE_CONFIGURATION						2
#define DESCRIPTOR_TYPE_STRING								3
#define DESCRIPTOR_TYPE_INTERFACE							4
#define DESCRIPTOR_TYPE_ENDPOINT							5
#define DESCRIPTOR_TYPE_DEVICE_QUALIFIER					6
#define DESCRIPTOR_TYPE_HID									33
#define DESCRIPTOR_TYPE_HID_REPORT							34

#define CONF_BMATTR_BASE									(1 << 7)
#define CONF_BMATTR_SELF_POWERED							(1 << 6)
#define CONF_BMATTR_REMOTE_WKUP								(1 << 5)

#define EP_BMATTR_TT_CONTROL								(0 << 0)
#define EP_BMATTR_TT_ISOCHRONOUS							(1 << 0)
#define EP_BMATTR_TT_BULK									(2 << 0)
#define EP_BMATTR_TT_INTERRUPT								(3 << 0)
#define EP_BMATTR_ISOSYNC_NOSYNC							(0 << 2)
#define EP_BMATTR_ISOSYNC_ASYNC								(1 << 2)
#define EP_BMATTR_ISOSYNC_ADAPT								(2 << 2)
#define EP_BMATTR_ISOSYNC_SYNC								(3 << 2)
#define EP_BMATTR_ISOUSAGE_DATA								(0 << 4)
#define EP_BMATTR_ISOUSAGE_FEEDBACK							(1 << 4)
#define EP_BMATTR_ISOUSAGE_EXPLICITFDBK						(2 << 4)

#define BINTERFACE_CLASS_HID								3
#define BINTERFACE_SUBCLASS_NONBOOT							0
#define NINTERFACE_PROTOCOL_NONE							0

#define BCOUNTRY_CODE_UNSUPPORTED							0

typedef enum {
	HID_IDLE = 0,
	HID_BUSY,
} HID_StateTypeDef;

typedef struct {
	uint32_t Protocol;
	uint32_t IdleState;
	uint32_t AltSetting;
	HID_StateTypeDef state;
	uint8_t out_buffer[HID_EPOUT_SIZE];
} USBD_HID_HandleTypeDef;

extern USBD_DescriptorsTypeDef FlightPanelUSBDescriptors;
extern USBD_ClassTypeDef FlightPanelUSBHIDClass;

/*************** AUTO GENERATED SECTION FOLLOWS ***************/
uint8_t USBD_HID_SendReport(USBD_HandleTypeDef *pdev, const struct hid_report_t *report);
/***************  AUTO GENERATED SECTION ENDS   ***************/

#endif
