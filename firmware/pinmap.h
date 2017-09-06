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
#define UserButton_set_HIGH()					UserButton_GPIO->BSRRL = UserButton_Pin
#define UserButton_set_LOW()					UserButton_GPIO->BSRRH = UserButton_Pin
#define UserButton_set(value)					if (value) { UserButton_set_HIGH(); } else { UserButton_set_LOW(); }
#define UserButton_toggle()						UserButton_GPIO->ODR ^= UserButton_Pin
#define UserButton_pulse()						do { UserButton_set_HIGH(); delay_loopcnt(LOOPCOUNT_50NS); UserButton_set_LOW(); } while (0)
#define UserButton_npulse()						do { UserButton_set_LOW(); delay_loopcnt(LOOPCOUNT_50NS); UserButton_set_HIGH(); } while (0)
#define UserButton_get()						(UserButton_GPIO->IDR & UserButton_Pin)
#define UserButton_GPIO_Definition				{ .name = "UserButton", .pin_name = "A0", .gpio = UserButton_GPIO, .pin = UserButton_Pin }

// PA4: Audio_WS (?)
#define Audio_WS_GPIO							GPIOA
#define Audio_WS_PinSource						GPIO_PinSource4
#define Audio_WS_Pin							GPIO_Pin_4
#define Audio_WS_set_HIGH()						Audio_WS_GPIO->BSRRL = Audio_WS_Pin
#define Audio_WS_set_LOW()						Audio_WS_GPIO->BSRRH = Audio_WS_Pin
#define Audio_WS_set(value)						if (value) { Audio_WS_set_HIGH(); } else { Audio_WS_set_LOW(); }
#define Audio_WS_toggle()						Audio_WS_GPIO->ODR ^= Audio_WS_Pin
#define Audio_WS_pulse()						do { Audio_WS_set_HIGH(); delay_loopcnt(LOOPCOUNT_50NS); Audio_WS_set_LOW(); } while (0)
#define Audio_WS_npulse()						do { Audio_WS_set_LOW(); delay_loopcnt(LOOPCOUNT_50NS); Audio_WS_set_HIGH(); } while (0)
#define Audio_WS_get()							(Audio_WS_GPIO->IDR & Audio_WS_Pin)
#define Audio_WS_GPIO_Definition				{ .name = "Audio_WS", .pin_name = "A4", .gpio = Audio_WS_GPIO, .pin = Audio_WS_Pin }

// PA5: MEMS_SCK (OUT)
#define MEMS_SCK_GPIO							GPIOA
#define MEMS_SCK_PinSource						GPIO_PinSource5
#define MEMS_SCK_Pin							GPIO_Pin_5
#define MEMS_SCK_set_HIGH()						MEMS_SCK_GPIO->BSRRL = MEMS_SCK_Pin
#define MEMS_SCK_set_LOW()						MEMS_SCK_GPIO->BSRRH = MEMS_SCK_Pin
#define MEMS_SCK_set(value)						if (value) { MEMS_SCK_set_HIGH(); } else { MEMS_SCK_set_LOW(); }
#define MEMS_SCK_toggle()						MEMS_SCK_GPIO->ODR ^= MEMS_SCK_Pin
#define MEMS_SCK_pulse()						do { MEMS_SCK_set_HIGH(); delay_loopcnt(LOOPCOUNT_50NS); MEMS_SCK_set_LOW(); } while (0)
#define MEMS_SCK_npulse()						do { MEMS_SCK_set_LOW(); delay_loopcnt(LOOPCOUNT_50NS); MEMS_SCK_set_HIGH(); } while (0)
#define MEMS_SCK_get()							(MEMS_SCK_GPIO->IDR & MEMS_SCK_Pin)
#define MEMS_SCK_GPIO_Definition				{ .name = "MEMS_SCK", .pin_name = "A5", .gpio = MEMS_SCK_GPIO, .pin = MEMS_SCK_Pin }

// PA6: MEMS_MISO (IN)
#define MEMS_MISO_GPIO							GPIOA
#define MEMS_MISO_PinSource						GPIO_PinSource6
#define MEMS_MISO_Pin							GPIO_Pin_6
#define MEMS_MISO_set_HIGH()					MEMS_MISO_GPIO->BSRRL = MEMS_MISO_Pin
#define MEMS_MISO_set_LOW()						MEMS_MISO_GPIO->BSRRH = MEMS_MISO_Pin
#define MEMS_MISO_set(value)					if (value) { MEMS_MISO_set_HIGH(); } else { MEMS_MISO_set_LOW(); }
#define MEMS_MISO_toggle()						MEMS_MISO_GPIO->ODR ^= MEMS_MISO_Pin
#define MEMS_MISO_pulse()						do { MEMS_MISO_set_HIGH(); delay_loopcnt(LOOPCOUNT_50NS); MEMS_MISO_set_LOW(); } while (0)
#define MEMS_MISO_npulse()						do { MEMS_MISO_set_LOW(); delay_loopcnt(LOOPCOUNT_50NS); MEMS_MISO_set_HIGH(); } while (0)
#define MEMS_MISO_get()							(MEMS_MISO_GPIO->IDR & MEMS_MISO_Pin)
#define MEMS_MISO_GPIO_Definition				{ .name = "MEMS_MISO", .pin_name = "A6", .gpio = MEMS_MISO_GPIO, .pin = MEMS_MISO_Pin }

// PA7: MEMS_MOSI (OUT)
#define MEMS_MOSI_GPIO							GPIOA
#define MEMS_MOSI_PinSource						GPIO_PinSource7
#define MEMS_MOSI_Pin							GPIO_Pin_7
#define MEMS_MOSI_set_HIGH()					MEMS_MOSI_GPIO->BSRRL = MEMS_MOSI_Pin
#define MEMS_MOSI_set_LOW()						MEMS_MOSI_GPIO->BSRRH = MEMS_MOSI_Pin
#define MEMS_MOSI_set(value)					if (value) { MEMS_MOSI_set_HIGH(); } else { MEMS_MOSI_set_LOW(); }
#define MEMS_MOSI_toggle()						MEMS_MOSI_GPIO->ODR ^= MEMS_MOSI_Pin
#define MEMS_MOSI_pulse()						do { MEMS_MOSI_set_HIGH(); delay_loopcnt(LOOPCOUNT_50NS); MEMS_MOSI_set_LOW(); } while (0)
#define MEMS_MOSI_npulse()						do { MEMS_MOSI_set_LOW(); delay_loopcnt(LOOPCOUNT_50NS); MEMS_MOSI_set_HIGH(); } while (0)
#define MEMS_MOSI_get()							(MEMS_MOSI_GPIO->IDR & MEMS_MOSI_Pin)
#define MEMS_MOSI_GPIO_Definition				{ .name = "MEMS_MOSI", .pin_name = "A7", .gpio = MEMS_MOSI_GPIO, .pin = MEMS_MOSI_Pin }

// PA9: USB_OTG_VBUS (IN)
#define USB_OTG_VBUS_GPIO						GPIOA
#define USB_OTG_VBUS_PinSource					GPIO_PinSource9
#define USB_OTG_VBUS_Pin						GPIO_Pin_9
#define USB_OTG_VBUS_set_HIGH()					USB_OTG_VBUS_GPIO->BSRRL = USB_OTG_VBUS_Pin
#define USB_OTG_VBUS_set_LOW()					USB_OTG_VBUS_GPIO->BSRRH = USB_OTG_VBUS_Pin
#define USB_OTG_VBUS_set(value)					if (value) { USB_OTG_VBUS_set_HIGH(); } else { USB_OTG_VBUS_set_LOW(); }
#define USB_OTG_VBUS_toggle()					USB_OTG_VBUS_GPIO->ODR ^= USB_OTG_VBUS_Pin
#define USB_OTG_VBUS_pulse()					do { USB_OTG_VBUS_set_HIGH(); delay_loopcnt(LOOPCOUNT_50NS); USB_OTG_VBUS_set_LOW(); } while (0)
#define USB_OTG_VBUS_npulse()					do { USB_OTG_VBUS_set_LOW(); delay_loopcnt(LOOPCOUNT_50NS); USB_OTG_VBUS_set_HIGH(); } while (0)
#define USB_OTG_VBUS_get()						(USB_OTG_VBUS_GPIO->IDR & USB_OTG_VBUS_Pin)
#define USB_OTG_VBUS_GPIO_Definition			{ .name = "USB_OTG_VBUS", .pin_name = "A9", .gpio = USB_OTG_VBUS_GPIO, .pin = USB_OTG_VBUS_Pin }

// PA10: USB_OTG_ID (IN)
#define USB_OTG_ID_GPIO							GPIOA
#define USB_OTG_ID_PinSource					GPIO_PinSource10
#define USB_OTG_ID_Pin							GPIO_Pin_10
#define USB_OTG_ID_set_HIGH()					USB_OTG_ID_GPIO->BSRRL = USB_OTG_ID_Pin
#define USB_OTG_ID_set_LOW()					USB_OTG_ID_GPIO->BSRRH = USB_OTG_ID_Pin
#define USB_OTG_ID_set(value)					if (value) { USB_OTG_ID_set_HIGH(); } else { USB_OTG_ID_set_LOW(); }
#define USB_OTG_ID_toggle()						USB_OTG_ID_GPIO->ODR ^= USB_OTG_ID_Pin
#define USB_OTG_ID_pulse()						do { USB_OTG_ID_set_HIGH(); delay_loopcnt(LOOPCOUNT_50NS); USB_OTG_ID_set_LOW(); } while (0)
#define USB_OTG_ID_npulse()						do { USB_OTG_ID_set_LOW(); delay_loopcnt(LOOPCOUNT_50NS); USB_OTG_ID_set_HIGH(); } while (0)
#define USB_OTG_ID_get()						(USB_OTG_ID_GPIO->IDR & USB_OTG_ID_Pin)
#define USB_OTG_ID_GPIO_Definition				{ .name = "USB_OTG_ID", .pin_name = "A10", .gpio = USB_OTG_ID_GPIO, .pin = USB_OTG_ID_Pin }

