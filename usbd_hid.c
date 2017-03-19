#include "usb.h"
#include "usbd_conf.h"
#include "usbd_core.h"
#include "usbd_ctlreq.h"
#include "usbd_def.h"
#include "usbd_hid.h"

#define USBD_VID											0x0483
#define USBD_PID											0x572B

#define USBD_LANGID_STRING									0x409
#define USBD_MANUFACTURER_STRING							"Johannes Bauer"

#define USBD_PRODUCT_FS_STRING								"flightpanel v0.01"
#define USBD_SERIALNUMBER_FS_STRING							"316eef25237e"
#define USBD_CONFIGURATION_FS_STRING						"HID Config"
#define USBD_INTERFACE_FS_STRING							"HID Interface"

#define MAX_POWER_MILLIAMPS									100
#define POLLING_INTERVAL_MILLISECONDS						20
#define ENDPOINT_COUNT										1

static uint8_t USBDeviceDescriptor[] = {
	18,																			/* bLength */
	DESCRIPTOR_TYPE_DEVICE,														/* bDescriptorType */
	UINT16(2),																	/* bcdUSB */
	0x00,																		/* bDeviceClass */
	0x00,																		/* bDeviceSubClass */
	0x00,																		/* bDeviceProtocol */
	USB_MAX_EP0_SIZE,															/* bMaxPacketSize0 */
	UINT16(USBD_VID), 															/* idVendor */
	UINT16(USBD_PID),															/* idProduct */
	UINT16(2),																	/* bcdDevice */
	USBD_IDX_MFC_STR,															/* iManufacturer */
	USBD_IDX_PRODUCT_STR,														/* iProduct */
	USBD_IDX_SERIAL_STR,														/* iSerial */
	USBD_MAX_NUM_CONFIGURATION													/* bNumConfigurations */
};

uint8_t USBD_LangIDDesc[4] = {
	4,																			/* bLength */
	DESCRIPTOR_TYPE_STRING,														/* bDescriptorType */
	UINT16(USBD_LANGID_STRING)
};

static uint8_t HIDReportDescriptor[] = {
	0x05, 0x01, 0x09, 0x04, 0xa1, 0x01, 0x05, 0x02, 0x09, 0xbb, 0x15, 0x00, 0x25, 0x64, 0x75, 0x08, 0x95, 0x01, 0x55, 0x00, 0x65, 0x00, 0x81, 0x02, 0xc0
};
#define HID_REPORT_DESCRIPTOR_SIZE_BYTES					sizeof(HIDReportDescriptor)

#define USB_HID_CONFIGURATION_DESCRIPTOR_SIZE_BYTES			34
#define USB_HID_DESCRIPTOR_OFFSET							18
static uint8_t USBD_HID_CfgDesc[USB_HID_CONFIGURATION_DESCRIPTOR_SIZE_BYTES] = {
	// Configuration Descriptor
	9, 																			/* bLength */
	DESCRIPTOR_TYPE_CONFIGURATION, 												/* bDescriptorType */
	UINT16(USB_HID_CONFIGURATION_DESCRIPTOR_SIZE_BYTES),						/* wTotalLength */
	1,																			/* bNumInterfaces */
	1,																			/* bConfigurationValue */
	0,																			/* iConfiguration */
	CONF_BMATTR_BASE | CONF_BMATTR_SELF_POWERED | CONF_BMATTR_REMOTE_WKUP,		/* bmAttributes */
	MAX_POWER_MILLIAMPS / 2,													/* MaxPower */

	// Interface Descriptor
	9,																			/* bLength */
	DESCRIPTOR_TYPE_INTERFACE,													/* bDescriptorType */
	0,																			/* bInterfaceNumber */
	0,																			/* bAlternateSetting */
	ENDPOINT_COUNT,																/* bNumEndpoints */
	BINTERFACE_CLASS_HID,														/* bInterfaceClass */
	BINTERFACE_SUBCLASS_NONBOOT,												/* bInterfaceSubClass */
	NINTERFACE_PROTOCOL_NONE,													/* nInterfaceProtocol */
	0,																			/* iInterface */

	// HID Descriptor
	9,																			/* bLength */
	DESCRIPTOR_TYPE_HID,														/* bDescriptorType */
	UINT16(0x111),																/* bcdHID */
	BCOUNTRY_CODE_UNSUPPORTED,													/* bCountryCode */
	1,																			/* bNumDescriptors*/
	DESCRIPTOR_TYPE_HID_REPORT,													/* bDescriptorType*/
	UINT16(HID_REPORT_DESCRIPTOR_SIZE_BYTES),									/* wItemLength */

	// Endpoint Descriptor
	7,																			/* bLength */
	DESCRIPTOR_TYPE_ENDPOINT,													/* bDescriptorType */
	HID_EPIN_ADDR,																/* bEndpointAddress */
	EP_BMATTR_TT_INTERRUPT,														/* bmAttributes */
	UINT16(HID_EPIN_SIZE),														/* wMaxPacketSize */
	POLLING_INTERVAL_MILLISECONDS,												/* bInterval */
};

