#ifndef __PINMAP_H__
#define __PINMAP_H__

#include <stm32f4xx_gpio.h>
#include "timer.h"

struct gpio_definition_t {
	const char *pin_name;
	const char *name;
	GPIO_TypeDef *gpio;
	int pin;
	const char *comment;
};

// PA0: UserButton (IN_DN)
#define UserButton_GPIO							GPIOA
#define UserButton_PinSource					GPIO_PinSource0
#define UserButton_Pin							GPIO_Pin_0
#define UserButton_SetHIGH()					UserButton_GPIO->BSRRL = UserButton_Pin
#define UserButton_SetLOW()						UserButton_GPIO->BSRRH = UserButton_Pin
#define UserButton_Toggle()						UserButton_GPIO->ODR ^= UserButton_Pin
#define UserButton_Pulse()						do { UserButton_SetHIGH(); delay_loopcnt(LOOPCOUNT_50NS); UserButton_SetLOW(); } while (0)
#define UserButton_InvPulse()					do { UserButton_SetLOW(); delay_loopcnt(LOOPCOUNT_50NS); UserButton_SetHIGH(); } while (0)
#define UserButton_SetTo(value)					if (value) { UserButton_SetHIGH(); } else { UserButton_SetLOW(); }
#define UserButton_GetValue()					(UserButton_GPIO->IDR & UserButton_Pin)
#define UserButton_GPIO_Definition				{ .name = "UserButton", .pin_name = "A0", .gpio = UserButton_GPIO, .pin = UserButton_Pin }

// PA4: Audio_WS (?)
#define Audio_WS_GPIO							GPIOA
#define Audio_WS_PinSource						GPIO_PinSource4
#define Audio_WS_Pin							GPIO_Pin_4
#define Audio_WS_SetHIGH()						Audio_WS_GPIO->BSRRL = Audio_WS_Pin
#define Audio_WS_SetLOW()						Audio_WS_GPIO->BSRRH = Audio_WS_Pin
#define Audio_WS_Toggle()						Audio_WS_GPIO->ODR ^= Audio_WS_Pin
#define Audio_WS_Pulse()						do { Audio_WS_SetHIGH(); delay_loopcnt(LOOPCOUNT_50NS); Audio_WS_SetLOW(); } while (0)
#define Audio_WS_InvPulse()						do { Audio_WS_SetLOW(); delay_loopcnt(LOOPCOUNT_50NS); Audio_WS_SetHIGH(); } while (0)
#define Audio_WS_SetTo(value)					if (value) { Audio_WS_SetHIGH(); } else { Audio_WS_SetLOW(); }
#define Audio_WS_GetValue()						(Audio_WS_GPIO->IDR & Audio_WS_Pin)
#define Audio_WS_GPIO_Definition				{ .name = "Audio_WS", .pin_name = "A4", .gpio = Audio_WS_GPIO, .pin = Audio_WS_Pin }

// PA5: MEMS_SCK (OUT)
#define MEMS_SCK_GPIO							GPIOA
#define MEMS_SCK_PinSource						GPIO_PinSource5
#define MEMS_SCK_Pin							GPIO_Pin_5
#define MEMS_SCK_SetHIGH()						MEMS_SCK_GPIO->BSRRL = MEMS_SCK_Pin
#define MEMS_SCK_SetLOW()						MEMS_SCK_GPIO->BSRRH = MEMS_SCK_Pin
#define MEMS_SCK_Toggle()						MEMS_SCK_GPIO->ODR ^= MEMS_SCK_Pin
#define MEMS_SCK_Pulse()						do { MEMS_SCK_SetHIGH(); delay_loopcnt(LOOPCOUNT_50NS); MEMS_SCK_SetLOW(); } while (0)
#define MEMS_SCK_InvPulse()						do { MEMS_SCK_SetLOW(); delay_loopcnt(LOOPCOUNT_50NS); MEMS_SCK_SetHIGH(); } while (0)
#define MEMS_SCK_SetTo(value)					if (value) { MEMS_SCK_SetHIGH(); } else { MEMS_SCK_SetLOW(); }
#define MEMS_SCK_GetValue()						(MEMS_SCK_GPIO->IDR & MEMS_SCK_Pin)
#define MEMS_SCK_GPIO_Definition				{ .name = "MEMS_SCK", .pin_name = "A5", .gpio = MEMS_SCK_GPIO, .pin = MEMS_SCK_Pin }

// PA6: MEMS_MISO (IN)
#define MEMS_MISO_GPIO							GPIOA
#define MEMS_MISO_PinSource						GPIO_PinSource6
#define MEMS_MISO_Pin							GPIO_Pin_6
#define MEMS_MISO_SetHIGH()						MEMS_MISO_GPIO->BSRRL = MEMS_MISO_Pin
#define MEMS_MISO_SetLOW()						MEMS_MISO_GPIO->BSRRH = MEMS_MISO_Pin
#define MEMS_MISO_Toggle()						MEMS_MISO_GPIO->ODR ^= MEMS_MISO_Pin
#define MEMS_MISO_Pulse()						do { MEMS_MISO_SetHIGH(); delay_loopcnt(LOOPCOUNT_50NS); MEMS_MISO_SetLOW(); } while (0)
#define MEMS_MISO_InvPulse()					do { MEMS_MISO_SetLOW(); delay_loopcnt(LOOPCOUNT_50NS); MEMS_MISO_SetHIGH(); } while (0)
#define MEMS_MISO_SetTo(value)					if (value) { MEMS_MISO_SetHIGH(); } else { MEMS_MISO_SetLOW(); }
#define MEMS_MISO_GetValue()					(MEMS_MISO_GPIO->IDR & MEMS_MISO_Pin)
#define MEMS_MISO_GPIO_Definition				{ .name = "MEMS_MISO", .pin_name = "A6", .gpio = MEMS_MISO_GPIO, .pin = MEMS_MISO_Pin }

// PA7: MEMS_MOSI (OUT)
#define MEMS_MOSI_GPIO							GPIOA
#define MEMS_MOSI_PinSource						GPIO_PinSource7
#define MEMS_MOSI_Pin							GPIO_Pin_7
#define MEMS_MOSI_SetHIGH()						MEMS_MOSI_GPIO->BSRRL = MEMS_MOSI_Pin
#define MEMS_MOSI_SetLOW()						MEMS_MOSI_GPIO->BSRRH = MEMS_MOSI_Pin
#define MEMS_MOSI_Toggle()						MEMS_MOSI_GPIO->ODR ^= MEMS_MOSI_Pin
#define MEMS_MOSI_Pulse()						do { MEMS_MOSI_SetHIGH(); delay_loopcnt(LOOPCOUNT_50NS); MEMS_MOSI_SetLOW(); } while (0)
#define MEMS_MOSI_InvPulse()					do { MEMS_MOSI_SetLOW(); delay_loopcnt(LOOPCOUNT_50NS); MEMS_MOSI_SetHIGH(); } while (0)
#define MEMS_MOSI_SetTo(value)					if (value) { MEMS_MOSI_SetHIGH(); } else { MEMS_MOSI_SetLOW(); }
#define MEMS_MOSI_GetValue()					(MEMS_MOSI_GPIO->IDR & MEMS_MOSI_Pin)
#define MEMS_MOSI_GPIO_Definition				{ .name = "MEMS_MOSI", .pin_name = "A7", .gpio = MEMS_MOSI_GPIO, .pin = MEMS_MOSI_Pin }

// PA9: USB_OTG_VBUS (IN)
#define USB_OTG_VBUS_GPIO						GPIOA
#define USB_OTG_VBUS_PinSource					GPIO_PinSource9
#define USB_OTG_VBUS_Pin						GPIO_Pin_9
#define USB_OTG_VBUS_SetHIGH()					USB_OTG_VBUS_GPIO->BSRRL = USB_OTG_VBUS_Pin
#define USB_OTG_VBUS_SetLOW()					USB_OTG_VBUS_GPIO->BSRRH = USB_OTG_VBUS_Pin
#define USB_OTG_VBUS_Toggle()					USB_OTG_VBUS_GPIO->ODR ^= USB_OTG_VBUS_Pin
#define USB_OTG_VBUS_Pulse()					do { USB_OTG_VBUS_SetHIGH(); delay_loopcnt(LOOPCOUNT_50NS); USB_OTG_VBUS_SetLOW(); } while (0)
#define USB_OTG_VBUS_InvPulse()					do { USB_OTG_VBUS_SetLOW(); delay_loopcnt(LOOPCOUNT_50NS); USB_OTG_VBUS_SetHIGH(); } while (0)
#define USB_OTG_VBUS_SetTo(value)				if (value) { USB_OTG_VBUS_SetHIGH(); } else { USB_OTG_VBUS_SetLOW(); }
#define USB_OTG_VBUS_GetValue()					(USB_OTG_VBUS_GPIO->IDR & USB_OTG_VBUS_Pin)
#define USB_OTG_VBUS_GPIO_Definition			{ .name = "USB_OTG_VBUS", .pin_name = "A9", .gpio = USB_OTG_VBUS_GPIO, .pin = USB_OTG_VBUS_Pin }

// PA10: USB_OTG_ID (IN)
#define USB_OTG_ID_GPIO							GPIOA
#define USB_OTG_ID_PinSource					GPIO_PinSource10
#define USB_OTG_ID_Pin							GPIO_Pin_10
#define USB_OTG_ID_SetHIGH()					USB_OTG_ID_GPIO->BSRRL = USB_OTG_ID_Pin
#define USB_OTG_ID_SetLOW()						USB_OTG_ID_GPIO->BSRRH = USB_OTG_ID_Pin
#define USB_OTG_ID_Toggle()						USB_OTG_ID_GPIO->ODR ^= USB_OTG_ID_Pin
#define USB_OTG_ID_Pulse()						do { USB_OTG_ID_SetHIGH(); delay_loopcnt(LOOPCOUNT_50NS); USB_OTG_ID_SetLOW(); } while (0)
#define USB_OTG_ID_InvPulse()					do { USB_OTG_ID_SetLOW(); delay_loopcnt(LOOPCOUNT_50NS); USB_OTG_ID_SetHIGH(); } while (0)
#define USB_OTG_ID_SetTo(value)					if (value) { USB_OTG_ID_SetHIGH(); } else { USB_OTG_ID_SetLOW(); }
#define USB_OTG_ID_GetValue()					(USB_OTG_ID_GPIO->IDR & USB_OTG_ID_Pin)
#define USB_OTG_ID_GPIO_Definition				{ .name = "USB_OTG_ID", .pin_name = "A10", .gpio = USB_OTG_ID_GPIO, .pin = USB_OTG_ID_Pin }