// PA11: USB_OTG_M (IO)
#define USB_OTG_M_GPIO							GPIOA
#define USB_OTG_M_PinSource						GPIO_PinSource11
#define USB_OTG_M_Pin							GPIO_Pin_11
#define USB_OTG_M_set_HIGH()					USB_OTG_M_GPIO->BSRRL = USB_OTG_M_Pin
#define USB_OTG_M_set_LOW()						USB_OTG_M_GPIO->BSRRH = USB_OTG_M_Pin
#define USB_OTG_M_set(value)					if (value) { USB_OTG_M_set_HIGH(); } else { USB_OTG_M_set_LOW(); }
#define USB_OTG_M_toggle()						USB_OTG_M_GPIO->ODR ^= USB_OTG_M_Pin
#define USB_OTG_M_pulse()						do { USB_OTG_M_set_HIGH(); delay_loopcnt(LOOPCOUNT_50NS); USB_OTG_M_set_LOW(); } while (0)
#define USB_OTG_M_npulse()						do { USB_OTG_M_set_LOW(); delay_loopcnt(LOOPCOUNT_50NS); USB_OTG_M_set_HIGH(); } while (0)
#define USB_OTG_M_get()							(USB_OTG_M_GPIO->IDR & USB_OTG_M_Pin)
#define USB_OTG_M_GPIO_Definition				{ .name = "USB_OTG_M", .pin_name = "A11", .gpio = USB_OTG_M_GPIO, .pin = USB_OTG_M_Pin }

// PA12: USB_OTG_P (IO)
#define USB_OTG_P_GPIO							GPIOA
#define USB_OTG_P_PinSource						GPIO_PinSource12
#define USB_OTG_P_Pin							GPIO_Pin_12
#define USB_OTG_P_set_HIGH()					USB_OTG_P_GPIO->BSRRL = USB_OTG_P_Pin
#define USB_OTG_P_set_LOW()						USB_OTG_P_GPIO->BSRRH = USB_OTG_P_Pin
#define USB_OTG_P_set(value)					if (value) { USB_OTG_P_set_HIGH(); } else { USB_OTG_P_set_LOW(); }
#define USB_OTG_P_toggle()						USB_OTG_P_GPIO->ODR ^= USB_OTG_P_Pin
#define USB_OTG_P_pulse()						do { USB_OTG_P_set_HIGH(); delay_loopcnt(LOOPCOUNT_50NS); USB_OTG_P_set_LOW(); } while (0)
#define USB_OTG_P_npulse()						do { USB_OTG_P_set_LOW(); delay_loopcnt(LOOPCOUNT_50NS); USB_OTG_P_set_HIGH(); } while (0)
#define USB_OTG_P_get()							(USB_OTG_P_GPIO->IDR & USB_OTG_P_Pin)
#define USB_OTG_P_GPIO_Definition				{ .name = "USB_OTG_P", .pin_name = "A12", .gpio = USB_OTG_P_GPIO, .pin = USB_OTG_P_Pin }

// PB0: Display1_nSS (OUT)
#define Display1_nSS_GPIO						GPIOB
#define Display1_nSS_PinSource					GPIO_PinSource0
#define Display1_nSS_Pin						GPIO_Pin_0
#define Display1_nSS_set_HIGH()					Display1_nSS_GPIO->BSRRL = Display1_nSS_Pin
#define Display1_nSS_set_LOW()					Display1_nSS_GPIO->BSRRH = Display1_nSS_Pin
#define Display1_nSS_set(value)					if (value) { Display1_nSS_set_HIGH(); } else { Display1_nSS_set_LOW(); }
#define Display1_nSS_toggle()					Display1_nSS_GPIO->ODR ^= Display1_nSS_Pin
#define Display1_nSS_pulse()					do { Display1_nSS_set_HIGH(); delay_loopcnt(LOOPCOUNT_50NS); Display1_nSS_set_LOW(); } while (0)
#define Display1_nSS_npulse()					do { Display1_nSS_set_LOW(); delay_loopcnt(LOOPCOUNT_50NS); Display1_nSS_set_HIGH(); } while (0)
#define Display1_nSS_get()						(Display1_nSS_GPIO->IDR & Display1_nSS_Pin)
#define Display1_nSS_GPIO_Definition			{ .name = "Display1_nSS", .pin_name = "B0", .gpio = Display1_nSS_GPIO, .pin = Display1_nSS_Pin }

// PB1: Display2_nSS (OUT)
#define Display2_nSS_GPIO						GPIOB
#define Display2_nSS_PinSource					GPIO_PinSource1
#define Display2_nSS_Pin						GPIO_Pin_1
#define Display2_nSS_set_HIGH()					Display2_nSS_GPIO->BSRRL = Display2_nSS_Pin
#define Display2_nSS_set_LOW()					Display2_nSS_GPIO->BSRRH = Display2_nSS_Pin
#define Display2_nSS_set(value)					if (value) { Display2_nSS_set_HIGH(); } else { Display2_nSS_set_LOW(); }
#define Display2_nSS_toggle()					Display2_nSS_GPIO->ODR ^= Display2_nSS_Pin
#define Display2_nSS_pulse()					do { Display2_nSS_set_HIGH(); delay_loopcnt(LOOPCOUNT_50NS); Display2_nSS_set_LOW(); } while (0)
#define Display2_nSS_npulse()					do { Display2_nSS_set_LOW(); delay_loopcnt(LOOPCOUNT_50NS); Display2_nSS_set_HIGH(); } while (0)
#define Display2_nSS_get()						(Display2_nSS_GPIO->IDR & Display2_nSS_Pin)
#define Display2_nSS_GPIO_Definition			{ .name = "Display2_nSS", .pin_name = "B1", .gpio = Display2_nSS_GPIO, .pin = Display2_nSS_Pin }

// PB2: Display3_nSS (OUT)
#define Display3_nSS_GPIO						GPIOB
#define Display3_nSS_PinSource					GPIO_PinSource2
#define Display3_nSS_Pin						GPIO_Pin_2
#define Display3_nSS_set_HIGH()					Display3_nSS_GPIO->BSRRL = Display3_nSS_Pin
#define Display3_nSS_set_LOW()					Display3_nSS_GPIO->BSRRH = Display3_nSS_Pin
#define Display3_nSS_set(value)					if (value) { Display3_nSS_set_HIGH(); } else { Display3_nSS_set_LOW(); }
#define Display3_nSS_toggle()					Display3_nSS_GPIO->ODR ^= Display3_nSS_Pin
#define Display3_nSS_pulse()					do { Display3_nSS_set_HIGH(); delay_loopcnt(LOOPCOUNT_50NS); Display3_nSS_set_LOW(); } while (0)
#define Display3_nSS_npulse()					do { Display3_nSS_set_LOW(); delay_loopcnt(LOOPCOUNT_50NS); Display3_nSS_set_HIGH(); } while (0)
#define Display3_nSS_get()						(Display3_nSS_GPIO->IDR & Display3_nSS_Pin)
#define Display3_nSS_GPIO_Definition			{ .name = "Display3_nSS", .pin_name = "B2", .gpio = Display3_nSS_GPIO, .pin = Display3_nSS_Pin }

// PB3: Display4_nSS (OUT)
#define Display4_nSS_GPIO						GPIOB
#define Display4_nSS_PinSource					GPIO_PinSource3
#define Display4_nSS_Pin						GPIO_Pin_3
#define Display4_nSS_set_HIGH()					Display4_nSS_GPIO->BSRRL = Display4_nSS_Pin
#define Display4_nSS_set_LOW()					Display4_nSS_GPIO->BSRRH = Display4_nSS_Pin
#define Display4_nSS_set(value)					if (value) { Display4_nSS_set_HIGH(); } else { Display4_nSS_set_LOW(); }
#define Display4_nSS_toggle()					Display4_nSS_GPIO->ODR ^= Display4_nSS_Pin
#define Display4_nSS_pulse()					do { Display4_nSS_set_HIGH(); delay_loopcnt(LOOPCOUNT_50NS); Display4_nSS_set_LOW(); } while (0)
#define Display4_nSS_npulse()					do { Display4_nSS_set_LOW(); delay_loopcnt(LOOPCOUNT_50NS); Display4_nSS_set_HIGH(); } while (0)
#define Display4_nSS_get()						(Display4_nSS_GPIO->IDR & Display4_nSS_Pin)
#define Display4_nSS_GPIO_Definition			{ .name = "Display4_nSS", .pin_name = "B3", .gpio = Display4_nSS_GPIO, .pin = Display4_nSS_Pin }

// PB4: Display5_nSS (OUT)
#define Display5_nSS_GPIO						GPIOB
#define Display5_nSS_PinSource					GPIO_PinSource4
#define Display5_nSS_Pin						GPIO_Pin_4
#define Display5_nSS_set_HIGH()					Display5_nSS_GPIO->BSRRL = Display5_nSS_Pin
#define Display5_nSS_set_LOW()					Display5_nSS_GPIO->BSRRH = Display5_nSS_Pin
#define Display5_nSS_set(value)					if (value) { Display5_nSS_set_HIGH(); } else { Display5_nSS_set_LOW(); }
#define Display5_nSS_toggle()					Display5_nSS_GPIO->ODR ^= Display5_nSS_Pin
#define Display5_nSS_pulse()					do { Display5_nSS_set_HIGH(); delay_loopcnt(LOOPCOUNT_50NS); Display5_nSS_set_LOW(); } while (0)
#define Display5_nSS_npulse()					do { Display5_nSS_set_LOW(); delay_loopcnt(LOOPCOUNT_50NS); Display5_nSS_set_HIGH(); } while (0)
#define Display5_nSS_get()						(Display5_nSS_GPIO->IDR & Display5_nSS_Pin)
#define Display5_nSS_GPIO_Definition			{ .name = "Display5_nSS", .pin_name = "B4", .gpio = Display5_nSS_GPIO, .pin = Display5_nSS_Pin }