// USB Standard Descriptor
static uint8_t USBD_HID_DeviceQualifierDesc[]  = {
	10,																			/* bLength */
	DESCRIPTOR_TYPE_DEVICE_QUALIFIER,											/* bDescriptorType */
	UINT16(2),																	/* bcdUSB */
	0x00,																		/* bDeviceClass */
	0x00,																		/* bDeviceSubClass */
	0x00,																		/* bDeviceProtocol */
	USB_MAX_EP0_SIZE,															/* bMaxPacketSize0 */
	1,																			/* bNumConfigurations */
	0																			/* bReserved */
};

static uint8_t usbd_desc_string_buffer[USBD_MAX_STR_DESC_SIZ];

static uint8_t USBD_HID_Init(USBD_HandleTypeDef *pdev, uint8_t cfgidx) {
	uint8_t ret = 0;
	USBD_LL_OpenEP(pdev, HID_EPIN_ADDR, USBD_EP_TYPE_INTR, HID_EPIN_SIZE);
	pdev->pClassData = USBD_malloc(sizeof(USBD_HID_HandleTypeDef));
	if (pdev->pClassData) {
		((USBD_HID_HandleTypeDef *) pdev->pClassData)->state = HID_IDLE;
	} else {
		ret = 1;
	}
	return ret;
}

static uint8_t USBD_HID_DeInit(USBD_HandleTypeDef *pdev, uint8_t cfgidx) {
	USBD_LL_CloseEP(pdev, HID_EPIN_ADDR);
	if (pdev->pClassData != NULL) {
		USBD_free(pdev->pClassData);
		pdev->pClassData = NULL;
	}
	return USBD_OK;
}

static uint8_t USBD_HID_Setup(USBD_HandleTypeDef *pdev, USBD_SetupReqTypedef *req) {
	uint16_t len = 0;
	uint8_t *pbuf = NULL;
	USBD_HID_HandleTypeDef *hhid = (USBD_HID_HandleTypeDef *) pdev->pClassData;

	switch (req->bmRequest & USB_REQ_TYPE_MASK) {
		case USB_REQ_TYPE_CLASS:
			switch (req->bRequest) {
				case HID_REQ_SET_PROTOCOL:
					hhid->Protocol = (uint8_t) (req->wValue);
					break;

				case HID_REQ_GET_PROTOCOL:
					USBD_CtlSendData(pdev, (uint8_t *) & hhid->Protocol, 1);
					break;

				case HID_REQ_SET_IDLE:
					hhid->IdleState = (uint8_t) (req->wValue >> 8);
					break;

				case HID_REQ_GET_IDLE:
					USBD_CtlSendData(pdev, (uint8_t *) & hhid->IdleState, 1);
					break;

				default:
					USBD_CtlError(pdev, req);
					return USBD_FAIL;
			}
			break;

		case USB_REQ_TYPE_STANDARD:
			switch (req->bRequest) {
				case USB_REQ_GET_DESCRIPTOR:
					if (req->wValue >> 8 == DESCRIPTOR_TYPE_HID_REPORT) {
						len = MIN(HID_REPORT_DESCRIPTOR_SIZE_BYTES, req->wLength);
						pbuf = HIDReportDescriptor;
					} else if (req->wValue >> 8 == DESCRIPTOR_TYPE_HID) {
						uint8_t hid_descriptor_length = USBD_HID_CfgDesc[USB_HID_DESCRIPTOR_OFFSET];
						pbuf = USBD_HID_CfgDesc + USB_HID_DESCRIPTOR_OFFSET;
						len = MIN(hid_descriptor_length, req->wLength);
					}

					USBD_CtlSendData(pdev, pbuf, len);

					break;

				case USB_REQ_GET_INTERFACE:
					USBD_CtlSendData(pdev, (uint8_t *) & hhid->AltSetting, 1);
					break;

				case USB_REQ_SET_INTERFACE:
					hhid->AltSetting = (uint8_t) (req->wValue);
					break;
			}
	}
	return USBD_OK;
}