// PA11: USB_OTG_M (IO)
#define USB_OTG_M_GPIO							GPIOA
#define USB_OTG_M_PinSource						GPIO_PinSource11
#define USB_OTG_M_Pin							GPIO_Pin_11
#define USB_OTG_M_SetHIGH()						USB_OTG_M_GPIO->BSRRL = USB_OTG_M_Pin
#define USB_OTG_M_SetLOW()						USB_OTG_M_GPIO->BSRRH = USB_OTG_M_Pin
#define USB_OTG_M_Toggle()						USB_OTG_M_GPIO->ODR ^= USB_OTG_M_Pin
#define USB_OTG_M_Pulse()						do { USB_OTG_M_SetHIGH(); delay_loopcnt(LOOPCOUNT_50NS); USB_OTG_M_SetLOW(); } while (0)
#define USB_OTG_M_InvPulse()					do { USB_OTG_M_SetLOW(); delay_loopcnt(LOOPCOUNT_50NS); USB_OTG_M_SetHIGH(); } while (0)
#define USB_OTG_M_SetTo(value)					if (value) { USB_OTG_M_SetHIGH(); } else { USB_OTG_M_SetLOW(); }
#define USB_OTG_M_GetValue()					(USB_OTG_M_GPIO->IDR & USB_OTG_M_Pin)
#define USB_OTG_M_GPIO_Definition				{ .name = "USB_OTG_M", .pin_name = "A11", .gpio = USB_OTG_M_GPIO, .pin = USB_OTG_M_Pin }

// PA12: USB_OTG_P (IO)
#define USB_OTG_P_GPIO							GPIOA
#define USB_OTG_P_PinSource						GPIO_PinSource12
#define USB_OTG_P_Pin							GPIO_Pin_12
#define USB_OTG_P_SetHIGH()						USB_OTG_P_GPIO->BSRRL = USB_OTG_P_Pin
#define USB_OTG_P_SetLOW()						USB_OTG_P_GPIO->BSRRH = USB_OTG_P_Pin
#define USB_OTG_P_Toggle()						USB_OTG_P_GPIO->ODR ^= USB_OTG_P_Pin
#define USB_OTG_P_Pulse()						do { USB_OTG_P_SetHIGH(); delay_loopcnt(LOOPCOUNT_50NS); USB_OTG_P_SetLOW(); } while (0)
#define USB_OTG_P_InvPulse()					do { USB_OTG_P_SetLOW(); delay_loopcnt(LOOPCOUNT_50NS); USB_OTG_P_SetHIGH(); } while (0)
#define USB_OTG_P_SetTo(value)					if (value) { USB_OTG_P_SetHIGH(); } else { USB_OTG_P_SetLOW(); }
#define USB_OTG_P_GetValue()					(USB_OTG_P_GPIO->IDR & USB_OTG_P_Pin)
#define USB_OTG_P_GPIO_Definition				{ .name = "USB_OTG_P", .pin_name = "A12", .gpio = USB_OTG_P_GPIO, .pin = USB_OTG_P_Pin }

// PB0: Display1_nSS (OUT)
#define Display1_nSS_GPIO						GPIOB
#define Display1_nSS_PinSource					GPIO_PinSource0
#define Display1_nSS_Pin						GPIO_Pin_0
#define Display1_nSS_SetHIGH()					Display1_nSS_GPIO->BSRRL = Display1_nSS_Pin
#define Display1_nSS_SetLOW()					Display1_nSS_GPIO->BSRRH = Display1_nSS_Pin
#define Display1_nSS_Toggle()					Display1_nSS_GPIO->ODR ^= Display1_nSS_Pin
#define Display1_nSS_Pulse()					do { Display1_nSS_SetHIGH(); delay_loopcnt(LOOPCOUNT_50NS); Display1_nSS_SetLOW(); } while (0)
#define Display1_nSS_InvPulse()					do { Display1_nSS_SetLOW(); delay_loopcnt(LOOPCOUNT_50NS); Display1_nSS_SetHIGH(); } while (0)
#define Display1_nSS_SetTo(value)				if (value) { Display1_nSS_SetHIGH(); } else { Display1_nSS_SetLOW(); }
#define Display1_nSS_GetValue()					(Display1_nSS_GPIO->IDR & Display1_nSS_Pin)
#define Display1_nSS_GPIO_Definition			{ .name = "Display1_nSS", .pin_name = "B0", .gpio = Display1_nSS_GPIO, .pin = Display1_nSS_Pin }

// PB1: Display2_nSS (OUT)
#define Display2_nSS_GPIO						GPIOB
#define Display2_nSS_PinSource					GPIO_PinSource1
#define Display2_nSS_Pin						GPIO_Pin_1
#define Display2_nSS_SetHIGH()					Display2_nSS_GPIO->BSRRL = Display2_nSS_Pin
#define Display2_nSS_SetLOW()					Display2_nSS_GPIO->BSRRH = Display2_nSS_Pin
#define Display2_nSS_Toggle()					Display2_nSS_GPIO->ODR ^= Display2_nSS_Pin
#define Display2_nSS_Pulse()					do { Display2_nSS_SetHIGH(); delay_loopcnt(LOOPCOUNT_50NS); Display2_nSS_SetLOW(); } while (0)
#define Display2_nSS_InvPulse()					do { Display2_nSS_SetLOW(); delay_loopcnt(LOOPCOUNT_50NS); Display2_nSS_SetHIGH(); } while (0)
#define Display2_nSS_SetTo(value)				if (value) { Display2_nSS_SetHIGH(); } else { Display2_nSS_SetLOW(); }
#define Display2_nSS_GetValue()					(Display2_nSS_GPIO->IDR & Display2_nSS_Pin)
#define Display2_nSS_GPIO_Definition			{ .name = "Display2_nSS", .pin_name = "B1", .gpio = Display2_nSS_GPIO, .pin = Display2_nSS_Pin }

// PB2: Display3_nSS (OUT)
#define Display3_nSS_GPIO						GPIOB
#define Display3_nSS_PinSource					GPIO_PinSource2
#define Display3_nSS_Pin						GPIO_Pin_2
#define Display3_nSS_SetHIGH()					Display3_nSS_GPIO->BSRRL = Display3_nSS_Pin
#define Display3_nSS_SetLOW()					Display3_nSS_GPIO->BSRRH = Display3_nSS_Pin
#define Display3_nSS_Toggle()					Display3_nSS_GPIO->ODR ^= Display3_nSS_Pin
#define Display3_nSS_Pulse()					do { Display3_nSS_SetHIGH(); delay_loopcnt(LOOPCOUNT_50NS); Display3_nSS_SetLOW(); } while (0)
#define Display3_nSS_InvPulse()					do { Display3_nSS_SetLOW(); delay_loopcnt(LOOPCOUNT_50NS); Display3_nSS_SetHIGH(); } while (0)
#define Display3_nSS_SetTo(value)				if (value) { Display3_nSS_SetHIGH(); } else { Display3_nSS_SetLOW(); }
#define Display3_nSS_GetValue()					(Display3_nSS_GPIO->IDR & Display3_nSS_Pin)
#define Display3_nSS_GPIO_Definition			{ .name = "Display3_nSS", .pin_name = "B2", .gpio = Display3_nSS_GPIO, .pin = Display3_nSS_Pin }

// PB3: Display4_nSS (OUT)
#define Display4_nSS_GPIO						GPIOB
#define Display4_nSS_PinSource					GPIO_PinSource3
#define Display4_nSS_Pin						GPIO_Pin_3
#define Display4_nSS_SetHIGH()					Display4_nSS_GPIO->BSRRL = Display4_nSS_Pin
#define Display4_nSS_SetLOW()					Display4_nSS_GPIO->BSRRH = Display4_nSS_Pin
#define Display4_nSS_Toggle()					Display4_nSS_GPIO->ODR ^= Display4_nSS_Pin
#define Display4_nSS_Pulse()					do { Display4_nSS_SetHIGH(); delay_loopcnt(LOOPCOUNT_50NS); Display4_nSS_SetLOW(); } while (0)
#define Display4_nSS_InvPulse()					do { Display4_nSS_SetLOW(); delay_loopcnt(LOOPCOUNT_50NS); Display4_nSS_SetHIGH(); } while (0)
#define Display4_nSS_SetTo(value)				if (value) { Display4_nSS_SetHIGH(); } else { Display4_nSS_SetLOW(); }
#define Display4_nSS_GetValue()					(Display4_nSS_GPIO->IDR & Display4_nSS_Pin)
#define Display4_nSS_GPIO_Definition			{ .name = "Display4_nSS", .pin_name = "B3", .gpio = Display4_nSS_GPIO, .pin = Display4_nSS_Pin }

// PB4: Display5_nSS (OUT)
#define Display5_nSS_GPIO						GPIOB
#define Display5_nSS_PinSource					GPIO_PinSource4
#define Display5_nSS_Pin						GPIO_Pin_4
#define Display5_nSS_SetHIGH()					Display5_nSS_GPIO->BSRRL = Display5_nSS_Pin
#define Display5_nSS_SetLOW()					Display5_nSS_GPIO->BSRRH = Display5_nSS_Pin
#define Display5_nSS_Toggle()					Display5_nSS_GPIO->ODR ^= Display5_nSS_Pin
#define Display5_nSS_Pulse()					do { Display5_nSS_SetHIGH(); delay_loopcnt(LOOPCOUNT_50NS); Display5_nSS_SetLOW(); } while (0)
#define Display5_nSS_InvPulse()					do { Display5_nSS_SetLOW(); delay_loopcnt(LOOPCOUNT_50NS); Display5_nSS_SetHIGH(); } while (0)
#define Display5_nSS_SetTo(value)				if (value) { Display5_nSS_SetHIGH(); } else { Display5_nSS_SetLOW(); }
#define Display5_nSS_GetValue()					(Display5_nSS_GPIO->IDR & Display5_nSS_Pin)
#define Display5_nSS_GPIO_Definition			{ .name = "Display5_nSS", .pin_name = "B4", .gpio = Display5_nSS_GPIO, .pin = Display5_nSS_Pin }