// PB5: Display6_nSS (OUT)
#define Display6_nSS_GPIO						GPIOB
#define Display6_nSS_PinSource					GPIO_PinSource5
#define Display6_nSS_Pin						GPIO_Pin_5
#define Display6_nSS_set_HIGH()					Display6_nSS_GPIO->BSRRL = Display6_nSS_Pin
#define Display6_nSS_set_LOW()					Display6_nSS_GPIO->BSRRH = Display6_nSS_Pin
#define Display6_nSS_set(value)					if (value) { Display6_nSS_set_HIGH(); } else { Display6_nSS_set_LOW(); }
#define Display6_nSS_toggle()					Display6_nSS_GPIO->ODR ^= Display6_nSS_Pin
#define Display6_nSS_pulse()					do { Display6_nSS_set_HIGH(); delay_loopcnt(LOOPCOUNT_50NS); Display6_nSS_set_LOW(); } while (0)
#define Display6_nSS_npulse()					do { Display6_nSS_set_LOW(); delay_loopcnt(LOOPCOUNT_50NS); Display6_nSS_set_HIGH(); } while (0)
#define Display6_nSS_get()						(Display6_nSS_GPIO->IDR & Display6_nSS_Pin)
#define Display6_nSS_GPIO_Definition			{ .name = "Display6_nSS", .pin_name = "B5", .gpio = Display6_nSS_GPIO, .pin = Display6_nSS_Pin }

// PB6: Audio_SCL (IO)
#define Audio_SCL_GPIO							GPIOB
#define Audio_SCL_PinSource						GPIO_PinSource6
#define Audio_SCL_Pin							GPIO_Pin_6
#define Audio_SCL_set_HIGH()					Audio_SCL_GPIO->BSRRL = Audio_SCL_Pin
#define Audio_SCL_set_LOW()						Audio_SCL_GPIO->BSRRH = Audio_SCL_Pin
#define Audio_SCL_set(value)					if (value) { Audio_SCL_set_HIGH(); } else { Audio_SCL_set_LOW(); }
#define Audio_SCL_toggle()						Audio_SCL_GPIO->ODR ^= Audio_SCL_Pin
#define Audio_SCL_pulse()						do { Audio_SCL_set_HIGH(); delay_loopcnt(LOOPCOUNT_50NS); Audio_SCL_set_LOW(); } while (0)
#define Audio_SCL_npulse()						do { Audio_SCL_set_LOW(); delay_loopcnt(LOOPCOUNT_50NS); Audio_SCL_set_HIGH(); } while (0)
#define Audio_SCL_get()							(Audio_SCL_GPIO->IDR & Audio_SCL_Pin)
#define Audio_SCL_GPIO_Definition				{ .name = "Audio_SCL", .pin_name = "B6", .gpio = Audio_SCL_GPIO, .pin = Audio_SCL_Pin }
// PB6: EEPROM_SCL (IO)
#define EEPROM_SCL_GPIO							GPIOB
#define EEPROM_SCL_PinSource					GPIO_PinSource6
#define EEPROM_SCL_Pin							GPIO_Pin_6
#define EEPROM_SCL_set_HIGH()					EEPROM_SCL_GPIO->BSRRL = EEPROM_SCL_Pin
#define EEPROM_SCL_set_LOW()					EEPROM_SCL_GPIO->BSRRH = EEPROM_SCL_Pin
#define EEPROM_SCL_set(value)					if (value) { EEPROM_SCL_set_HIGH(); } else { EEPROM_SCL_set_LOW(); }
#define EEPROM_SCL_toggle()						EEPROM_SCL_GPIO->ODR ^= EEPROM_SCL_Pin
#define EEPROM_SCL_pulse()						do { EEPROM_SCL_set_HIGH(); delay_loopcnt(LOOPCOUNT_50NS); EEPROM_SCL_set_LOW(); } while (0)
#define EEPROM_SCL_npulse()						do { EEPROM_SCL_set_LOW(); delay_loopcnt(LOOPCOUNT_50NS); EEPROM_SCL_set_HIGH(); } while (0)
#define EEPROM_SCL_get()						(EEPROM_SCL_GPIO->IDR & EEPROM_SCL_Pin)
#define EEPROM_SCL_GPIO_Definition				{ .name = "EEPROM_SCL", .pin_name = "B6", .gpio = EEPROM_SCL_GPIO, .pin = EEPROM_SCL_Pin }

// PB7: Display7_nSS (OUT)
#define Display7_nSS_GPIO						GPIOB
#define Display7_nSS_PinSource					GPIO_PinSource7
#define Display7_nSS_Pin						GPIO_Pin_7
#define Display7_nSS_set_HIGH()					Display7_nSS_GPIO->BSRRL = Display7_nSS_Pin
#define Display7_nSS_set_LOW()					Display7_nSS_GPIO->BSRRH = Display7_nSS_Pin
#define Display7_nSS_set(value)					if (value) { Display7_nSS_set_HIGH(); } else { Display7_nSS_set_LOW(); }
#define Display7_nSS_toggle()					Display7_nSS_GPIO->ODR ^= Display7_nSS_Pin
#define Display7_nSS_pulse()					do { Display7_nSS_set_HIGH(); delay_loopcnt(LOOPCOUNT_50NS); Display7_nSS_set_LOW(); } while (0)
#define Display7_nSS_npulse()					do { Display7_nSS_set_LOW(); delay_loopcnt(LOOPCOUNT_50NS); Display7_nSS_set_HIGH(); } while (0)
#define Display7_nSS_get()						(Display7_nSS_GPIO->IDR & Display7_nSS_Pin)
#define Display7_nSS_GPIO_Definition			{ .name = "Display7_nSS", .pin_name = "B7", .gpio = Display7_nSS_GPIO, .pin = Display7_nSS_Pin }

// PB9: Audio_SDA (IO)
#define Audio_SDA_GPIO							GPIOB
#define Audio_SDA_PinSource						GPIO_PinSource9
#define Audio_SDA_Pin							GPIO_Pin_9
#define Audio_SDA_set_HIGH()					Audio_SDA_GPIO->BSRRL = Audio_SDA_Pin
#define Audio_SDA_set_LOW()						Audio_SDA_GPIO->BSRRH = Audio_SDA_Pin
#define Audio_SDA_set(value)					if (value) { Audio_SDA_set_HIGH(); } else { Audio_SDA_set_LOW(); }
#define Audio_SDA_toggle()						Audio_SDA_GPIO->ODR ^= Audio_SDA_Pin
#define Audio_SDA_pulse()						do { Audio_SDA_set_HIGH(); delay_loopcnt(LOOPCOUNT_50NS); Audio_SDA_set_LOW(); } while (0)
#define Audio_SDA_npulse()						do { Audio_SDA_set_LOW(); delay_loopcnt(LOOPCOUNT_50NS); Audio_SDA_set_HIGH(); } while (0)
#define Audio_SDA_get()							(Audio_SDA_GPIO->IDR & Audio_SDA_Pin)
#define Audio_SDA_GPIO_Definition				{ .name = "Audio_SDA", .pin_name = "B9", .gpio = Audio_SDA_GPIO, .pin = Audio_SDA_Pin }
// PB9: EEPROM_SDA (IO)
#define EEPROM_SDA_GPIO							GPIOB
#define EEPROM_SDA_PinSource					GPIO_PinSource9
#define EEPROM_SDA_Pin							GPIO_Pin_9
#define EEPROM_SDA_set_HIGH()					EEPROM_SDA_GPIO->BSRRL = EEPROM_SDA_Pin
#define EEPROM_SDA_set_LOW()					EEPROM_SDA_GPIO->BSRRH = EEPROM_SDA_Pin
#define EEPROM_SDA_set(value)					if (value) { EEPROM_SDA_set_HIGH(); } else { EEPROM_SDA_set_LOW(); }
#define EEPROM_SDA_toggle()						EEPROM_SDA_GPIO->ODR ^= EEPROM_SDA_Pin
#define EEPROM_SDA_pulse()						do { EEPROM_SDA_set_HIGH(); delay_loopcnt(LOOPCOUNT_50NS); EEPROM_SDA_set_LOW(); } while (0)
#define EEPROM_SDA_npulse()						do { EEPROM_SDA_set_LOW(); delay_loopcnt(LOOPCOUNT_50NS); EEPROM_SDA_set_HIGH(); } while (0)
#define EEPROM_SDA_get()						(EEPROM_SDA_GPIO->IDR & EEPROM_SDA_Pin)
#define EEPROM_SDA_GPIO_Definition				{ .name = "EEPROM_SDA", .pin_name = "B9", .gpio = EEPROM_SDA_GPIO, .pin = EEPROM_SDA_Pin }

// PB10: Audio_CLK_IN (OUT)
#define Audio_CLK_IN_GPIO						GPIOB
#define Audio_CLK_IN_PinSource					GPIO_PinSource10
#define Audio_CLK_IN_Pin						GPIO_Pin_10
#define Audio_CLK_IN_set_HIGH()					Audio_CLK_IN_GPIO->BSRRL = Audio_CLK_IN_Pin
#define Audio_CLK_IN_set_LOW()					Audio_CLK_IN_GPIO->BSRRH = Audio_CLK_IN_Pin
#define Audio_CLK_IN_set(value)					if (value) { Audio_CLK_IN_set_HIGH(); } else { Audio_CLK_IN_set_LOW(); }
#define Audio_CLK_IN_toggle()					Audio_CLK_IN_GPIO->ODR ^= Audio_CLK_IN_Pin
#define Audio_CLK_IN_pulse()					do { Audio_CLK_IN_set_HIGH(); delay_loopcnt(LOOPCOUNT_50NS); Audio_CLK_IN_set_LOW(); } while (0)
#define Audio_CLK_IN_npulse()					do { Audio_CLK_IN_set_LOW(); delay_loopcnt(LOOPCOUNT_50NS); Audio_CLK_IN_set_HIGH(); } while (0)
#define Audio_CLK_IN_get()						(Audio_CLK_IN_GPIO->IDR & Audio_CLK_IN_Pin)
#define Audio_CLK_IN_GPIO_Definition			{ .name = "Audio_CLK_IN", .pin_name = "B10", .gpio = Audio_CLK_IN_GPIO, .pin = Audio_CLK_IN_Pin }

