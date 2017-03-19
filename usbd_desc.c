#include "usbd_core.h"
#include "usbd_desc.h"
#include "usbd_conf.h"
#include "usbd_def.h"

#define USBD_VID						0x0483
#define USBD_PID						0x572B

#define USBD_LANGID_STRING				0x409
#define USBD_MANUFACTURER_STRING		"Johannes Bauer"

#define USBD_PRODUCT_FS_STRING			"flightpanel v0.01"
#define USBD_SERIALNUMBER_FS_STRING		"316eef25237e"
#define USBD_CONFIGURATION_FS_STRING	"HID Config"
#define USBD_INTERFACE_FS_STRING		"HID Interface"

#define UINT16(x)						((x) >> 0) & 0xff, ((x) >> 8) & 0xff

static uint8_t hUSBDDeviceDesc[USB_LEN_DEV_DESC] = {
	0x12,									/* bLength */
	USB_DESC_TYPE_DEVICE,					/* bDescriptorType */
	UINT16(2),								/* bcdUSB */
	0x00,									/* bDeviceClass */
	0x00,									/* bDeviceSubClass */
	0x00,									/* bDeviceProtocol */
	USB_MAX_EP0_SIZE,						/* bMaxPacketSize0 */
	UINT16(USBD_VID), 						/* idVendor */
	UINT16(USBD_PID),						/* idProduct */
	UINT16(2),								/* bcdDevice */
	USBD_IDX_MFC_STR,						/* iManufacturer */
	USBD_IDX_PRODUCT_STR,					/* iProduct */
	USBD_IDX_SERIAL_STR,					/* iSerial */
	USBD_MAX_NUM_CONFIGURATION				/* bNumConfigurations */
};

uint8_t USBD_LangIDDesc[USB_LEN_LANGID_STR_DESC] = {
	USB_LEN_LANGID_STR_DESC,
	USB_DESC_TYPE_STRING,
	UINT16(USBD_LANGID_STRING)
};

static uint8_t usbd_desc_string_buffer[USBD_MAX_STR_DESC_SIZ];

static uint8_t* USBD_HID_DeviceDescriptor(USBD_SpeedTypeDef speed, uint16_t *length) {
	*length = sizeof(hUSBDDeviceDesc);
	return hUSBDDeviceDesc;
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

USBD_DescriptorsTypeDef HID_Desc = {
	.GetDeviceDescriptor = USBD_HID_DeviceDescriptor,
	.GetLangIDStrDescriptor = USBD_HID_LangIDStrDescriptor,
	.GetManufacturerStrDescriptor = USBD_HID_ManufacturerStrDescriptor,
	.GetProductStrDescriptor = USBD_HID_ProductStrDescriptor,
	.GetSerialStrDescriptor = USBD_HID_SerialStrDescriptor,
	.GetConfigurationStrDescriptor = USBD_HID_ConfigStrDescriptor,
	.GetInterfaceStrDescriptor = USBD_HID_InterfaceStrDescriptor,
};