// PB5: Display6_nSS (OUT)
#define Display6_nSS_GPIO						GPIOB
#define Display6_nSS_PinSource					GPIO_PinSource5
#define Display6_nSS_Pin						GPIO_Pin_5
#define Display6_nSS_SetHIGH()					Display6_nSS_GPIO->BSRRL = Display6_nSS_Pin
#define Display6_nSS_SetLOW()					Display6_nSS_GPIO->BSRRH = Display6_nSS_Pin
#define Display6_nSS_Toggle()					Display6_nSS_GPIO->ODR ^= Display6_nSS_Pin
#define Display6_nSS_Pulse()					do { Display6_nSS_SetHIGH(); delay_loopcnt(LOOPCOUNT_50NS); Display6_nSS_SetLOW(); } while (0)
#define Display6_nSS_InvPulse()					do { Display6_nSS_SetLOW(); delay_loopcnt(LOOPCOUNT_50NS); Display6_nSS_SetHIGH(); } while (0)
#define Display6_nSS_SetTo(value)				if (value) { Display6_nSS_SetHIGH(); } else { Display6_nSS_SetLOW(); }
#define Display6_nSS_GetValue()					(Display6_nSS_GPIO->IDR & Display6_nSS_Pin)
#define Display6_nSS_GPIO_Definition			{ .name = "Display6_nSS", .pin_name = "B5", .gpio = Display6_nSS_GPIO, .pin = Display6_nSS_Pin }

// PB6: Audio_SCL (IO)
#define Audio_SCL_GPIO							GPIOB
#define Audio_SCL_PinSource						GPIO_PinSource6
#define Audio_SCL_Pin							GPIO_Pin_6
#define Audio_SCL_SetHIGH()						Audio_SCL_GPIO->BSRRL = Audio_SCL_Pin
#define Audio_SCL_SetLOW()						Audio_SCL_GPIO->BSRRH = Audio_SCL_Pin
#define Audio_SCL_Toggle()						Audio_SCL_GPIO->ODR ^= Audio_SCL_Pin
#define Audio_SCL_Pulse()						do { Audio_SCL_SetHIGH(); delay_loopcnt(LOOPCOUNT_50NS); Audio_SCL_SetLOW(); } while (0)
#define Audio_SCL_InvPulse()					do { Audio_SCL_SetLOW(); delay_loopcnt(LOOPCOUNT_50NS); Audio_SCL_SetHIGH(); } while (0)
#define Audio_SCL_SetTo(value)					if (value) { Audio_SCL_SetHIGH(); } else { Audio_SCL_SetLOW(); }
#define Audio_SCL_GetValue()					(Audio_SCL_GPIO->IDR & Audio_SCL_Pin)
#define Audio_SCL_GPIO_Definition				{ .name = "Audio_SCL", .pin_name = "B6", .gpio = Audio_SCL_GPIO, .pin = Audio_SCL_Pin }
// PB6: EEPROM_SCL (IO)
#define EEPROM_SCL_GPIO							GPIOB
#define EEPROM_SCL_PinSource					GPIO_PinSource6
#define EEPROM_SCL_Pin							GPIO_Pin_6
#define EEPROM_SCL_SetHIGH()					EEPROM_SCL_GPIO->BSRRL = EEPROM_SCL_Pin
#define EEPROM_SCL_SetLOW()						EEPROM_SCL_GPIO->BSRRH = EEPROM_SCL_Pin
#define EEPROM_SCL_Toggle()						EEPROM_SCL_GPIO->ODR ^= EEPROM_SCL_Pin
#define EEPROM_SCL_Pulse()						do { EEPROM_SCL_SetHIGH(); delay_loopcnt(LOOPCOUNT_50NS); EEPROM_SCL_SetLOW(); } while (0)
#define EEPROM_SCL_InvPulse()					do { EEPROM_SCL_SetLOW(); delay_loopcnt(LOOPCOUNT_50NS); EEPROM_SCL_SetHIGH(); } while (0)
#define EEPROM_SCL_SetTo(value)					if (value) { EEPROM_SCL_SetHIGH(); } else { EEPROM_SCL_SetLOW(); }
#define EEPROM_SCL_GetValue()					(EEPROM_SCL_GPIO->IDR & EEPROM_SCL_Pin)
#define EEPROM_SCL_GPIO_Definition				{ .name = "EEPROM_SCL", .pin_name = "B6", .gpio = EEPROM_SCL_GPIO, .pin = EEPROM_SCL_Pin }

// PB7: Display7_nSS (OUT)
#define Display7_nSS_GPIO						GPIOB
#define Display7_nSS_PinSource					GPIO_PinSource7
#define Display7_nSS_Pin						GPIO_Pin_7
#define Display7_nSS_SetHIGH()					Display7_nSS_GPIO->BSRRL = Display7_nSS_Pin
#define Display7_nSS_SetLOW()					Display7_nSS_GPIO->BSRRH = Display7_nSS_Pin
#define Display7_nSS_Toggle()					Display7_nSS_GPIO->ODR ^= Display7_nSS_Pin
#define Display7_nSS_Pulse()					do { Display7_nSS_SetHIGH(); delay_loopcnt(LOOPCOUNT_50NS); Display7_nSS_SetLOW(); } while (0)
#define Display7_nSS_InvPulse()					do { Display7_nSS_SetLOW(); delay_loopcnt(LOOPCOUNT_50NS); Display7_nSS_SetHIGH(); } while (0)
#define Display7_nSS_SetTo(value)				if (value) { Display7_nSS_SetHIGH(); } else { Display7_nSS_SetLOW(); }
#define Display7_nSS_GetValue()					(Display7_nSS_GPIO->IDR & Display7_nSS_Pin)
#define Display7_nSS_GPIO_Definition			{ .name = "Display7_nSS", .pin_name = "B7", .gpio = Display7_nSS_GPIO, .pin = Display7_nSS_Pin }

// PB9: Audio_SDA (IO)
#define Audio_SDA_GPIO							GPIOB
#define Audio_SDA_PinSource						GPIO_PinSource9
#define Audio_SDA_Pin							GPIO_Pin_9
#define Audio_SDA_SetHIGH()						Audio_SDA_GPIO->BSRRL = Audio_SDA_Pin
#define Audio_SDA_SetLOW()						Audio_SDA_GPIO->BSRRH = Audio_SDA_Pin
#define Audio_SDA_Toggle()						Audio_SDA_GPIO->ODR ^= Audio_SDA_Pin
#define Audio_SDA_Pulse()						do { Audio_SDA_SetHIGH(); delay_loopcnt(LOOPCOUNT_50NS); Audio_SDA_SetLOW(); } while (0)
#define Audio_SDA_InvPulse()					do { Audio_SDA_SetLOW(); delay_loopcnt(LOOPCOUNT_50NS); Audio_SDA_SetHIGH(); } while (0)
#define Audio_SDA_SetTo(value)					if (value) { Audio_SDA_SetHIGH(); } else { Audio_SDA_SetLOW(); }
#define Audio_SDA_GetValue()					(Audio_SDA_GPIO->IDR & Audio_SDA_Pin)
#define Audio_SDA_GPIO_Definition				{ .name = "Audio_SDA", .pin_name = "B9", .gpio = Audio_SDA_GPIO, .pin = Audio_SDA_Pin }
// PB9: EEPROM_SDA (IO)
#define EEPROM_SDA_GPIO							GPIOB
#define EEPROM_SDA_PinSource					GPIO_PinSource9
#define EEPROM_SDA_Pin							GPIO_Pin_9
#define EEPROM_SDA_SetHIGH()					EEPROM_SDA_GPIO->BSRRL = EEPROM_SDA_Pin
#define EEPROM_SDA_SetLOW()						EEPROM_SDA_GPIO->BSRRH = EEPROM_SDA_Pin
#define EEPROM_SDA_Toggle()						EEPROM_SDA_GPIO->ODR ^= EEPROM_SDA_Pin
#define EEPROM_SDA_Pulse()						do { EEPROM_SDA_SetHIGH(); delay_loopcnt(LOOPCOUNT_50NS); EEPROM_SDA_SetLOW(); } while (0)
#define EEPROM_SDA_InvPulse()					do { EEPROM_SDA_SetLOW(); delay_loopcnt(LOOPCOUNT_50NS); EEPROM_SDA_SetHIGH(); } while (0)
#define EEPROM_SDA_SetTo(value)					if (value) { EEPROM_SDA_SetHIGH(); } else { EEPROM_SDA_SetLOW(); }
#define EEPROM_SDA_GetValue()					(EEPROM_SDA_GPIO->IDR & EEPROM_SDA_Pin)
#define EEPROM_SDA_GPIO_Definition				{ .name = "EEPROM_SDA", .pin_name = "B9", .gpio = EEPROM_SDA_GPIO, .pin = EEPROM_SDA_Pin }

// PB10: Audio_CLK_IN (OUT)
#define Audio_CLK_IN_GPIO						GPIOB
#define Audio_CLK_IN_PinSource					GPIO_PinSource10
#define Audio_CLK_IN_Pin						GPIO_Pin_10
#define Audio_CLK_IN_SetHIGH()					Audio_CLK_IN_GPIO->BSRRL = Audio_CLK_IN_Pin
#define Audio_CLK_IN_SetLOW()					Audio_CLK_IN_GPIO->BSRRH = Audio_CLK_IN_Pin
#define Audio_CLK_IN_Toggle()					Audio_CLK_IN_GPIO->ODR ^= Audio_CLK_IN_Pin
#define Audio_CLK_IN_Pulse()					do { Audio_CLK_IN_SetHIGH(); delay_loopcnt(LOOPCOUNT_50NS); Audio_CLK_IN_SetLOW(); } while (0)
#define Audio_CLK_IN_InvPulse()					do { Audio_CLK_IN_SetLOW(); delay_loopcnt(LOOPCOUNT_50NS); Audio_CLK_IN_SetHIGH(); } while (0)
#define Audio_CLK_IN_SetTo(value)				if (value) { Audio_CLK_IN_SetHIGH(); } else { Audio_CLK_IN_SetLOW(); }
#define Audio_CLK_IN_GetValue()					(Audio_CLK_IN_GPIO->IDR & Audio_CLK_IN_Pin)
#define Audio_CLK_IN_GPIO_Definition			{ .name = "Audio_CLK_IN", .pin_name = "B10", .gpio = Audio_CLK_IN_GPIO, .pin = Audio_CLK_IN_Pin }