// PB13: Display_SCK (OUT)
#define Display_SCK_GPIO						GPIOB
#define Display_SCK_PinSource					GPIO_PinSource13
#define Display_SCK_Pin							GPIO_Pin_13
#define Display_SCK_set_HIGH()					Display_SCK_GPIO->BSRRL = Display_SCK_Pin
#define Display_SCK_set_LOW()					Display_SCK_GPIO->BSRRH = Display_SCK_Pin
#define Display_SCK_set(value)					if (value) { Display_SCK_set_HIGH(); } else { Display_SCK_set_LOW(); }
#define Display_SCK_toggle()					Display_SCK_GPIO->ODR ^= Display_SCK_Pin
#define Display_SCK_pulse()						do { Display_SCK_set_HIGH(); delay_loopcnt(LOOPCOUNT_50NS); Display_SCK_set_LOW(); } while (0)
#define Display_SCK_npulse()					do { Display_SCK_set_LOW(); delay_loopcnt(LOOPCOUNT_50NS); Display_SCK_set_HIGH(); } while (0)
#define Display_SCK_get()						(Display_SCK_GPIO->IDR & Display_SCK_Pin)
#define Display_SCK_GPIO_Definition				{ .name = "Display_SCK", .pin_name = "B13", .gpio = Display_SCK_GPIO, .pin = Display_SCK_Pin }

// PB15: Display_MOSI (OUT)
#define Display_MOSI_GPIO						GPIOB
#define Display_MOSI_PinSource					GPIO_PinSource15
#define Display_MOSI_Pin						GPIO_Pin_15
#define Display_MOSI_set_HIGH()					Display_MOSI_GPIO->BSRRL = Display_MOSI_Pin
#define Display_MOSI_set_LOW()					Display_MOSI_GPIO->BSRRH = Display_MOSI_Pin
#define Display_MOSI_set(value)					if (value) { Display_MOSI_set_HIGH(); } else { Display_MOSI_set_LOW(); }
#define Display_MOSI_toggle()					Display_MOSI_GPIO->ODR ^= Display_MOSI_Pin
#define Display_MOSI_pulse()					do { Display_MOSI_set_HIGH(); delay_loopcnt(LOOPCOUNT_50NS); Display_MOSI_set_LOW(); } while (0)
#define Display_MOSI_npulse()					do { Display_MOSI_set_LOW(); delay_loopcnt(LOOPCOUNT_50NS); Display_MOSI_set_HIGH(); } while (0)
#define Display_MOSI_get()						(Display_MOSI_GPIO->IDR & Display_MOSI_Pin)
#define Display_MOSI_GPIO_Definition			{ .name = "Display_MOSI", .pin_name = "B15", .gpio = Display_MOSI_GPIO, .pin = Display_MOSI_Pin }

// PC0: USB_PowerSwitchOn (OUT)
#define USB_PowerSwitchOn_GPIO					GPIOC
#define USB_PowerSwitchOn_PinSource				GPIO_PinSource0
#define USB_PowerSwitchOn_Pin					GPIO_Pin_0
#define USB_PowerSwitchOn_set_HIGH()			USB_PowerSwitchOn_GPIO->BSRRL = USB_PowerSwitchOn_Pin
#define USB_PowerSwitchOn_set_LOW()				USB_PowerSwitchOn_GPIO->BSRRH = USB_PowerSwitchOn_Pin
#define USB_PowerSwitchOn_set(value)			if (value) { USB_PowerSwitchOn_set_HIGH(); } else { USB_PowerSwitchOn_set_LOW(); }
#define USB_PowerSwitchOn_toggle()				USB_PowerSwitchOn_GPIO->ODR ^= USB_PowerSwitchOn_Pin
#define USB_PowerSwitchOn_pulse()				do { USB_PowerSwitchOn_set_HIGH(); delay_loopcnt(LOOPCOUNT_50NS); USB_PowerSwitchOn_set_LOW(); } while (0)
#define USB_PowerSwitchOn_npulse()				do { USB_PowerSwitchOn_set_LOW(); delay_loopcnt(LOOPCOUNT_50NS); USB_PowerSwitchOn_set_HIGH(); } while (0)
#define USB_PowerSwitchOn_get()					(USB_PowerSwitchOn_GPIO->IDR & USB_PowerSwitchOn_Pin)
#define USB_PowerSwitchOn_GPIO_Definition		{ .name = "USB_PowerSwitchOn", .pin_name = "C0", .gpio = USB_PowerSwitchOn_GPIO, .pin = USB_PowerSwitchOn_Pin }
// PC0: IOMux_In_PE (OUT)
#define IOMux_In_PE_GPIO						GPIOC
#define IOMux_In_PE_PinSource					GPIO_PinSource0
#define IOMux_In_PE_Pin							GPIO_Pin_0
#define IOMux_In_PE_set_HIGH()					IOMux_In_PE_GPIO->BSRRL = IOMux_In_PE_Pin
#define IOMux_In_PE_set_LOW()					IOMux_In_PE_GPIO->BSRRH = IOMux_In_PE_Pin
#define IOMux_In_PE_set(value)					if (value) { IOMux_In_PE_set_HIGH(); } else { IOMux_In_PE_set_LOW(); }
#define IOMux_In_PE_toggle()					IOMux_In_PE_GPIO->ODR ^= IOMux_In_PE_Pin
#define IOMux_In_PE_pulse()						do { IOMux_In_PE_set_HIGH(); delay_loopcnt(LOOPCOUNT_50NS); IOMux_In_PE_set_LOW(); } while (0)
#define IOMux_In_PE_npulse()					do { IOMux_In_PE_set_LOW(); delay_loopcnt(LOOPCOUNT_50NS); IOMux_In_PE_set_HIGH(); } while (0)
#define IOMux_In_PE_get()						(IOMux_In_PE_GPIO->IDR & IOMux_In_PE_Pin)
#define IOMux_In_PE_GPIO_Definition				{ .name = "IOMux_In_PE", .pin_name = "C0", .gpio = IOMux_In_PE_GPIO, .pin = IOMux_In_PE_Pin }

// PC1: IOMux_Out_OE (OUT)
#define IOMux_Out_OE_GPIO						GPIOC
#define IOMux_Out_OE_PinSource					GPIO_PinSource1
#define IOMux_Out_OE_Pin						GPIO_Pin_1
#define IOMux_Out_OE_set_HIGH()					IOMux_Out_OE_GPIO->BSRRL = IOMux_Out_OE_Pin
#define IOMux_Out_OE_set_LOW()					IOMux_Out_OE_GPIO->BSRRH = IOMux_Out_OE_Pin
#define IOMux_Out_OE_set(value)					if (value) { IOMux_Out_OE_set_HIGH(); } else { IOMux_Out_OE_set_LOW(); }
#define IOMux_Out_OE_toggle()					IOMux_Out_OE_GPIO->ODR ^= IOMux_Out_OE_Pin
#define IOMux_Out_OE_pulse()					do { IOMux_Out_OE_set_HIGH(); delay_loopcnt(LOOPCOUNT_50NS); IOMux_Out_OE_set_LOW(); } while (0)
#define IOMux_Out_OE_npulse()					do { IOMux_Out_OE_set_LOW(); delay_loopcnt(LOOPCOUNT_50NS); IOMux_Out_OE_set_HIGH(); } while (0)
#define IOMux_Out_OE_get()						(IOMux_Out_OE_GPIO->IDR & IOMux_Out_OE_Pin)
#define IOMux_Out_OE_GPIO_Definition			{ .name = "IOMux_Out_OE", .pin_name = "C1", .gpio = IOMux_Out_OE_GPIO, .pin = IOMux_Out_OE_Pin }

// PC2: IOMux_Out_STCP (OUT)
#define IOMux_Out_STCP_GPIO						GPIOC
#define IOMux_Out_STCP_PinSource				GPIO_PinSource2
#define IOMux_Out_STCP_Pin						GPIO_Pin_2
#define IOMux_Out_STCP_set_HIGH()				IOMux_Out_STCP_GPIO->BSRRL = IOMux_Out_STCP_Pin
#define IOMux_Out_STCP_set_LOW()				IOMux_Out_STCP_GPIO->BSRRH = IOMux_Out_STCP_Pin
#define IOMux_Out_STCP_set(value)				if (value) { IOMux_Out_STCP_set_HIGH(); } else { IOMux_Out_STCP_set_LOW(); }
#define IOMux_Out_STCP_toggle()					IOMux_Out_STCP_GPIO->ODR ^= IOMux_Out_STCP_Pin
#define IOMux_Out_STCP_pulse()					do { IOMux_Out_STCP_set_HIGH(); delay_loopcnt(LOOPCOUNT_50NS); IOMux_Out_STCP_set_LOW(); } while (0)
#define IOMux_Out_STCP_npulse()					do { IOMux_Out_STCP_set_LOW(); delay_loopcnt(LOOPCOUNT_50NS); IOMux_Out_STCP_set_HIGH(); } while (0)
#define IOMux_Out_STCP_get()					(IOMux_Out_STCP_GPIO->IDR & IOMux_Out_STCP_Pin)
#define IOMux_Out_STCP_GPIO_Definition			{ .name = "IOMux_Out_STCP", .pin_name = "C2", .gpio = IOMux_Out_STCP_GPIO, .pin = IOMux_Out_STCP_Pin }