uint8_t USBD_HID_SendReport(USBD_HandleTypeDef *pdev, const struct hid_report_t *report) {
	USBD_HID_HandleTypeDef *hhid = (USBD_HID_HandleTypeDef *) pdev->pClassData;

	if ((pdev->dev_state == USBD_STATE_CONFIGURED) && (hhid->state == HID_IDLE)) {
		hhid->state = HID_BUSY;
		USBD_LL_Transmit(pdev, HID_EPIN_ADDR, (uint8_t*)report, sizeof(struct hid_report_t));
	}
	return USBD_OK;
}

static uint8_t* USBD_HID_GetCfgDesc(uint16_t *length) {
	*length = sizeof(USBD_HID_CfgDesc);
	return USBD_HID_CfgDesc;
}

static uint8_t USBD_HID_DataIn(USBD_HandleTypeDef *pdev, uint8_t epnum) {
  /* Ensure that the FIFO is empty before a new transfer, this condition could
     be caused by  a new transfer before the end of the previous transfer */
  ((USBD_HID_HandleTypeDef *) pdev->pClassData)->state = HID_IDLE;
  return USBD_OK;
}

static uint8_t* USBD_HID_GetDeviceQualifierDesc(uint16_t *length) {
  *length = sizeof(USBD_HID_DeviceQualifierDesc);
  return USBD_HID_DeviceQualifierDesc;
}

static uint8_t* USBD_HID_DeviceDescriptor(USBD_SpeedTypeDef speed, uint16_t *length) {
	*length = sizeof(USBDeviceDescriptor);
	return USBDeviceDescriptor;
}

static uint8_t* USBD_HID_LangIDStrDescriptor(USBD_SpeedTypeDef speed, uint16_t *length) {
	*length = sizeof(USBD_LangIDDesc);
	return USBD_LangIDDesc;
}

static uint8_t* USBD_HID_ProductStrDescriptor(USBD_SpeedTypeDef speed, uint16_t *length) {
	USBD_GetString((uint8_t *)USBD_PRODUCT_FS_STRING, usbd_desc_string_buffer, length);
	return usbd_desc_string_buffer;
}

static uint8_t* USBD_HID_ManufacturerStrDescriptor(USBD_SpeedTypeDef speed, uint16_t *length) {
	USBD_GetString((uint8_t *)USBD_MANUFACTURER_STRING, usbd_desc_string_buffer, length);
	return usbd_desc_string_buffer;
}

static uint8_t* USBD_HID_SerialStrDescriptor(USBD_SpeedTypeDef speed, uint16_t *length) {
	USBD_GetString((uint8_t *)USBD_SERIALNUMBER_FS_STRING, usbd_desc_string_buffer, length);
	return usbd_desc_string_buffer;
}

static uint8_t* USBD_HID_ConfigStrDescriptor(USBD_SpeedTypeDef speed, uint16_t *length) {
	USBD_GetString((uint8_t *)USBD_CONFIGURATION_FS_STRING, usbd_desc_string_buffer, length);
	return usbd_desc_string_buffer;
}

static uint8_t* USBD_HID_InterfaceStrDescriptor(USBD_SpeedTypeDef speed, uint16_t *length) {
	USBD_GetString((uint8_t *)USBD_INTERFACE_FS_STRING, usbd_desc_string_buffer, length);
	return usbd_desc_string_buffer;
}

/* Export device calls */
USBD_DescriptorsTypeDef FlightPanelUSBDescriptors = {
	.GetDeviceDescriptor = USBD_HID_DeviceDescriptor,
	.GetLangIDStrDescriptor = USBD_HID_LangIDStrDescriptor,
	.GetManufacturerStrDescriptor = USBD_HID_ManufacturerStrDescriptor,
	.GetProductStrDescriptor = USBD_HID_ProductStrDescriptor,
	.GetSerialStrDescriptor = USBD_HID_SerialStrDescriptor,
	.GetConfigurationStrDescriptor = USBD_HID_ConfigStrDescriptor,
	.GetInterfaceStrDescriptor = USBD_HID_InterfaceStrDescriptor,
};

/* Export HID class calls */
USBD_ClassTypeDef FlightPanelUSBHIDClass = {
	.Init = USBD_HID_Init,
	.DeInit = USBD_HID_DeInit,
	.Setup = USBD_HID_Setup,
	.DataIn = USBD_HID_DataIn,
	.GetFSConfigDescriptor = USBD_HID_GetCfgDesc,
	.GetOtherSpeedConfigDescriptor = USBD_HID_GetCfgDesc,
	.GetDeviceQualifierDescriptor = USBD_HID_GetDeviceQualifierDesc,
};