// PB13: Display_SCK (OUT)
#define Display_SCK_GPIO						GPIOB
#define Display_SCK_PinSource					GPIO_PinSource13
#define Display_SCK_Pin							GPIO_Pin_13
#define Display_SCK_SetHIGH()					Display_SCK_GPIO->BSRRL = Display_SCK_Pin
#define Display_SCK_SetLOW()					Display_SCK_GPIO->BSRRH = Display_SCK_Pin
#define Display_SCK_Toggle()					Display_SCK_GPIO->ODR ^= Display_SCK_Pin
#define Display_SCK_Pulse()						do { Display_SCK_SetHIGH(); delay_loopcnt(LOOPCOUNT_50NS); Display_SCK_SetLOW(); } while (0)
#define Display_SCK_InvPulse()					do { Display_SCK_SetLOW(); delay_loopcnt(LOOPCOUNT_50NS); Display_SCK_SetHIGH(); } while (0)
#define Display_SCK_SetTo(value)				if (value) { Display_SCK_SetHIGH(); } else { Display_SCK_SetLOW(); }
#define Display_SCK_GetValue()					(Display_SCK_GPIO->IDR & Display_SCK_Pin)
#define Display_SCK_GPIO_Definition				{ .name = "Display_SCK", .pin_name = "B13", .gpio = Display_SCK_GPIO, .pin = Display_SCK_Pin }

// PB15: Display_MOSI (OUT)
#define Display_MOSI_GPIO						GPIOB
#define Display_MOSI_PinSource					GPIO_PinSource15
#define Display_MOSI_Pin						GPIO_Pin_15
#define Display_MOSI_SetHIGH()					Display_MOSI_GPIO->BSRRL = Display_MOSI_Pin
#define Display_MOSI_SetLOW()					Display_MOSI_GPIO->BSRRH = Display_MOSI_Pin
#define Display_MOSI_Toggle()					Display_MOSI_GPIO->ODR ^= Display_MOSI_Pin
#define Display_MOSI_Pulse()					do { Display_MOSI_SetHIGH(); delay_loopcnt(LOOPCOUNT_50NS); Display_MOSI_SetLOW(); } while (0)
#define Display_MOSI_InvPulse()					do { Display_MOSI_SetLOW(); delay_loopcnt(LOOPCOUNT_50NS); Display_MOSI_SetHIGH(); } while (0)
#define Display_MOSI_SetTo(value)				if (value) { Display_MOSI_SetHIGH(); } else { Display_MOSI_SetLOW(); }
#define Display_MOSI_GetValue()					(Display_MOSI_GPIO->IDR & Display_MOSI_Pin)
#define Display_MOSI_GPIO_Definition			{ .name = "Display_MOSI", .pin_name = "B15", .gpio = Display_MOSI_GPIO, .pin = Display_MOSI_Pin }

// PC0: USB_PowerSwitchOn (OUT)
#define USB_PowerSwitchOn_GPIO					GPIOC
#define USB_PowerSwitchOn_PinSource				GPIO_PinSource0
#define USB_PowerSwitchOn_Pin					GPIO_Pin_0
#define USB_PowerSwitchOn_SetHIGH()				USB_PowerSwitchOn_GPIO->BSRRL = USB_PowerSwitchOn_Pin
#define USB_PowerSwitchOn_SetLOW()				USB_PowerSwitchOn_GPIO->BSRRH = USB_PowerSwitchOn_Pin
#define USB_PowerSwitchOn_Toggle()				USB_PowerSwitchOn_GPIO->ODR ^= USB_PowerSwitchOn_Pin
#define USB_PowerSwitchOn_Pulse()				do { USB_PowerSwitchOn_SetHIGH(); delay_loopcnt(LOOPCOUNT_50NS); USB_PowerSwitchOn_SetLOW(); } while (0)
#define USB_PowerSwitchOn_InvPulse()			do { USB_PowerSwitchOn_SetLOW(); delay_loopcnt(LOOPCOUNT_50NS); USB_PowerSwitchOn_SetHIGH(); } while (0)
#define USB_PowerSwitchOn_SetTo(value)			if (value) { USB_PowerSwitchOn_SetHIGH(); } else { USB_PowerSwitchOn_SetLOW(); }
#define USB_PowerSwitchOn_GetValue()			(USB_PowerSwitchOn_GPIO->IDR & USB_PowerSwitchOn_Pin)
#define USB_PowerSwitchOn_GPIO_Definition		{ .name = "USB_PowerSwitchOn", .pin_name = "C0", .gpio = USB_PowerSwitchOn_GPIO, .pin = USB_PowerSwitchOn_Pin }
// PC0: IOMux_In_PE (OUT)
#define IOMux_In_PE_GPIO						GPIOC
#define IOMux_In_PE_PinSource					GPIO_PinSource0
#define IOMux_In_PE_Pin							GPIO_Pin_0
#define IOMux_In_PE_SetHIGH()					IOMux_In_PE_GPIO->BSRRL = IOMux_In_PE_Pin
#define IOMux_In_PE_SetLOW()					IOMux_In_PE_GPIO->BSRRH = IOMux_In_PE_Pin
#define IOMux_In_PE_Toggle()					IOMux_In_PE_GPIO->ODR ^= IOMux_In_PE_Pin
#define IOMux_In_PE_Pulse()						do { IOMux_In_PE_SetHIGH(); delay_loopcnt(LOOPCOUNT_50NS); IOMux_In_PE_SetLOW(); } while (0)
#define IOMux_In_PE_InvPulse()					do { IOMux_In_PE_SetLOW(); delay_loopcnt(LOOPCOUNT_50NS); IOMux_In_PE_SetHIGH(); } while (0)
#define IOMux_In_PE_SetTo(value)				if (value) { IOMux_In_PE_SetHIGH(); } else { IOMux_In_PE_SetLOW(); }
#define IOMux_In_PE_GetValue()					(IOMux_In_PE_GPIO->IDR & IOMux_In_PE_Pin)
#define IOMux_In_PE_GPIO_Definition				{ .name = "IOMux_In_PE", .pin_name = "C0", .gpio = IOMux_In_PE_GPIO, .pin = IOMux_In_PE_Pin }

// PC1: IOMux_Out_OE (OUT)
#define IOMux_Out_OE_GPIO						GPIOC
#define IOMux_Out_OE_PinSource					GPIO_PinSource1
#define IOMux_Out_OE_Pin						GPIO_Pin_1
#define IOMux_Out_OE_SetHIGH()					IOMux_Out_OE_GPIO->BSRRL = IOMux_Out_OE_Pin
#define IOMux_Out_OE_SetLOW()					IOMux_Out_OE_GPIO->BSRRH = IOMux_Out_OE_Pin
#define IOMux_Out_OE_Toggle()					IOMux_Out_OE_GPIO->ODR ^= IOMux_Out_OE_Pin
#define IOMux_Out_OE_Pulse()					do { IOMux_Out_OE_SetHIGH(); delay_loopcnt(LOOPCOUNT_50NS); IOMux_Out_OE_SetLOW(); } while (0)
#define IOMux_Out_OE_InvPulse()					do { IOMux_Out_OE_SetLOW(); delay_loopcnt(LOOPCOUNT_50NS); IOMux_Out_OE_SetHIGH(); } while (0)
#define IOMux_Out_OE_SetTo(value)				if (value) { IOMux_Out_OE_SetHIGH(); } else { IOMux_Out_OE_SetLOW(); }
#define IOMux_Out_OE_GetValue()					(IOMux_Out_OE_GPIO->IDR & IOMux_Out_OE_Pin)
#define IOMux_Out_OE_GPIO_Definition			{ .name = "IOMux_Out_OE", .pin_name = "C1", .gpio = IOMux_Out_OE_GPIO, .pin = IOMux_Out_OE_Pin }

// PC2: IOMux_Out_STCP (OUT)
#define IOMux_Out_STCP_GPIO						GPIOC
#define IOMux_Out_STCP_PinSource				GPIO_PinSource2
#define IOMux_Out_STCP_Pin						GPIO_Pin_2
#define IOMux_Out_STCP_SetHIGH()				IOMux_Out_STCP_GPIO->BSRRL = IOMux_Out_STCP_Pin
#define IOMux_Out_STCP_SetLOW()					IOMux_Out_STCP_GPIO->BSRRH = IOMux_Out_STCP_Pin
#define IOMux_Out_STCP_Toggle()					IOMux_Out_STCP_GPIO->ODR ^= IOMux_Out_STCP_Pin
#define IOMux_Out_STCP_Pulse()					do { IOMux_Out_STCP_SetHIGH(); delay_loopcnt(LOOPCOUNT_50NS); IOMux_Out_STCP_SetLOW(); } while (0)
#define IOMux_Out_STCP_InvPulse()				do { IOMux_Out_STCP_SetLOW(); delay_loopcnt(LOOPCOUNT_50NS); IOMux_Out_STCP_SetHIGH(); } while (0)
#define IOMux_Out_STCP_SetTo(value)				if (value) { IOMux_Out_STCP_SetHIGH(); } else { IOMux_Out_STCP_SetLOW(); }
#define IOMux_Out_STCP_GetValue()				(IOMux_Out_STCP_GPIO->IDR & IOMux_Out_STCP_Pin)
#define IOMux_Out_STCP_GPIO_Definition			{ .name = "IOMux_Out_STCP", .pin_name = "C2", .gpio = IOMux_Out_STCP_GPIO, .pin = IOMux_Out_STCP_Pin }