// PC3: AudioPDM_OUT (IN)
#define AudioPDM_OUT_GPIO						GPIOC
#define AudioPDM_OUT_PinSource					GPIO_PinSource3
#define AudioPDM_OUT_Pin						GPIO_Pin_3
#define AudioPDM_OUT_set_HIGH()					AudioPDM_OUT_GPIO->BSRRL = AudioPDM_OUT_Pin
#define AudioPDM_OUT_set_LOW()					AudioPDM_OUT_GPIO->BSRRH = AudioPDM_OUT_Pin
#define AudioPDM_OUT_set(value)					if (value) { AudioPDM_OUT_set_HIGH(); } else { AudioPDM_OUT_set_LOW(); }
#define AudioPDM_OUT_toggle()					AudioPDM_OUT_GPIO->ODR ^= AudioPDM_OUT_Pin
#define AudioPDM_OUT_pulse()					do { AudioPDM_OUT_set_HIGH(); delay_loopcnt(LOOPCOUNT_50NS); AudioPDM_OUT_set_LOW(); } while (0)
#define AudioPDM_OUT_npulse()					do { AudioPDM_OUT_set_LOW(); delay_loopcnt(LOOPCOUNT_50NS); AudioPDM_OUT_set_HIGH(); } while (0)
#define AudioPDM_OUT_get()						(AudioPDM_OUT_GPIO->IDR & AudioPDM_OUT_Pin)
#define AudioPDM_OUT_GPIO_Definition			{ .name = "AudioPDM_OUT", .pin_name = "C3", .gpio = AudioPDM_OUT_GPIO, .pin = AudioPDM_OUT_Pin }

// PC7: Audio_MCK (?)
#define Audio_MCK_GPIO							GPIOC
#define Audio_MCK_PinSource						GPIO_PinSource7
#define Audio_MCK_Pin							GPIO_Pin_7
#define Audio_MCK_set_HIGH()					Audio_MCK_GPIO->BSRRL = Audio_MCK_Pin
#define Audio_MCK_set_LOW()						Audio_MCK_GPIO->BSRRH = Audio_MCK_Pin
#define Audio_MCK_set(value)					if (value) { Audio_MCK_set_HIGH(); } else { Audio_MCK_set_LOW(); }
#define Audio_MCK_toggle()						Audio_MCK_GPIO->ODR ^= Audio_MCK_Pin
#define Audio_MCK_pulse()						do { Audio_MCK_set_HIGH(); delay_loopcnt(LOOPCOUNT_50NS); Audio_MCK_set_LOW(); } while (0)
#define Audio_MCK_npulse()						do { Audio_MCK_set_LOW(); delay_loopcnt(LOOPCOUNT_50NS); Audio_MCK_set_HIGH(); } while (0)
#define Audio_MCK_get()							(Audio_MCK_GPIO->IDR & Audio_MCK_Pin)
#define Audio_MCK_GPIO_Definition				{ .name = "Audio_MCK", .pin_name = "C7", .gpio = Audio_MCK_GPIO, .pin = Audio_MCK_Pin }

// PC10: Audio_SCK (?)
#define Audio_SCK_GPIO							GPIOC
#define Audio_SCK_PinSource						GPIO_PinSource10
#define Audio_SCK_Pin							GPIO_Pin_10
#define Audio_SCK_set_HIGH()					Audio_SCK_GPIO->BSRRL = Audio_SCK_Pin
#define Audio_SCK_set_LOW()						Audio_SCK_GPIO->BSRRH = Audio_SCK_Pin
#define Audio_SCK_set(value)					if (value) { Audio_SCK_set_HIGH(); } else { Audio_SCK_set_LOW(); }
#define Audio_SCK_toggle()						Audio_SCK_GPIO->ODR ^= Audio_SCK_Pin
#define Audio_SCK_pulse()						do { Audio_SCK_set_HIGH(); delay_loopcnt(LOOPCOUNT_50NS); Audio_SCK_set_LOW(); } while (0)
#define Audio_SCK_npulse()						do { Audio_SCK_set_LOW(); delay_loopcnt(LOOPCOUNT_50NS); Audio_SCK_set_HIGH(); } while (0)
#define Audio_SCK_get()							(Audio_SCK_GPIO->IDR & Audio_SCK_Pin)
#define Audio_SCK_GPIO_Definition				{ .name = "Audio_SCK", .pin_name = "C10", .gpio = Audio_SCK_GPIO, .pin = Audio_SCK_Pin }
// PC10: IOMux_SCK (OUT)
#define IOMux_SCK_GPIO							GPIOC
#define IOMux_SCK_PinSource						GPIO_PinSource10
#define IOMux_SCK_Pin							GPIO_Pin_10
#define IOMux_SCK_set_HIGH()					IOMux_SCK_GPIO->BSRRL = IOMux_SCK_Pin
#define IOMux_SCK_set_LOW()						IOMux_SCK_GPIO->BSRRH = IOMux_SCK_Pin
#define IOMux_SCK_set(value)					if (value) { IOMux_SCK_set_HIGH(); } else { IOMux_SCK_set_LOW(); }
#define IOMux_SCK_toggle()						IOMux_SCK_GPIO->ODR ^= IOMux_SCK_Pin
#define IOMux_SCK_pulse()						do { IOMux_SCK_set_HIGH(); delay_loopcnt(LOOPCOUNT_50NS); IOMux_SCK_set_LOW(); } while (0)
#define IOMux_SCK_npulse()						do { IOMux_SCK_set_LOW(); delay_loopcnt(LOOPCOUNT_50NS); IOMux_SCK_set_HIGH(); } while (0)
#define IOMux_SCK_get()							(IOMux_SCK_GPIO->IDR & IOMux_SCK_Pin)
#define IOMux_SCK_GPIO_Definition				{ .name = "IOMux_SCK", .pin_name = "C10", .gpio = IOMux_SCK_GPIO, .pin = IOMux_SCK_Pin }

// PC11: IOMux_MISO (IN)
#define IOMux_MISO_GPIO							GPIOC
#define IOMux_MISO_PinSource					GPIO_PinSource11
#define IOMux_MISO_Pin							GPIO_Pin_11
#define IOMux_MISO_set_HIGH()					IOMux_MISO_GPIO->BSRRL = IOMux_MISO_Pin
#define IOMux_MISO_set_LOW()					IOMux_MISO_GPIO->BSRRH = IOMux_MISO_Pin
#define IOMux_MISO_set(value)					if (value) { IOMux_MISO_set_HIGH(); } else { IOMux_MISO_set_LOW(); }
#define IOMux_MISO_toggle()						IOMux_MISO_GPIO->ODR ^= IOMux_MISO_Pin
#define IOMux_MISO_pulse()						do { IOMux_MISO_set_HIGH(); delay_loopcnt(LOOPCOUNT_50NS); IOMux_MISO_set_LOW(); } while (0)
#define IOMux_MISO_npulse()						do { IOMux_MISO_set_LOW(); delay_loopcnt(LOOPCOUNT_50NS); IOMux_MISO_set_HIGH(); } while (0)
#define IOMux_MISO_get()						(IOMux_MISO_GPIO->IDR & IOMux_MISO_Pin)
#define IOMux_MISO_GPIO_Definition				{ .name = "IOMux_MISO", .pin_name = "C11", .gpio = IOMux_MISO_GPIO, .pin = IOMux_MISO_Pin }

// PC12: Audio_SD (?)
#define Audio_SD_GPIO							GPIOC
#define Audio_SD_PinSource						GPIO_PinSource12
#define Audio_SD_Pin							GPIO_Pin_12
#define Audio_SD_set_HIGH()						Audio_SD_GPIO->BSRRL = Audio_SD_Pin
#define Audio_SD_set_LOW()						Audio_SD_GPIO->BSRRH = Audio_SD_Pin
#define Audio_SD_set(value)						if (value) { Audio_SD_set_HIGH(); } else { Audio_SD_set_LOW(); }
#define Audio_SD_toggle()						Audio_SD_GPIO->ODR ^= Audio_SD_Pin
#define Audio_SD_pulse()						do { Audio_SD_set_HIGH(); delay_loopcnt(LOOPCOUNT_50NS); Audio_SD_set_LOW(); } while (0)
#define Audio_SD_npulse()						do { Audio_SD_set_LOW(); delay_loopcnt(LOOPCOUNT_50NS); Audio_SD_set_HIGH(); } while (0)
#define Audio_SD_get()							(Audio_SD_GPIO->IDR & Audio_SD_Pin)
#define Audio_SD_GPIO_Definition				{ .name = "Audio_SD", .pin_name = "C12", .gpio = Audio_SD_GPIO, .pin = Audio_SD_Pin }
// PC12: IOMux_MOSI (OUT)
#define IOMux_MOSI_GPIO							GPIOC
#define IOMux_MOSI_PinSource					GPIO_PinSource12
#define IOMux_MOSI_Pin							GPIO_Pin_12
#define IOMux_MOSI_set_HIGH()					IOMux_MOSI_GPIO->BSRRL = IOMux_MOSI_Pin
#define IOMux_MOSI_set_LOW()					IOMux_MOSI_GPIO->BSRRH = IOMux_MOSI_Pin
#define IOMux_MOSI_set(value)					if (value) { IOMux_MOSI_set_HIGH(); } else { IOMux_MOSI_set_LOW(); }
#define IOMux_MOSI_toggle()						IOMux_MOSI_GPIO->ODR ^= IOMux_MOSI_Pin
#define IOMux_MOSI_pulse()						do { IOMux_MOSI_set_HIGH(); delay_loopcnt(LOOPCOUNT_50NS); IOMux_MOSI_set_LOW(); } while (0)
#define IOMux_MOSI_npulse()						do { IOMux_MOSI_set_LOW(); delay_loopcnt(LOOPCOUNT_50NS); IOMux_MOSI_set_HIGH(); } while (0)
#define IOMux_MOSI_get()						(IOMux_MOSI_GPIO->IDR & IOMux_MOSI_Pin)
#define IOMux_MOSI_GPIO_Definition				{ .name = "IOMux_MOSI", .pin_name = "C12", .gpio = IOMux_MOSI_GPIO, .pin = IOMux_MOSI_Pin }

// PD0: Display11_nSS (OUT)
#define Display11_nSS_GPIO						GPIOD
#define Display11_nSS_PinSource					GPIO_PinSource0
#define Display11_nSS_Pin						GPIO_Pin_0
#define Display11_nSS_set_HIGH()				Display11_nSS_GPIO->BSRRL = Display11_nSS_Pin
#define Display11_nSS_set_LOW()					Display11_nSS_GPIO->BSRRH = Display11_nSS_Pin
#define Display11_nSS_set(value)				if (value) { Display11_nSS_set_HIGH(); } else { Display11_nSS_set_LOW(); }
#define Display11_nSS_toggle()					Display11_nSS_GPIO->ODR ^= Display11_nSS_Pin
#define Display11_nSS_pulse()					do { Display11_nSS_set_HIGH(); delay_loopcnt(LOOPCOUNT_50NS); Display11_nSS_set_LOW(); } while (0)
#define Display11_nSS_npulse()					do { Display11_nSS_set_LOW(); delay_loopcnt(LOOPCOUNT_50NS); Display11_nSS_set_HIGH(); } while (0)
#define Display11_nSS_get()						(Display11_nSS_GPIO->IDR & Display11_nSS_Pin)
#define Display11_nSS_GPIO_Definition			{ .name = "Display11_nSS", .pin_name = "D0", .gpio = Display11_nSS_GPIO, .pin = Display11_nSS_Pin }

// PD1: Display_nRESET (OUT)
#define Display_nRESET_GPIO						GPIOD
#define Display_nRESET_PinSource				GPIO_PinSource1
#define Display_nRESET_Pin						GPIO_Pin_1
#define Display_nRESET_set_HIGH()				Display_nRESET_GPIO->BSRRL = Display_nRESET_Pin
#define Display_nRESET_set_LOW()				Display_nRESET_GPIO->BSRRH = Display_nRESET_Pin
#define Display_nRESET_set(value)				if (value) { Display_nRESET_set_HIGH(); } else { Display_nRESET_set_LOW(); }
#define Display_nRESET_toggle()					Display_nRESET_GPIO->ODR ^= Display_nRESET_Pin
#define Display_nRESET_pulse()					do { Display_nRESET_set_HIGH(); delay_loopcnt(LOOPCOUNT_50NS); Display_nRESET_set_LOW(); } while (0)
#define Display_nRESET_npulse()					do { Display_nRESET_set_LOW(); delay_loopcnt(LOOPCOUNT_50NS); Display_nRESET_set_HIGH(); } while (0)
#define Display_nRESET_get()					(Display_nRESET_GPIO->IDR & Display_nRESET_Pin)
#define Display_nRESET_GPIO_Definition			{ .name = "Display_nRESET", .pin_name = "D1", .gpio = Display_nRESET_GPIO, .pin = Display_nRESET_Pin }

// PD2: Display_DC (OUT)
#define Display_DC_GPIO							GPIOD
#define Display_DC_PinSource					GPIO_PinSource2
#define Display_DC_Pin							GPIO_Pin_2
#define Display_DC_set_HIGH()					Display_DC_GPIO->BSRRL = Display_DC_Pin
#define Display_DC_set_LOW()					Display_DC_GPIO->BSRRH = Display_DC_Pin
#define Display_DC_set(value)					if (value) { Display_DC_set_HIGH(); } else { Display_DC_set_LOW(); }
#define Display_DC_toggle()						Display_DC_GPIO->ODR ^= Display_DC_Pin
#define Display_DC_pulse()						do { Display_DC_set_HIGH(); delay_loopcnt(LOOPCOUNT_50NS); Display_DC_set_LOW(); } while (0)
#define Display_DC_npulse()						do { Display_DC_set_LOW(); delay_loopcnt(LOOPCOUNT_50NS); Display_DC_set_HIGH(); } while (0)
#define Display_DC_get()						(Display_DC_GPIO->IDR & Display_DC_Pin)
#define Display_DC_GPIO_Definition				{ .name = "Display_DC", .pin_name = "D2", .gpio = Display_DC_GPIO, .pin = Display_DC_Pin }

// PD3: Display8_nSS (OUT)
#define Display8_nSS_GPIO						GPIOD
#define Display8_nSS_PinSource					GPIO_PinSource3
#define Display8_nSS_Pin						GPIO_Pin_3
#define Display8_nSS_set_HIGH()					Display8_nSS_GPIO->BSRRL = Display8_nSS_Pin
#define Display8_nSS_set_LOW()					Display8_nSS_GPIO->BSRRH = Display8_nSS_Pin
#define Display8_nSS_set(value)					if (value) { Display8_nSS_set_HIGH(); } else { Display8_nSS_set_LOW(); }
#define Display8_nSS_toggle()					Display8_nSS_GPIO->ODR ^= Display8_nSS_Pin
#define Display8_nSS_pulse()					do { Display8_nSS_set_HIGH(); delay_loopcnt(LOOPCOUNT_50NS); Display8_nSS_set_LOW(); } while (0)
#define Display8_nSS_npulse()					do { Display8_nSS_set_LOW(); delay_loopcnt(LOOPCOUNT_50NS); Display8_nSS_set_HIGH(); } while (0)
#define Display8_nSS_get()						(Display8_nSS_GPIO->IDR & Display8_nSS_Pin)
#define Display8_nSS_GPIO_Definition			{ .name = "Display8_nSS", .pin_name = "D3", .gpio = Display8_nSS_GPIO, .pin = Display8_nSS_Pin }

// PD4: AudioReset (OUT)
#define AudioReset_GPIO							GPIOD
#define AudioReset_PinSource					GPIO_PinSource4
#define AudioReset_Pin							GPIO_Pin_4
#define AudioReset_set_HIGH()					AudioReset_GPIO->BSRRL = AudioReset_Pin
#define AudioReset_set_LOW()					AudioReset_GPIO->BSRRH = AudioReset_Pin
#define AudioReset_set(value)					if (value) { AudioReset_set_HIGH(); } else { AudioReset_set_LOW(); }
#define AudioReset_toggle()						AudioReset_GPIO->ODR ^= AudioReset_Pin
#define AudioReset_pulse()						do { AudioReset_set_HIGH(); delay_loopcnt(LOOPCOUNT_50NS); AudioReset_set_LOW(); } while (0)
#define AudioReset_npulse()						do { AudioReset_set_LOW(); delay_loopcnt(LOOPCOUNT_50NS); AudioReset_set_HIGH(); } while (0)
#define AudioReset_get()						(AudioReset_GPIO->IDR & AudioReset_Pin)
#define AudioReset_GPIO_Definition				{ .name = "AudioReset", .pin_name = "D4", .gpio = AudioReset_GPIO, .pin = AudioReset_Pin }
// PD4: Display9_nSS (OUT)
#define Display9_nSS_GPIO						GPIOD
#define Display9_nSS_PinSource					GPIO_PinSource4
#define Display9_nSS_Pin						GPIO_Pin_4
#define Display9_nSS_set_HIGH()					Display9_nSS_GPIO->BSRRL = Display9_nSS_Pin
#define Display9_nSS_set_LOW()					Display9_nSS_GPIO->BSRRH = Display9_nSS_Pin
#define Display9_nSS_set(value)					if (value) { Display9_nSS_set_HIGH(); } else { Display9_nSS_set_LOW(); }
#define Display9_nSS_toggle()					Display9_nSS_GPIO->ODR ^= Display9_nSS_Pin
#define Display9_nSS_pulse()					do { Display9_nSS_set_HIGH(); delay_loopcnt(LOOPCOUNT_50NS); Display9_nSS_set_LOW(); } while (0)
#define Display9_nSS_npulse()					do { Display9_nSS_set_LOW(); delay_loopcnt(LOOPCOUNT_50NS); Display9_nSS_set_HIGH(); } while (0)
#define Display9_nSS_get()						(Display9_nSS_GPIO->IDR & Display9_nSS_Pin)
#define Display9_nSS_GPIO_Definition			{ .name = "Display9_nSS", .pin_name = "D4", .gpio = Display9_nSS_GPIO, .pin = Display9_nSS_Pin }

// PD5: USB_FS_OverCurrent (IN)
#define USB_FS_OverCurrent_GPIO					GPIOD
#define USB_FS_OverCurrent_PinSource			GPIO_PinSource5
#define USB_FS_OverCurrent_Pin					GPIO_Pin_5
#define USB_FS_OverCurrent_set_HIGH()			USB_FS_OverCurrent_GPIO->BSRRL = USB_FS_OverCurrent_Pin
#define USB_FS_OverCurrent_set_LOW()			USB_FS_OverCurrent_GPIO->BSRRH = USB_FS_OverCurrent_Pin
#define USB_FS_OverCurrent_set(value)			if (value) { USB_FS_OverCurrent_set_HIGH(); } else { USB_FS_OverCurrent_set_LOW(); }
#define USB_FS_OverCurrent_toggle()				USB_FS_OverCurrent_GPIO->ODR ^= USB_FS_OverCurrent_Pin
#define USB_FS_OverCurrent_pulse()				do { USB_FS_OverCurrent_set_HIGH(); delay_loopcnt(LOOPCOUNT_50NS); USB_FS_OverCurrent_set_LOW(); } while (0)
#define USB_FS_OverCurrent_npulse()				do { USB_FS_OverCurrent_set_LOW(); delay_loopcnt(LOOPCOUNT_50NS); USB_FS_OverCurrent_set_HIGH(); } while (0)
#define USB_FS_OverCurrent_get()				(USB_FS_OverCurrent_GPIO->IDR & USB_FS_OverCurrent_Pin)
#define USB_FS_OverCurrent_GPIO_Definition		{ .name = "USB_FS_OverCurrent", .pin_name = "D5", .gpio = USB_FS_OverCurrent_GPIO, .pin = USB_FS_OverCurrent_Pin }
// PD5: USART_TX (OUT)
#define USART_TX_GPIO							GPIOD
#define USART_TX_PinSource						GPIO_PinSource5
#define USART_TX_Pin							GPIO_Pin_5
#define USART_TX_set_HIGH()						USART_TX_GPIO->BSRRL = USART_TX_Pin
#define USART_TX_set_LOW()						USART_TX_GPIO->BSRRH = USART_TX_Pin
#define USART_TX_set(value)						if (value) { USART_TX_set_HIGH(); } else { USART_TX_set_LOW(); }
#define USART_TX_toggle()						USART_TX_GPIO->ODR ^= USART_TX_Pin
#define USART_TX_pulse()						do { USART_TX_set_HIGH(); delay_loopcnt(LOOPCOUNT_50NS); USART_TX_set_LOW(); } while (0)
#define USART_TX_npulse()						do { USART_TX_set_LOW(); delay_loopcnt(LOOPCOUNT_50NS); USART_TX_set_HIGH(); } while (0)
#define USART_TX_get()							(USART_TX_GPIO->IDR & USART_TX_Pin)
#define USART_TX_GPIO_Definition				{ .name = "USART_TX", .pin_name = "D5", .gpio = USART_TX_GPIO, .pin = USART_TX_Pin }