// PC3: AudioPDM_OUT (IN)
#define AudioPDM_OUT_GPIO						GPIOC
#define AudioPDM_OUT_PinSource					GPIO_PinSource3
#define AudioPDM_OUT_Pin						GPIO_Pin_3
#define AudioPDM_OUT_SetHIGH()					AudioPDM_OUT_GPIO->BSRRL = AudioPDM_OUT_Pin
#define AudioPDM_OUT_SetLOW()					AudioPDM_OUT_GPIO->BSRRH = AudioPDM_OUT_Pin
#define AudioPDM_OUT_Toggle()					AudioPDM_OUT_GPIO->ODR ^= AudioPDM_OUT_Pin
#define AudioPDM_OUT_Pulse()					do { AudioPDM_OUT_SetHIGH(); delay_loopcnt(LOOPCOUNT_50NS); AudioPDM_OUT_SetLOW(); } while (0)
#define AudioPDM_OUT_InvPulse()					do { AudioPDM_OUT_SetLOW(); delay_loopcnt(LOOPCOUNT_50NS); AudioPDM_OUT_SetHIGH(); } while (0)
#define AudioPDM_OUT_SetTo(value)				if (value) { AudioPDM_OUT_SetHIGH(); } else { AudioPDM_OUT_SetLOW(); }
#define AudioPDM_OUT_GetValue()					(AudioPDM_OUT_GPIO->IDR & AudioPDM_OUT_Pin)
#define AudioPDM_OUT_GPIO_Definition			{ .name = "AudioPDM_OUT", .pin_name = "C3", .gpio = AudioPDM_OUT_GPIO, .pin = AudioPDM_OUT_Pin }

// PC7: Audio_MCK (?)
#define Audio_MCK_GPIO							GPIOC
#define Audio_MCK_PinSource						GPIO_PinSource7
#define Audio_MCK_Pin							GPIO_Pin_7
#define Audio_MCK_SetHIGH()						Audio_MCK_GPIO->BSRRL = Audio_MCK_Pin
#define Audio_MCK_SetLOW()						Audio_MCK_GPIO->BSRRH = Audio_MCK_Pin
#define Audio_MCK_Toggle()						Audio_MCK_GPIO->ODR ^= Audio_MCK_Pin
#define Audio_MCK_Pulse()						do { Audio_MCK_SetHIGH(); delay_loopcnt(LOOPCOUNT_50NS); Audio_MCK_SetLOW(); } while (0)
#define Audio_MCK_InvPulse()					do { Audio_MCK_SetLOW(); delay_loopcnt(LOOPCOUNT_50NS); Audio_MCK_SetHIGH(); } while (0)
#define Audio_MCK_SetTo(value)					if (value) { Audio_MCK_SetHIGH(); } else { Audio_MCK_SetLOW(); }
#define Audio_MCK_GetValue()					(Audio_MCK_GPIO->IDR & Audio_MCK_Pin)
#define Audio_MCK_GPIO_Definition				{ .name = "Audio_MCK", .pin_name = "C7", .gpio = Audio_MCK_GPIO, .pin = Audio_MCK_Pin }

// PC10: Audio_SCK (?)
#define Audio_SCK_GPIO							GPIOC
#define Audio_SCK_PinSource						GPIO_PinSource10
#define Audio_SCK_Pin							GPIO_Pin_10
#define Audio_SCK_SetHIGH()						Audio_SCK_GPIO->BSRRL = Audio_SCK_Pin
#define Audio_SCK_SetLOW()						Audio_SCK_GPIO->BSRRH = Audio_SCK_Pin
#define Audio_SCK_Toggle()						Audio_SCK_GPIO->ODR ^= Audio_SCK_Pin
#define Audio_SCK_Pulse()						do { Audio_SCK_SetHIGH(); delay_loopcnt(LOOPCOUNT_50NS); Audio_SCK_SetLOW(); } while (0)
#define Audio_SCK_InvPulse()					do { Audio_SCK_SetLOW(); delay_loopcnt(LOOPCOUNT_50NS); Audio_SCK_SetHIGH(); } while (0)
#define Audio_SCK_SetTo(value)					if (value) { Audio_SCK_SetHIGH(); } else { Audio_SCK_SetLOW(); }
#define Audio_SCK_GetValue()					(Audio_SCK_GPIO->IDR & Audio_SCK_Pin)
#define Audio_SCK_GPIO_Definition				{ .name = "Audio_SCK", .pin_name = "C10", .gpio = Audio_SCK_GPIO, .pin = Audio_SCK_Pin }
// PC10: IOMux_SCK (OUT)
#define IOMux_SCK_GPIO							GPIOC
#define IOMux_SCK_PinSource						GPIO_PinSource10
#define IOMux_SCK_Pin							GPIO_Pin_10
#define IOMux_SCK_SetHIGH()						IOMux_SCK_GPIO->BSRRL = IOMux_SCK_Pin
#define IOMux_SCK_SetLOW()						IOMux_SCK_GPIO->BSRRH = IOMux_SCK_Pin
#define IOMux_SCK_Toggle()						IOMux_SCK_GPIO->ODR ^= IOMux_SCK_Pin
#define IOMux_SCK_Pulse()						do { IOMux_SCK_SetHIGH(); delay_loopcnt(LOOPCOUNT_50NS); IOMux_SCK_SetLOW(); } while (0)
#define IOMux_SCK_InvPulse()					do { IOMux_SCK_SetLOW(); delay_loopcnt(LOOPCOUNT_50NS); IOMux_SCK_SetHIGH(); } while (0)
#define IOMux_SCK_SetTo(value)					if (value) { IOMux_SCK_SetHIGH(); } else { IOMux_SCK_SetLOW(); }
#define IOMux_SCK_GetValue()					(IOMux_SCK_GPIO->IDR & IOMux_SCK_Pin)
#define IOMux_SCK_GPIO_Definition				{ .name = "IOMux_SCK", .pin_name = "C10", .gpio = IOMux_SCK_GPIO, .pin = IOMux_SCK_Pin }

// PC11: IOMux_MISO (IN)
#define IOMux_MISO_GPIO							GPIOC
#define IOMux_MISO_PinSource					GPIO_PinSource11
#define IOMux_MISO_Pin							GPIO_Pin_11
#define IOMux_MISO_SetHIGH()					IOMux_MISO_GPIO->BSRRL = IOMux_MISO_Pin
#define IOMux_MISO_SetLOW()						IOMux_MISO_GPIO->BSRRH = IOMux_MISO_Pin
#define IOMux_MISO_Toggle()						IOMux_MISO_GPIO->ODR ^= IOMux_MISO_Pin
#define IOMux_MISO_Pulse()						do { IOMux_MISO_SetHIGH(); delay_loopcnt(LOOPCOUNT_50NS); IOMux_MISO_SetLOW(); } while (0)
#define IOMux_MISO_InvPulse()					do { IOMux_MISO_SetLOW(); delay_loopcnt(LOOPCOUNT_50NS); IOMux_MISO_SetHIGH(); } while (0)
#define IOMux_MISO_SetTo(value)					if (value) { IOMux_MISO_SetHIGH(); } else { IOMux_MISO_SetLOW(); }
#define IOMux_MISO_GetValue()					(IOMux_MISO_GPIO->IDR & IOMux_MISO_Pin)
#define IOMux_MISO_GPIO_Definition				{ .name = "IOMux_MISO", .pin_name = "C11", .gpio = IOMux_MISO_GPIO, .pin = IOMux_MISO_Pin }

// PC12: Audio_SD (?)
#define Audio_SD_GPIO							GPIOC
#define Audio_SD_PinSource						GPIO_PinSource12
#define Audio_SD_Pin							GPIO_Pin_12
#define Audio_SD_SetHIGH()						Audio_SD_GPIO->BSRRL = Audio_SD_Pin
#define Audio_SD_SetLOW()						Audio_SD_GPIO->BSRRH = Audio_SD_Pin
#define Audio_SD_Toggle()						Audio_SD_GPIO->ODR ^= Audio_SD_Pin
#define Audio_SD_Pulse()						do { Audio_SD_SetHIGH(); delay_loopcnt(LOOPCOUNT_50NS); Audio_SD_SetLOW(); } while (0)
#define Audio_SD_InvPulse()						do { Audio_SD_SetLOW(); delay_loopcnt(LOOPCOUNT_50NS); Audio_SD_SetHIGH(); } while (0)
#define Audio_SD_SetTo(value)					if (value) { Audio_SD_SetHIGH(); } else { Audio_SD_SetLOW(); }
#define Audio_SD_GetValue()						(Audio_SD_GPIO->IDR & Audio_SD_Pin)
#define Audio_SD_GPIO_Definition				{ .name = "Audio_SD", .pin_name = "C12", .gpio = Audio_SD_GPIO, .pin = Audio_SD_Pin }
// PC12: IOMux_MOSI (OUT)
#define IOMux_MOSI_GPIO							GPIOC
#define IOMux_MOSI_PinSource					GPIO_PinSource12
#define IOMux_MOSI_Pin							GPIO_Pin_12
#define IOMux_MOSI_SetHIGH()					IOMux_MOSI_GPIO->BSRRL = IOMux_MOSI_Pin
#define IOMux_MOSI_SetLOW()						IOMux_MOSI_GPIO->BSRRH = IOMux_MOSI_Pin
#define IOMux_MOSI_Toggle()						IOMux_MOSI_GPIO->ODR ^= IOMux_MOSI_Pin
#define IOMux_MOSI_Pulse()						do { IOMux_MOSI_SetHIGH(); delay_loopcnt(LOOPCOUNT_50NS); IOMux_MOSI_SetLOW(); } while (0)
#define IOMux_MOSI_InvPulse()					do { IOMux_MOSI_SetLOW(); delay_loopcnt(LOOPCOUNT_50NS); IOMux_MOSI_SetHIGH(); } while (0)
#define IOMux_MOSI_SetTo(value)					if (value) { IOMux_MOSI_SetHIGH(); } else { IOMux_MOSI_SetLOW(); }
#define IOMux_MOSI_GetValue()					(IOMux_MOSI_GPIO->IDR & IOMux_MOSI_Pin)
#define IOMux_MOSI_GPIO_Definition				{ .name = "IOMux_MOSI", .pin_name = "C12", .gpio = IOMux_MOSI_GPIO, .pin = IOMux_MOSI_Pin }