// PD6: USART_RX (IN)
#define USART_RX_GPIO							GPIOD
#define USART_RX_PinSource						GPIO_PinSource6
#define USART_RX_Pin							GPIO_Pin_6
#define USART_RX_set_HIGH()						USART_RX_GPIO->BSRRL = USART_RX_Pin
#define USART_RX_set_LOW()						USART_RX_GPIO->BSRRH = USART_RX_Pin
#define USART_RX_set(value)						if (value) { USART_RX_set_HIGH(); } else { USART_RX_set_LOW(); }
#define USART_RX_toggle()						USART_RX_GPIO->ODR ^= USART_RX_Pin
#define USART_RX_pulse()						do { USART_RX_set_HIGH(); delay_loopcnt(LOOPCOUNT_50NS); USART_RX_set_LOW(); } while (0)
#define USART_RX_npulse()						do { USART_RX_set_LOW(); delay_loopcnt(LOOPCOUNT_50NS); USART_RX_set_HIGH(); } while (0)
#define USART_RX_get()							(USART_RX_GPIO->IDR & USART_RX_Pin)
#define USART_RX_GPIO_Definition				{ .name = "USART_RX", .pin_name = "D6", .gpio = USART_RX_GPIO, .pin = USART_RX_Pin }

// PD7: Display10_nSS (OUT)
#define Display10_nSS_GPIO						GPIOD
#define Display10_nSS_PinSource					GPIO_PinSource7
#define Display10_nSS_Pin						GPIO_Pin_7
#define Display10_nSS_set_HIGH()				Display10_nSS_GPIO->BSRRL = Display10_nSS_Pin
#define Display10_nSS_set_LOW()					Display10_nSS_GPIO->BSRRH = Display10_nSS_Pin
#define Display10_nSS_set(value)				if (value) { Display10_nSS_set_HIGH(); } else { Display10_nSS_set_LOW(); }
#define Display10_nSS_toggle()					Display10_nSS_GPIO->ODR ^= Display10_nSS_Pin
#define Display10_nSS_pulse()					do { Display10_nSS_set_HIGH(); delay_loopcnt(LOOPCOUNT_50NS); Display10_nSS_set_LOW(); } while (0)
#define Display10_nSS_npulse()					do { Display10_nSS_set_LOW(); delay_loopcnt(LOOPCOUNT_50NS); Display10_nSS_set_HIGH(); } while (0)
#define Display10_nSS_get()						(Display10_nSS_GPIO->IDR & Display10_nSS_Pin)
#define Display10_nSS_GPIO_Definition			{ .name = "Display10_nSS", .pin_name = "D7", .gpio = Display10_nSS_GPIO, .pin = Display10_nSS_Pin }

// PD8: Display12_nSS (OUT)
#define Display12_nSS_GPIO						GPIOD
#define Display12_nSS_PinSource					GPIO_PinSource8
#define Display12_nSS_Pin						GPIO_Pin_8
#define Display12_nSS_set_HIGH()				Display12_nSS_GPIO->BSRRL = Display12_nSS_Pin
#define Display12_nSS_set_LOW()					Display12_nSS_GPIO->BSRRH = Display12_nSS_Pin
#define Display12_nSS_set(value)				if (value) { Display12_nSS_set_HIGH(); } else { Display12_nSS_set_LOW(); }
#define Display12_nSS_toggle()					Display12_nSS_GPIO->ODR ^= Display12_nSS_Pin
#define Display12_nSS_pulse()					do { Display12_nSS_set_HIGH(); delay_loopcnt(LOOPCOUNT_50NS); Display12_nSS_set_LOW(); } while (0)
#define Display12_nSS_npulse()					do { Display12_nSS_set_LOW(); delay_loopcnt(LOOPCOUNT_50NS); Display12_nSS_set_HIGH(); } while (0)
#define Display12_nSS_get()						(Display12_nSS_GPIO->IDR & Display12_nSS_Pin)
#define Display12_nSS_GPIO_Definition			{ .name = "Display12_nSS", .pin_name = "D8", .gpio = Display12_nSS_GPIO, .pin = Display12_nSS_Pin }

// PD9: Rotary1_Btn (IN_UP)
#define Rotary1_Btn_GPIO						GPIOD
#define Rotary1_Btn_PinSource					GPIO_PinSource9
#define Rotary1_Btn_Pin							GPIO_Pin_9
#define Rotary1_Btn_set_HIGH()					Rotary1_Btn_GPIO->BSRRL = Rotary1_Btn_Pin
#define Rotary1_Btn_set_LOW()					Rotary1_Btn_GPIO->BSRRH = Rotary1_Btn_Pin
#define Rotary1_Btn_set(value)					if (value) { Rotary1_Btn_set_HIGH(); } else { Rotary1_Btn_set_LOW(); }
#define Rotary1_Btn_toggle()					Rotary1_Btn_GPIO->ODR ^= Rotary1_Btn_Pin
#define Rotary1_Btn_pulse()						do { Rotary1_Btn_set_HIGH(); delay_loopcnt(LOOPCOUNT_50NS); Rotary1_Btn_set_LOW(); } while (0)
#define Rotary1_Btn_npulse()					do { Rotary1_Btn_set_LOW(); delay_loopcnt(LOOPCOUNT_50NS); Rotary1_Btn_set_HIGH(); } while (0)
#define Rotary1_Btn_get()						(Rotary1_Btn_GPIO->IDR & Rotary1_Btn_Pin)
#define Rotary1_Btn_GPIO_Definition				{ .name = "Rotary1_Btn", .pin_name = "D9", .gpio = Rotary1_Btn_GPIO, .pin = Rotary1_Btn_Pin }

// PD10: Rotary1_A (IN_UP)
#define Rotary1_A_GPIO							GPIOD
#define Rotary1_A_PinSource						GPIO_PinSource10
#define Rotary1_A_Pin							GPIO_Pin_10
#define Rotary1_A_set_HIGH()					Rotary1_A_GPIO->BSRRL = Rotary1_A_Pin
#define Rotary1_A_set_LOW()						Rotary1_A_GPIO->BSRRH = Rotary1_A_Pin
#define Rotary1_A_set(value)					if (value) { Rotary1_A_set_HIGH(); } else { Rotary1_A_set_LOW(); }
#define Rotary1_A_toggle()						Rotary1_A_GPIO->ODR ^= Rotary1_A_Pin
#define Rotary1_A_pulse()						do { Rotary1_A_set_HIGH(); delay_loopcnt(LOOPCOUNT_50NS); Rotary1_A_set_LOW(); } while (0)
#define Rotary1_A_npulse()						do { Rotary1_A_set_LOW(); delay_loopcnt(LOOPCOUNT_50NS); Rotary1_A_set_HIGH(); } while (0)
#define Rotary1_A_get()							(Rotary1_A_GPIO->IDR & Rotary1_A_Pin)
#define Rotary1_A_GPIO_Definition				{ .name = "Rotary1_A", .pin_name = "D10", .gpio = Rotary1_A_GPIO, .pin = Rotary1_A_Pin }

// PD11: Rotary1_B (IN_UP)
#define Rotary1_B_GPIO							GPIOD
#define Rotary1_B_PinSource						GPIO_PinSource11
#define Rotary1_B_Pin							GPIO_Pin_11
#define Rotary1_B_set_HIGH()					Rotary1_B_GPIO->BSRRL = Rotary1_B_Pin
#define Rotary1_B_set_LOW()						Rotary1_B_GPIO->BSRRH = Rotary1_B_Pin
#define Rotary1_B_set(value)					if (value) { Rotary1_B_set_HIGH(); } else { Rotary1_B_set_LOW(); }
#define Rotary1_B_toggle()						Rotary1_B_GPIO->ODR ^= Rotary1_B_Pin
#define Rotary1_B_pulse()						do { Rotary1_B_set_HIGH(); delay_loopcnt(LOOPCOUNT_50NS); Rotary1_B_set_LOW(); } while (0)
#define Rotary1_B_npulse()						do { Rotary1_B_set_LOW(); delay_loopcnt(LOOPCOUNT_50NS); Rotary1_B_set_HIGH(); } while (0)
#define Rotary1_B_get()							(Rotary1_B_GPIO->IDR & Rotary1_B_Pin)
#define Rotary1_B_GPIO_Definition				{ .name = "Rotary1_B", .pin_name = "D11", .gpio = Rotary1_B_GPIO, .pin = Rotary1_B_Pin }

// PD12: LEDGreen (OUT)
#define LEDGreen_GPIO							GPIOD
#define LEDGreen_PinSource						GPIO_PinSource12
#define LEDGreen_Pin							GPIO_Pin_12
#define LEDGreen_set_HIGH()						LEDGreen_GPIO->BSRRL = LEDGreen_Pin
#define LEDGreen_set_LOW()						LEDGreen_GPIO->BSRRH = LEDGreen_Pin
#define LEDGreen_set(value)						if (value) { LEDGreen_set_HIGH(); } else { LEDGreen_set_LOW(); }
#define LEDGreen_toggle()						LEDGreen_GPIO->ODR ^= LEDGreen_Pin
#define LEDGreen_pulse()						do { LEDGreen_set_HIGH(); delay_loopcnt(LOOPCOUNT_50NS); LEDGreen_set_LOW(); } while (0)
#define LEDGreen_npulse()						do { LEDGreen_set_LOW(); delay_loopcnt(LOOPCOUNT_50NS); LEDGreen_set_HIGH(); } while (0)
#define LEDGreen_get()							(LEDGreen_GPIO->IDR & LEDGreen_Pin)
#define LEDGreen_GPIO_Definition				{ .name = "LEDGreen", .pin_name = "D12", .gpio = LEDGreen_GPIO, .pin = LEDGreen_Pin }

// PD13: LEDOrange (OUT)
#define LEDOrange_GPIO							GPIOD
#define LEDOrange_PinSource						GPIO_PinSource13
#define LEDOrange_Pin							GPIO_Pin_13
#define LEDOrange_set_HIGH()					LEDOrange_GPIO->BSRRL = LEDOrange_Pin
#define LEDOrange_set_LOW()						LEDOrange_GPIO->BSRRH = LEDOrange_Pin
#define LEDOrange_set(value)					if (value) { LEDOrange_set_HIGH(); } else { LEDOrange_set_LOW(); }
#define LEDOrange_toggle()						LEDOrange_GPIO->ODR ^= LEDOrange_Pin
#define LEDOrange_pulse()						do { LEDOrange_set_HIGH(); delay_loopcnt(LOOPCOUNT_50NS); LEDOrange_set_LOW(); } while (0)
#define LEDOrange_npulse()						do { LEDOrange_set_LOW(); delay_loopcnt(LOOPCOUNT_50NS); LEDOrange_set_HIGH(); } while (0)
#define LEDOrange_get()							(LEDOrange_GPIO->IDR & LEDOrange_Pin)
#define LEDOrange_GPIO_Definition				{ .name = "LEDOrange", .pin_name = "D13", .gpio = LEDOrange_GPIO, .pin = LEDOrange_Pin }

// PD14: LEDRed (OUT)
#define LEDRed_GPIO								GPIOD
#define LEDRed_PinSource						GPIO_PinSource14
#define LEDRed_Pin								GPIO_Pin_14
#define LEDRed_set_HIGH()						LEDRed_GPIO->BSRRL = LEDRed_Pin
#define LEDRed_set_LOW()						LEDRed_GPIO->BSRRH = LEDRed_Pin
#define LEDRed_set(value)						if (value) { LEDRed_set_HIGH(); } else { LEDRed_set_LOW(); }
#define LEDRed_toggle()							LEDRed_GPIO->ODR ^= LEDRed_Pin
#define LEDRed_pulse()							do { LEDRed_set_HIGH(); delay_loopcnt(LOOPCOUNT_50NS); LEDRed_set_LOW(); } while (0)
#define LEDRed_npulse()							do { LEDRed_set_LOW(); delay_loopcnt(LOOPCOUNT_50NS); LEDRed_set_HIGH(); } while (0)
#define LEDRed_get()							(LEDRed_GPIO->IDR & LEDRed_Pin)
#define LEDRed_GPIO_Definition					{ .name = "LEDRed", .pin_name = "D14", .gpio = LEDRed_GPIO, .pin = LEDRed_Pin }

// PD15: LEDBlue (OUT)
#define LEDBlue_GPIO							GPIOD
#define LEDBlue_PinSource						GPIO_PinSource15
#define LEDBlue_Pin								GPIO_Pin_15
#define LEDBlue_set_HIGH()						LEDBlue_GPIO->BSRRL = LEDBlue_Pin
#define LEDBlue_set_LOW()						LEDBlue_GPIO->BSRRH = LEDBlue_Pin
#define LEDBlue_set(value)						if (value) { LEDBlue_set_HIGH(); } else { LEDBlue_set_LOW(); }
#define LEDBlue_toggle()						LEDBlue_GPIO->ODR ^= LEDBlue_Pin
#define LEDBlue_pulse()							do { LEDBlue_set_HIGH(); delay_loopcnt(LOOPCOUNT_50NS); LEDBlue_set_LOW(); } while (0)
#define LEDBlue_npulse()						do { LEDBlue_set_LOW(); delay_loopcnt(LOOPCOUNT_50NS); LEDBlue_set_HIGH(); } while (0)
#define LEDBlue_get()							(LEDBlue_GPIO->IDR & LEDBlue_Pin)
#define LEDBlue_GPIO_Definition					{ .name = "LEDBlue", .pin_name = "D15", .gpio = LEDBlue_GPIO, .pin = LEDBlue_Pin }

// PE0: MEMS_INT1 (IN)
#define MEMS_INT1_GPIO							GPIOE
#define MEMS_INT1_PinSource						GPIO_PinSource0
#define MEMS_INT1_Pin							GPIO_Pin_0
#define MEMS_INT1_set_HIGH()					MEMS_INT1_GPIO->BSRRL = MEMS_INT1_Pin
#define MEMS_INT1_set_LOW()						MEMS_INT1_GPIO->BSRRH = MEMS_INT1_Pin
#define MEMS_INT1_set(value)					if (value) { MEMS_INT1_set_HIGH(); } else { MEMS_INT1_set_LOW(); }
#define MEMS_INT1_toggle()						MEMS_INT1_GPIO->ODR ^= MEMS_INT1_Pin
#define MEMS_INT1_pulse()						do { MEMS_INT1_set_HIGH(); delay_loopcnt(LOOPCOUNT_50NS); MEMS_INT1_set_LOW(); } while (0)
#define MEMS_INT1_npulse()						do { MEMS_INT1_set_LOW(); delay_loopcnt(LOOPCOUNT_50NS); MEMS_INT1_set_HIGH(); } while (0)
#define MEMS_INT1_get()							(MEMS_INT1_GPIO->IDR & MEMS_INT1_Pin)
#define MEMS_INT1_GPIO_Definition				{ .name = "MEMS_INT1", .pin_name = "E0", .gpio = MEMS_INT1_GPIO, .pin = MEMS_INT1_Pin }

// PE1: MEMS_INT2 (IN)
#define MEMS_INT2_GPIO							GPIOE
#define MEMS_INT2_PinSource						GPIO_PinSource1
#define MEMS_INT2_Pin							GPIO_Pin_1
#define MEMS_INT2_set_HIGH()					MEMS_INT2_GPIO->BSRRL = MEMS_INT2_Pin
#define MEMS_INT2_set_LOW()						MEMS_INT2_GPIO->BSRRH = MEMS_INT2_Pin
#define MEMS_INT2_set(value)					if (value) { MEMS_INT2_set_HIGH(); } else { MEMS_INT2_set_LOW(); }
#define MEMS_INT2_toggle()						MEMS_INT2_GPIO->ODR ^= MEMS_INT2_Pin
#define MEMS_INT2_pulse()						do { MEMS_INT2_set_HIGH(); delay_loopcnt(LOOPCOUNT_50NS); MEMS_INT2_set_LOW(); } while (0)
#define MEMS_INT2_npulse()						do { MEMS_INT2_set_LOW(); delay_loopcnt(LOOPCOUNT_50NS); MEMS_INT2_set_HIGH(); } while (0)
#define MEMS_INT2_get()							(MEMS_INT2_GPIO->IDR & MEMS_INT2_Pin)
#define MEMS_INT2_GPIO_Definition				{ .name = "MEMS_INT2", .pin_name = "E1", .gpio = MEMS_INT2_GPIO, .pin = MEMS_INT2_Pin }

// PE3: MEMS_I2C_SPI (OUT)
#define MEMS_I2C_SPI_GPIO						GPIOE
#define MEMS_I2C_SPI_PinSource					GPIO_PinSource3
#define MEMS_I2C_SPI_Pin						GPIO_Pin_3
#define MEMS_I2C_SPI_set_HIGH()					MEMS_I2C_SPI_GPIO->BSRRL = MEMS_I2C_SPI_Pin
#define MEMS_I2C_SPI_set_LOW()					MEMS_I2C_SPI_GPIO->BSRRH = MEMS_I2C_SPI_Pin
#define MEMS_I2C_SPI_set(value)					if (value) { MEMS_I2C_SPI_set_HIGH(); } else { MEMS_I2C_SPI_set_LOW(); }
#define MEMS_I2C_SPI_toggle()					MEMS_I2C_SPI_GPIO->ODR ^= MEMS_I2C_SPI_Pin
#define MEMS_I2C_SPI_pulse()					do { MEMS_I2C_SPI_set_HIGH(); delay_loopcnt(LOOPCOUNT_50NS); MEMS_I2C_SPI_set_LOW(); } while (0)
#define MEMS_I2C_SPI_npulse()					do { MEMS_I2C_SPI_set_LOW(); delay_loopcnt(LOOPCOUNT_50NS); MEMS_I2C_SPI_set_HIGH(); } while (0)
#define MEMS_I2C_SPI_get()						(MEMS_I2C_SPI_GPIO->IDR & MEMS_I2C_SPI_Pin)
#define MEMS_I2C_SPI_GPIO_Definition			{ .name = "MEMS_I2C_SPI", .pin_name = "E3", .gpio = MEMS_I2C_SPI_GPIO, .pin = MEMS_I2C_SPI_Pin }

// Display_nSS: Group of 12 pins (Display7_nSS, Display12_nSS, Display10_nSS, Display8_nSS, Display1_nSS, Display5_nSS, Display3_nSS, Display4_nSS, Display11_nSS, Display9_nSS, Display6_nSS, Display2_nSS)

#endif