// PD0: Display11_nSS (OUT)
#define Display11_nSS_GPIO						GPIOD
#define Display11_nSS_PinSource					GPIO_PinSource0
#define Display11_nSS_Pin						GPIO_Pin_0
#define Display11_nSS_SetHIGH()					Display11_nSS_GPIO->BSRRL = Display11_nSS_Pin
#define Display11_nSS_SetLOW()					Display11_nSS_GPIO->BSRRH = Display11_nSS_Pin
#define Display11_nSS_Toggle()					Display11_nSS_GPIO->ODR ^= Display11_nSS_Pin
#define Display11_nSS_Pulse()					do { Display11_nSS_SetHIGH(); delay_loopcnt(LOOPCOUNT_50NS); Display11_nSS_SetLOW(); } while (0)
#define Display11_nSS_InvPulse()				do { Display11_nSS_SetLOW(); delay_loopcnt(LOOPCOUNT_50NS); Display11_nSS_SetHIGH(); } while (0)
#define Display11_nSS_SetTo(value)				if (value) { Display11_nSS_SetHIGH(); } else { Display11_nSS_SetLOW(); }
#define Display11_nSS_GetValue()				(Display11_nSS_GPIO->IDR & Display11_nSS_Pin)
#define Display11_nSS_GPIO_Definition			{ .name = "Display11_nSS", .pin_name = "D0", .gpio = Display11_nSS_GPIO, .pin = Display11_nSS_Pin }

// PD1: Display_nRESET (OUT)
#define Display_nRESET_GPIO						GPIOD
#define Display_nRESET_PinSource				GPIO_PinSource1
#define Display_nRESET_Pin						GPIO_Pin_1
#define Display_nRESET_SetHIGH()				Display_nRESET_GPIO->BSRRL = Display_nRESET_Pin
#define Display_nRESET_SetLOW()					Display_nRESET_GPIO->BSRRH = Display_nRESET_Pin
#define Display_nRESET_Toggle()					Display_nRESET_GPIO->ODR ^= Display_nRESET_Pin
#define Display_nRESET_Pulse()					do { Display_nRESET_SetHIGH(); delay_loopcnt(LOOPCOUNT_50NS); Display_nRESET_SetLOW(); } while (0)
#define Display_nRESET_InvPulse()				do { Display_nRESET_SetLOW(); delay_loopcnt(LOOPCOUNT_50NS); Display_nRESET_SetHIGH(); } while (0)
#define Display_nRESET_SetTo(value)				if (value) { Display_nRESET_SetHIGH(); } else { Display_nRESET_SetLOW(); }
#define Display_nRESET_GetValue()				(Display_nRESET_GPIO->IDR & Display_nRESET_Pin)
#define Display_nRESET_GPIO_Definition			{ .name = "Display_nRESET", .pin_name = "D1", .gpio = Display_nRESET_GPIO, .pin = Display_nRESET_Pin }

// PD2: Display_DC (OUT)
#define Display_DC_GPIO							GPIOD
#define Display_DC_PinSource					GPIO_PinSource2
#define Display_DC_Pin							GPIO_Pin_2
#define Display_DC_SetHIGH()					Display_DC_GPIO->BSRRL = Display_DC_Pin
#define Display_DC_SetLOW()						Display_DC_GPIO->BSRRH = Display_DC_Pin
#define Display_DC_Toggle()						Display_DC_GPIO->ODR ^= Display_DC_Pin
#define Display_DC_Pulse()						do { Display_DC_SetHIGH(); delay_loopcnt(LOOPCOUNT_50NS); Display_DC_SetLOW(); } while (0)
#define Display_DC_InvPulse()					do { Display_DC_SetLOW(); delay_loopcnt(LOOPCOUNT_50NS); Display_DC_SetHIGH(); } while (0)
#define Display_DC_SetTo(value)					if (value) { Display_DC_SetHIGH(); } else { Display_DC_SetLOW(); }
#define Display_DC_GetValue()					(Display_DC_GPIO->IDR & Display_DC_Pin)
#define Display_DC_GPIO_Definition				{ .name = "Display_DC", .pin_name = "D2", .gpio = Display_DC_GPIO, .pin = Display_DC_Pin }

// PD3: Display8_nSS (OUT)
#define Display8_nSS_GPIO						GPIOD
#define Display8_nSS_PinSource					GPIO_PinSource3
#define Display8_nSS_Pin						GPIO_Pin_3
#define Display8_nSS_SetHIGH()					Display8_nSS_GPIO->BSRRL = Display8_nSS_Pin
#define Display8_nSS_SetLOW()					Display8_nSS_GPIO->BSRRH = Display8_nSS_Pin
#define Display8_nSS_Toggle()					Display8_nSS_GPIO->ODR ^= Display8_nSS_Pin
#define Display8_nSS_Pulse()					do { Display8_nSS_SetHIGH(); delay_loopcnt(LOOPCOUNT_50NS); Display8_nSS_SetLOW(); } while (0)
#define Display8_nSS_InvPulse()					do { Display8_nSS_SetLOW(); delay_loopcnt(LOOPCOUNT_50NS); Display8_nSS_SetHIGH(); } while (0)
#define Display8_nSS_SetTo(value)				if (value) { Display8_nSS_SetHIGH(); } else { Display8_nSS_SetLOW(); }
#define Display8_nSS_GetValue()					(Display8_nSS_GPIO->IDR & Display8_nSS_Pin)
#define Display8_nSS_GPIO_Definition			{ .name = "Display8_nSS", .pin_name = "D3", .gpio = Display8_nSS_GPIO, .pin = Display8_nSS_Pin }

// PD4: AudioReset (OUT)
#define AudioReset_GPIO							GPIOD
#define AudioReset_PinSource					GPIO_PinSource4
#define AudioReset_Pin							GPIO_Pin_4
#define AudioReset_SetHIGH()					AudioReset_GPIO->BSRRL = AudioReset_Pin
#define AudioReset_SetLOW()						AudioReset_GPIO->BSRRH = AudioReset_Pin
#define AudioReset_Toggle()						AudioReset_GPIO->ODR ^= AudioReset_Pin
#define AudioReset_Pulse()						do { AudioReset_SetHIGH(); delay_loopcnt(LOOPCOUNT_50NS); AudioReset_SetLOW(); } while (0)
#define AudioReset_InvPulse()					do { AudioReset_SetLOW(); delay_loopcnt(LOOPCOUNT_50NS); AudioReset_SetHIGH(); } while (0)
#define AudioReset_SetTo(value)					if (value) { AudioReset_SetHIGH(); } else { AudioReset_SetLOW(); }
#define AudioReset_GetValue()					(AudioReset_GPIO->IDR & AudioReset_Pin)
#define AudioReset_GPIO_Definition				{ .name = "AudioReset", .pin_name = "D4", .gpio = AudioReset_GPIO, .pin = AudioReset_Pin }
// PD4: Display9_nSS (OUT)
#define Display9_nSS_GPIO						GPIOD
#define Display9_nSS_PinSource					GPIO_PinSource4
#define Display9_nSS_Pin						GPIO_Pin_4
#define Display9_nSS_SetHIGH()					Display9_nSS_GPIO->BSRRL = Display9_nSS_Pin
#define Display9_nSS_SetLOW()					Display9_nSS_GPIO->BSRRH = Display9_nSS_Pin
#define Display9_nSS_Toggle()					Display9_nSS_GPIO->ODR ^= Display9_nSS_Pin
#define Display9_nSS_Pulse()					do { Display9_nSS_SetHIGH(); delay_loopcnt(LOOPCOUNT_50NS); Display9_nSS_SetLOW(); } while (0)
#define Display9_nSS_InvPulse()					do { Display9_nSS_SetLOW(); delay_loopcnt(LOOPCOUNT_50NS); Display9_nSS_SetHIGH(); } while (0)
#define Display9_nSS_SetTo(value)				if (value) { Display9_nSS_SetHIGH(); } else { Display9_nSS_SetLOW(); }
#define Display9_nSS_GetValue()					(Display9_nSS_GPIO->IDR & Display9_nSS_Pin)
#define Display9_nSS_GPIO_Definition			{ .name = "Display9_nSS", .pin_name = "D4", .gpio = Display9_nSS_GPIO, .pin = Display9_nSS_Pin }

// PD5: USB_FS_OverCurrent (IN)
#define USB_FS_OverCurrent_GPIO					GPIOD
#define USB_FS_OverCurrent_PinSource			GPIO_PinSource5
#define USB_FS_OverCurrent_Pin					GPIO_Pin_5
#define USB_FS_OverCurrent_SetHIGH()			USB_FS_OverCurrent_GPIO->BSRRL = USB_FS_OverCurrent_Pin
#define USB_FS_OverCurrent_SetLOW()				USB_FS_OverCurrent_GPIO->BSRRH = USB_FS_OverCurrent_Pin
#define USB_FS_OverCurrent_Toggle()				USB_FS_OverCurrent_GPIO->ODR ^= USB_FS_OverCurrent_Pin
#define USB_FS_OverCurrent_Pulse()				do { USB_FS_OverCurrent_SetHIGH(); delay_loopcnt(LOOPCOUNT_50NS); USB_FS_OverCurrent_SetLOW(); } while (0)
#define USB_FS_OverCurrent_InvPulse()			do { USB_FS_OverCurrent_SetLOW(); delay_loopcnt(LOOPCOUNT_50NS); USB_FS_OverCurrent_SetHIGH(); } while (0)
#define USB_FS_OverCurrent_SetTo(value)			if (value) { USB_FS_OverCurrent_SetHIGH(); } else { USB_FS_OverCurrent_SetLOW(); }
#define USB_FS_OverCurrent_GetValue()			(USB_FS_OverCurrent_GPIO->IDR & USB_FS_OverCurrent_Pin)
#define USB_FS_OverCurrent_GPIO_Definition		{ .name = "USB_FS_OverCurrent", .pin_name = "D5", .gpio = USB_FS_OverCurrent_GPIO, .pin = USB_FS_OverCurrent_Pin }
// PD5: USART_TX (OUT)
#define USART_TX_GPIO							GPIOD
#define USART_TX_PinSource						GPIO_PinSource5
#define USART_TX_Pin							GPIO_Pin_5
#define USART_TX_SetHIGH()						USART_TX_GPIO->BSRRL = USART_TX_Pin
#define USART_TX_SetLOW()						USART_TX_GPIO->BSRRH = USART_TX_Pin
#define USART_TX_Toggle()						USART_TX_GPIO->ODR ^= USART_TX_Pin
#define USART_TX_Pulse()						do { USART_TX_SetHIGH(); delay_loopcnt(LOOPCOUNT_50NS); USART_TX_SetLOW(); } while (0)
#define USART_TX_InvPulse()						do { USART_TX_SetLOW(); delay_loopcnt(LOOPCOUNT_50NS); USART_TX_SetHIGH(); } while (0)
#define USART_TX_SetTo(value)					if (value) { USART_TX_SetHIGH(); } else { USART_TX_SetLOW(); }
#define USART_TX_GetValue()						(USART_TX_GPIO->IDR & USART_TX_Pin)
#define USART_TX_GPIO_Definition				{ .name = "USART_TX", .pin_name = "D5", .gpio = USART_TX_GPIO, .pin = USART_TX_Pin }

// PD6: USART_RX (IN)
#define USART_RX_GPIO							GPIOD
#define USART_RX_PinSource						GPIO_PinSource6
#define USART_RX_Pin							GPIO_Pin_6
#define USART_RX_SetHIGH()						USART_RX_GPIO->BSRRL = USART_RX_Pin
#define USART_RX_SetLOW()						USART_RX_GPIO->BSRRH = USART_RX_Pin
#define USART_RX_Toggle()						USART_RX_GPIO->ODR ^= USART_RX_Pin
#define USART_RX_Pulse()						do { USART_RX_SetHIGH(); delay_loopcnt(LOOPCOUNT_50NS); USART_RX_SetLOW(); } while (0)
#define USART_RX_InvPulse()						do { USART_RX_SetLOW(); delay_loopcnt(LOOPCOUNT_50NS); USART_RX_SetHIGH(); } while (0)
#define USART_RX_SetTo(value)					if (value) { USART_RX_SetHIGH(); } else { USART_RX_SetLOW(); }
#define USART_RX_GetValue()						(USART_RX_GPIO->IDR & USART_RX_Pin)
#define USART_RX_GPIO_Definition				{ .name = "USART_RX", .pin_name = "D6", .gpio = USART_RX_GPIO, .pin = USART_RX_Pin }

// PD7: Display10_nSS (OUT)
#define Display10_nSS_GPIO						GPIOD
#define Display10_nSS_PinSource					GPIO_PinSource7
#define Display10_nSS_Pin						GPIO_Pin_7
#define Display10_nSS_SetHIGH()					Display10_nSS_GPIO->BSRRL = Display10_nSS_Pin
#define Display10_nSS_SetLOW()					Display10_nSS_GPIO->BSRRH = Display10_nSS_Pin
#define Display10_nSS_Toggle()					Display10_nSS_GPIO->ODR ^= Display10_nSS_Pin
#define Display10_nSS_Pulse()					do { Display10_nSS_SetHIGH(); delay_loopcnt(LOOPCOUNT_50NS); Display10_nSS_SetLOW(); } while (0)
#define Display10_nSS_InvPulse()				do { Display10_nSS_SetLOW(); delay_loopcnt(LOOPCOUNT_50NS); Display10_nSS_SetHIGH(); } while (0)
#define Display10_nSS_SetTo(value)				if (value) { Display10_nSS_SetHIGH(); } else { Display10_nSS_SetLOW(); }
#define Display10_nSS_GetValue()				(Display10_nSS_GPIO->IDR & Display10_nSS_Pin)
#define Display10_nSS_GPIO_Definition			{ .name = "Display10_nSS", .pin_name = "D7", .gpio = Display10_nSS_GPIO, .pin = Display10_nSS_Pin }

// PD8: Display12_nSS (OUT)
#define Display12_nSS_GPIO						GPIOD
#define Display12_nSS_PinSource					GPIO_PinSource8
#define Display12_nSS_Pin						GPIO_Pin_8
#define Display12_nSS_SetHIGH()					Display12_nSS_GPIO->BSRRL = Display12_nSS_Pin
#define Display12_nSS_SetLOW()					Display12_nSS_GPIO->BSRRH = Display12_nSS_Pin
#define Display12_nSS_Toggle()					Display12_nSS_GPIO->ODR ^= Display12_nSS_Pin
#define Display12_nSS_Pulse()					do { Display12_nSS_SetHIGH(); delay_loopcnt(LOOPCOUNT_50NS); Display12_nSS_SetLOW(); } while (0)
#define Display12_nSS_InvPulse()				do { Display12_nSS_SetLOW(); delay_loopcnt(LOOPCOUNT_50NS); Display12_nSS_SetHIGH(); } while (0)
#define Display12_nSS_SetTo(value)				if (value) { Display12_nSS_SetHIGH(); } else { Display12_nSS_SetLOW(); }
#define Display12_nSS_GetValue()				(Display12_nSS_GPIO->IDR & Display12_nSS_Pin)
#define Display12_nSS_GPIO_Definition			{ .name = "Display12_nSS", .pin_name = "D8", .gpio = Display12_nSS_GPIO, .pin = Display12_nSS_Pin }

// PD9: Rotary1_Btn (IN_UP)
#define Rotary1_Btn_GPIO						GPIOD
#define Rotary1_Btn_PinSource					GPIO_PinSource9
#define Rotary1_Btn_Pin							GPIO_Pin_9
#define Rotary1_Btn_SetHIGH()					Rotary1_Btn_GPIO->BSRRL = Rotary1_Btn_Pin
#define Rotary1_Btn_SetLOW()					Rotary1_Btn_GPIO->BSRRH = Rotary1_Btn_Pin
#define Rotary1_Btn_Toggle()					Rotary1_Btn_GPIO->ODR ^= Rotary1_Btn_Pin
#define Rotary1_Btn_Pulse()						do { Rotary1_Btn_SetHIGH(); delay_loopcnt(LOOPCOUNT_50NS); Rotary1_Btn_SetLOW(); } while (0)
#define Rotary1_Btn_InvPulse()					do { Rotary1_Btn_SetLOW(); delay_loopcnt(LOOPCOUNT_50NS); Rotary1_Btn_SetHIGH(); } while (0)
#define Rotary1_Btn_SetTo(value)				if (value) { Rotary1_Btn_SetHIGH(); } else { Rotary1_Btn_SetLOW(); }
#define Rotary1_Btn_GetValue()					(Rotary1_Btn_GPIO->IDR & Rotary1_Btn_Pin)
#define Rotary1_Btn_GPIO_Definition				{ .name = "Rotary1_Btn", .pin_name = "D9", .gpio = Rotary1_Btn_GPIO, .pin = Rotary1_Btn_Pin }

// PD10: Rotary1_A (IN_UP)
#define Rotary1_A_GPIO							GPIOD
#define Rotary1_A_PinSource						GPIO_PinSource10
#define Rotary1_A_Pin							GPIO_Pin_10
#define Rotary1_A_SetHIGH()						Rotary1_A_GPIO->BSRRL = Rotary1_A_Pin
#define Rotary1_A_SetLOW()						Rotary1_A_GPIO->BSRRH = Rotary1_A_Pin
#define Rotary1_A_Toggle()						Rotary1_A_GPIO->ODR ^= Rotary1_A_Pin
#define Rotary1_A_Pulse()						do { Rotary1_A_SetHIGH(); delay_loopcnt(LOOPCOUNT_50NS); Rotary1_A_SetLOW(); } while (0)
#define Rotary1_A_InvPulse()					do { Rotary1_A_SetLOW(); delay_loopcnt(LOOPCOUNT_50NS); Rotary1_A_SetHIGH(); } while (0)
#define Rotary1_A_SetTo(value)					if (value) { Rotary1_A_SetHIGH(); } else { Rotary1_A_SetLOW(); }
#define Rotary1_A_GetValue()					(Rotary1_A_GPIO->IDR & Rotary1_A_Pin)
#define Rotary1_A_GPIO_Definition				{ .name = "Rotary1_A", .pin_name = "D10", .gpio = Rotary1_A_GPIO, .pin = Rotary1_A_Pin }

// PD11: Rotary1_B (IN_UP)
#define Rotary1_B_GPIO							GPIOD
#define Rotary1_B_PinSource						GPIO_PinSource11
#define Rotary1_B_Pin							GPIO_Pin_11
#define Rotary1_B_SetHIGH()						Rotary1_B_GPIO->BSRRL = Rotary1_B_Pin
#define Rotary1_B_SetLOW()						Rotary1_B_GPIO->BSRRH = Rotary1_B_Pin
#define Rotary1_B_Toggle()						Rotary1_B_GPIO->ODR ^= Rotary1_B_Pin
#define Rotary1_B_Pulse()						do { Rotary1_B_SetHIGH(); delay_loopcnt(LOOPCOUNT_50NS); Rotary1_B_SetLOW(); } while (0)
#define Rotary1_B_InvPulse()					do { Rotary1_B_SetLOW(); delay_loopcnt(LOOPCOUNT_50NS); Rotary1_B_SetHIGH(); } while (0)
#define Rotary1_B_SetTo(value)					if (value) { Rotary1_B_SetHIGH(); } else { Rotary1_B_SetLOW(); }
#define Rotary1_B_GetValue()					(Rotary1_B_GPIO->IDR & Rotary1_B_Pin)
#define Rotary1_B_GPIO_Definition				{ .name = "Rotary1_B", .pin_name = "D11", .gpio = Rotary1_B_GPIO, .pin = Rotary1_B_Pin }

// PD12: LEDGreen (OUT)
#define LEDGreen_GPIO							GPIOD
#define LEDGreen_PinSource						GPIO_PinSource12
#define LEDGreen_Pin							GPIO_Pin_12
#define LEDGreen_SetHIGH()						LEDGreen_GPIO->BSRRL = LEDGreen_Pin
#define LEDGreen_SetLOW()						LEDGreen_GPIO->BSRRH = LEDGreen_Pin
#define LEDGreen_Toggle()						LEDGreen_GPIO->ODR ^= LEDGreen_Pin
#define LEDGreen_Pulse()						do { LEDGreen_SetHIGH(); delay_loopcnt(LOOPCOUNT_50NS); LEDGreen_SetLOW(); } while (0)
#define LEDGreen_InvPulse()						do { LEDGreen_SetLOW(); delay_loopcnt(LOOPCOUNT_50NS); LEDGreen_SetHIGH(); } while (0)
#define LEDGreen_SetTo(value)					if (value) { LEDGreen_SetHIGH(); } else { LEDGreen_SetLOW(); }
#define LEDGreen_GetValue()						(LEDGreen_GPIO->IDR & LEDGreen_Pin)
#define LEDGreen_GPIO_Definition				{ .name = "LEDGreen", .pin_name = "D12", .gpio = LEDGreen_GPIO, .pin = LEDGreen_Pin }

// PD13: LEDOrange (OUT)
#define LEDOrange_GPIO							GPIOD
#define LEDOrange_PinSource						GPIO_PinSource13
#define LEDOrange_Pin							GPIO_Pin_13
#define LEDOrange_SetHIGH()						LEDOrange_GPIO->BSRRL = LEDOrange_Pin
#define LEDOrange_SetLOW()						LEDOrange_GPIO->BSRRH = LEDOrange_Pin
#define LEDOrange_Toggle()						LEDOrange_GPIO->ODR ^= LEDOrange_Pin
#define LEDOrange_Pulse()						do { LEDOrange_SetHIGH(); delay_loopcnt(LOOPCOUNT_50NS); LEDOrange_SetLOW(); } while (0)
#define LEDOrange_InvPulse()					do { LEDOrange_SetLOW(); delay_loopcnt(LOOPCOUNT_50NS); LEDOrange_SetHIGH(); } while (0)
#define LEDOrange_SetTo(value)					if (value) { LEDOrange_SetHIGH(); } else { LEDOrange_SetLOW(); }
#define LEDOrange_GetValue()					(LEDOrange_GPIO->IDR & LEDOrange_Pin)
#define LEDOrange_GPIO_Definition				{ .name = "LEDOrange", .pin_name = "D13", .gpio = LEDOrange_GPIO, .pin = LEDOrange_Pin }

// PD14: LEDRed (OUT)
#define LEDRed_GPIO								GPIOD
#define LEDRed_PinSource						GPIO_PinSource14
#define LEDRed_Pin								GPIO_Pin_14
#define LEDRed_SetHIGH()						LEDRed_GPIO->BSRRL = LEDRed_Pin
#define LEDRed_SetLOW()							LEDRed_GPIO->BSRRH = LEDRed_Pin
#define LEDRed_Toggle()							LEDRed_GPIO->ODR ^= LEDRed_Pin
#define LEDRed_Pulse()							do { LEDRed_SetHIGH(); delay_loopcnt(LOOPCOUNT_50NS); LEDRed_SetLOW(); } while (0)
#define LEDRed_InvPulse()						do { LEDRed_SetLOW(); delay_loopcnt(LOOPCOUNT_50NS); LEDRed_SetHIGH(); } while (0)
#define LEDRed_SetTo(value)						if (value) { LEDRed_SetHIGH(); } else { LEDRed_SetLOW(); }
#define LEDRed_GetValue()						(LEDRed_GPIO->IDR & LEDRed_Pin)
#define LEDRed_GPIO_Definition					{ .name = "LEDRed", .pin_name = "D14", .gpio = LEDRed_GPIO, .pin = LEDRed_Pin }

// PD15: LEDBlue (OUT)
#define LEDBlue_GPIO							GPIOD
#define LEDBlue_PinSource						GPIO_PinSource15
#define LEDBlue_Pin								GPIO_Pin_15
#define LEDBlue_SetHIGH()						LEDBlue_GPIO->BSRRL = LEDBlue_Pin
#define LEDBlue_SetLOW()						LEDBlue_GPIO->BSRRH = LEDBlue_Pin
#define LEDBlue_Toggle()						LEDBlue_GPIO->ODR ^= LEDBlue_Pin
#define LEDBlue_Pulse()							do { LEDBlue_SetHIGH(); delay_loopcnt(LOOPCOUNT_50NS); LEDBlue_SetLOW(); } while (0)
#define LEDBlue_InvPulse()						do { LEDBlue_SetLOW(); delay_loopcnt(LOOPCOUNT_50NS); LEDBlue_SetHIGH(); } while (0)
#define LEDBlue_SetTo(value)					if (value) { LEDBlue_SetHIGH(); } else { LEDBlue_SetLOW(); }
#define LEDBlue_GetValue()						(LEDBlue_GPIO->IDR & LEDBlue_Pin)
#define LEDBlue_GPIO_Definition					{ .name = "LEDBlue", .pin_name = "D15", .gpio = LEDBlue_GPIO, .pin = LEDBlue_Pin }

// PE0: MEMS_INT1 (IN)
#define MEMS_INT1_GPIO							GPIOE
#define MEMS_INT1_PinSource						GPIO_PinSource0
#define MEMS_INT1_Pin							GPIO_Pin_0
#define MEMS_INT1_SetHIGH()						MEMS_INT1_GPIO->BSRRL = MEMS_INT1_Pin
#define MEMS_INT1_SetLOW()						MEMS_INT1_GPIO->BSRRH = MEMS_INT1_Pin
#define MEMS_INT1_Toggle()						MEMS_INT1_GPIO->ODR ^= MEMS_INT1_Pin
#define MEMS_INT1_Pulse()						do { MEMS_INT1_SetHIGH(); delay_loopcnt(LOOPCOUNT_50NS); MEMS_INT1_SetLOW(); } while (0)
#define MEMS_INT1_InvPulse()					do { MEMS_INT1_SetLOW(); delay_loopcnt(LOOPCOUNT_50NS); MEMS_INT1_SetHIGH(); } while (0)
#define MEMS_INT1_SetTo(value)					if (value) { MEMS_INT1_SetHIGH(); } else { MEMS_INT1_SetLOW(); }
#define MEMS_INT1_GetValue()					(MEMS_INT1_GPIO->IDR & MEMS_INT1_Pin)
#define MEMS_INT1_GPIO_Definition				{ .name = "MEMS_INT1", .pin_name = "E0", .gpio = MEMS_INT1_GPIO, .pin = MEMS_INT1_Pin }

// PE1: MEMS_INT2 (IN)
#define MEMS_INT2_GPIO							GPIOE
#define MEMS_INT2_PinSource						GPIO_PinSource1
#define MEMS_INT2_Pin							GPIO_Pin_1
#define MEMS_INT2_SetHIGH()						MEMS_INT2_GPIO->BSRRL = MEMS_INT2_Pin
#define MEMS_INT2_SetLOW()						MEMS_INT2_GPIO->BSRRH = MEMS_INT2_Pin
#define MEMS_INT2_Toggle()						MEMS_INT2_GPIO->ODR ^= MEMS_INT2_Pin
#define MEMS_INT2_Pulse()						do { MEMS_INT2_SetHIGH(); delay_loopcnt(LOOPCOUNT_50NS); MEMS_INT2_SetLOW(); } while (0)
#define MEMS_INT2_InvPulse()					do { MEMS_INT2_SetLOW(); delay_loopcnt(LOOPCOUNT_50NS); MEMS_INT2_SetHIGH(); } while (0)
#define MEMS_INT2_SetTo(value)					if (value) { MEMS_INT2_SetHIGH(); } else { MEMS_INT2_SetLOW(); }
#define MEMS_INT2_GetValue()					(MEMS_INT2_GPIO->IDR & MEMS_INT2_Pin)
#define MEMS_INT2_GPIO_Definition				{ .name = "MEMS_INT2", .pin_name = "E1", .gpio = MEMS_INT2_GPIO, .pin = MEMS_INT2_Pin }

// PE3: MEMS_I2C_SPI (OUT)
#define MEMS_I2C_SPI_GPIO						GPIOE
#define MEMS_I2C_SPI_PinSource					GPIO_PinSource3
#define MEMS_I2C_SPI_Pin						GPIO_Pin_3
#define MEMS_I2C_SPI_SetHIGH()					MEMS_I2C_SPI_GPIO->BSRRL = MEMS_I2C_SPI_Pin
#define MEMS_I2C_SPI_SetLOW()					MEMS_I2C_SPI_GPIO->BSRRH = MEMS_I2C_SPI_Pin
#define MEMS_I2C_SPI_Toggle()					MEMS_I2C_SPI_GPIO->ODR ^= MEMS_I2C_SPI_Pin
#define MEMS_I2C_SPI_Pulse()					do { MEMS_I2C_SPI_SetHIGH(); delay_loopcnt(LOOPCOUNT_50NS); MEMS_I2C_SPI_SetLOW(); } while (0)
#define MEMS_I2C_SPI_InvPulse()					do { MEMS_I2C_SPI_SetLOW(); delay_loopcnt(LOOPCOUNT_50NS); MEMS_I2C_SPI_SetHIGH(); } while (0)
#define MEMS_I2C_SPI_SetTo(value)				if (value) { MEMS_I2C_SPI_SetHIGH(); } else { MEMS_I2C_SPI_SetLOW(); }
#define MEMS_I2C_SPI_GetValue()					(MEMS_I2C_SPI_GPIO->IDR & MEMS_I2C_SPI_Pin)
#define MEMS_I2C_SPI_GPIO_Definition			{ .name = "MEMS_I2C_SPI", .pin_name = "E3", .gpio = MEMS_I2C_SPI_GPIO, .pin = MEMS_I2C_SPI_Pin }

// Display_nSS: Group of 12 pins (Display4_nSS, Display12_nSS, Display9_nSS, Display8_nSS, Display5_nSS, Display3_nSS, Display11_nSS, Display1_nSS, Display6_nSS, Display10_nSS, Display7_nSS, Display2_nSS)

#endif
