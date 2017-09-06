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
	const char *connect;
};

// PC0: USB_PowerSwitchOn (OUT) -- {}
#define USB_PowerSwitchOn_GPIO					GPIOC
#define USB_PowerSwitchOn_PinSource				GPIO_PinSource0
#define USB_PowerSwitchOn_Pin					GPIO_Pin_0
#define USB_PowerSwitchOn_set_HIGH()			USB_PowerSwitchOn_GPIO->BSRRL = USB_PowerSwitchOn_Pin
#define USB_PowerSwitchOn_set_LOW()				USB_PowerSwitchOn_GPIO->BSRRH = USB_PowerSwitchOn_Pin
#define USB_PowerSwitchOn_set_ACTIVE()			USB_PowerSwitchOn_set_HIGH()
#define USB_PowerSwitchOn_set_INACTIVE()		USB_PowerSwitchOn_set_LOW()
#define USB_PowerSwitchOn_get()					(USB_PowerSwitchOn_GPIO->IDR & USB_PowerSwitchOn_Pin)
#define USB_PowerSwitchOn_set(value)			if (value) { USB_PowerSwitchOn_set_ACTIVE(); } else { USB_PowerSwitchOn_set_INACTIVE(); }
#define USB_PowerSwitchOn_toggle()				USB_PowerSwitchOn_GPIO->ODR ^= USB_PowerSwitchOn_Pin
#define USB_PowerSwitchOn_pulse()				do { USB_PowerSwitchOn_set_ACTIVE(); delay_loopcnt(LOOPCOUNT_50NS); USB_PowerSwitchOn_set_INACTIVE(); } while (0)
#define USB_PowerSwitchOn_npulse()				do { USB_PowerSwitchOn_set_INACTIVE(); delay_loopcnt(LOOPCOUNT_50NS); USB_PowerSwitchOn_set_ACTIVE(); } while (0)
#define USB_PowerSwitchOn_GPIO_Definition		{ .gpio = USB_PowerSwitchOn_GPIO, .name = "USB_PowerSwitchOn", .pin = USB_PowerSwitchOn_Pin, .pin_name = "C0" }
// PC0: IOMux_In_PE (OUT) -- {'activelow': True, 'init': True, 'connect': 'IOMux 9'}
#define IOMux_In_PE_GPIO						GPIOC
#define IOMux_In_PE_PinSource					GPIO_PinSource0
#define IOMux_In_PE_Pin							GPIO_Pin_0
#define IOMux_In_PE_set_HIGH()					IOMux_In_PE_GPIO->BSRRL = IOMux_In_PE_Pin
#define IOMux_In_PE_set_LOW()					IOMux_In_PE_GPIO->BSRRH = IOMux_In_PE_Pin
#define IOMux_In_PE_set_ACTIVE()				IOMux_In_PE_set_LOW()
#define IOMux_In_PE_set_INACTIVE()				IOMux_In_PE_set_HIGH()
#define IOMux_In_PE_get()						(!(IOMux_In_PE_GPIO->IDR & IOMux_In_PE_Pin))
#define IOMux_In_PE_set(value)					if (value) { IOMux_In_PE_set_ACTIVE(); } else { IOMux_In_PE_set_INACTIVE(); }
#define IOMux_In_PE_toggle()					IOMux_In_PE_GPIO->ODR ^= IOMux_In_PE_Pin
#define IOMux_In_PE_pulse()						do { IOMux_In_PE_set_ACTIVE(); delay_loopcnt(LOOPCOUNT_50NS); IOMux_In_PE_set_INACTIVE(); } while (0)
#define IOMux_In_PE_npulse()					do { IOMux_In_PE_set_INACTIVE(); delay_loopcnt(LOOPCOUNT_50NS); IOMux_In_PE_set_ACTIVE(); } while (0)
#define IOMux_In_PE_GPIO_Definition				{ .connect = "IOMux 9", .gpio = IOMux_In_PE_GPIO, .name = "IOMux_In_PE", .pin = IOMux_In_PE_Pin, .pin_name = "C0" }

// PD5: USB_FS_OverCurrent (IN) -- {}
#define USB_FS_OverCurrent_GPIO					GPIOD
#define USB_FS_OverCurrent_PinSource			GPIO_PinSource5
#define USB_FS_OverCurrent_Pin					GPIO_Pin_5
#define USB_FS_OverCurrent_set_HIGH()			USB_FS_OverCurrent_GPIO->BSRRL = USB_FS_OverCurrent_Pin
#define USB_FS_OverCurrent_set_LOW()			USB_FS_OverCurrent_GPIO->BSRRH = USB_FS_OverCurrent_Pin
#define USB_FS_OverCurrent_set_ACTIVE()			USB_FS_OverCurrent_set_HIGH()
#define USB_FS_OverCurrent_set_INACTIVE()		USB_FS_OverCurrent_set_LOW()
#define USB_FS_OverCurrent_get()				(USB_FS_OverCurrent_GPIO->IDR & USB_FS_OverCurrent_Pin)
#define USB_FS_OverCurrent_set(value)			if (value) { USB_FS_OverCurrent_set_ACTIVE(); } else { USB_FS_OverCurrent_set_INACTIVE(); }
#define USB_FS_OverCurrent_toggle()				USB_FS_OverCurrent_GPIO->ODR ^= USB_FS_OverCurrent_Pin
#define USB_FS_OverCurrent_pulse()				do { USB_FS_OverCurrent_set_ACTIVE(); delay_loopcnt(LOOPCOUNT_50NS); USB_FS_OverCurrent_set_INACTIVE(); } while (0)
#define USB_FS_OverCurrent_npulse()				do { USB_FS_OverCurrent_set_INACTIVE(); delay_loopcnt(LOOPCOUNT_50NS); USB_FS_OverCurrent_set_ACTIVE(); } while (0)
#define USB_FS_OverCurrent_GPIO_Definition		{ .gpio = USB_FS_OverCurrent_GPIO, .name = "USB_FS_OverCurrent", .pin = USB_FS_OverCurrent_Pin, .pin_name = "D5" }
// PD5: USART_TX (OUT) -- {'af': 'USART2', 'init': True}
#define USART_TX_GPIO							GPIOD
#define USART_TX_PinSource						GPIO_PinSource5
#define USART_TX_Pin							GPIO_Pin_5
#define USART_TX_set_HIGH()						USART_TX_GPIO->BSRRL = USART_TX_Pin
#define USART_TX_set_LOW()						USART_TX_GPIO->BSRRH = USART_TX_Pin
#define USART_TX_set_ACTIVE()					USART_TX_set_HIGH()
#define USART_TX_set_INACTIVE()					USART_TX_set_LOW()
#define USART_TX_get()							(USART_TX_GPIO->IDR & USART_TX_Pin)
#define USART_TX_set(value)						if (value) { USART_TX_set_ACTIVE(); } else { USART_TX_set_INACTIVE(); }
#define USART_TX_toggle()						USART_TX_GPIO->ODR ^= USART_TX_Pin
#define USART_TX_pulse()						do { USART_TX_set_ACTIVE(); delay_loopcnt(LOOPCOUNT_50NS); USART_TX_set_INACTIVE(); } while (0)
#define USART_TX_npulse()						do { USART_TX_set_INACTIVE(); delay_loopcnt(LOOPCOUNT_50NS); USART_TX_set_ACTIVE(); } while (0)
#define USART_TX_GPIO_Definition				{ .gpio = USART_TX_GPIO, .name = "USART_TX", .pin = USART_TX_Pin, .pin_name = "D5" }

// PA9: USB_OTG_VBUS (IN) -- {}
#define USB_OTG_VBUS_GPIO						GPIOA
#define USB_OTG_VBUS_PinSource					GPIO_PinSource9
#define USB_OTG_VBUS_Pin						GPIO_Pin_9
#define USB_OTG_VBUS_set_HIGH()					USB_OTG_VBUS_GPIO->BSRRL = USB_OTG_VBUS_Pin
#define USB_OTG_VBUS_set_LOW()					USB_OTG_VBUS_GPIO->BSRRH = USB_OTG_VBUS_Pin
#define USB_OTG_VBUS_set_ACTIVE()				USB_OTG_VBUS_set_HIGH()
#define USB_OTG_VBUS_set_INACTIVE()				USB_OTG_VBUS_set_LOW()
#define USB_OTG_VBUS_get()						(USB_OTG_VBUS_GPIO->IDR & USB_OTG_VBUS_Pin)
#define USB_OTG_VBUS_set(value)					if (value) { USB_OTG_VBUS_set_ACTIVE(); } else { USB_OTG_VBUS_set_INACTIVE(); }
#define USB_OTG_VBUS_toggle()					USB_OTG_VBUS_GPIO->ODR ^= USB_OTG_VBUS_Pin
#define USB_OTG_VBUS_pulse()					do { USB_OTG_VBUS_set_ACTIVE(); delay_loopcnt(LOOPCOUNT_50NS); USB_OTG_VBUS_set_INACTIVE(); } while (0)
#define USB_OTG_VBUS_npulse()					do { USB_OTG_VBUS_set_INACTIVE(); delay_loopcnt(LOOPCOUNT_50NS); USB_OTG_VBUS_set_ACTIVE(); } while (0)
#define USB_OTG_VBUS_GPIO_Definition			{ .gpio = USB_OTG_VBUS_GPIO, .name = "USB_OTG_VBUS", .pin = USB_OTG_VBUS_Pin, .pin_name = "A9" }

// PA11: USB_OTG_M (IO) -- {}
#define USB_OTG_M_GPIO							GPIOA
#define USB_OTG_M_PinSource						GPIO_PinSource11
#define USB_OTG_M_Pin							GPIO_Pin_11
#define USB_OTG_M_set_HIGH()					USB_OTG_M_GPIO->BSRRL = USB_OTG_M_Pin
#define USB_OTG_M_set_LOW()						USB_OTG_M_GPIO->BSRRH = USB_OTG_M_Pin
#define USB_OTG_M_set_ACTIVE()					USB_OTG_M_set_HIGH()
#define USB_OTG_M_set_INACTIVE()				USB_OTG_M_set_LOW()
#define USB_OTG_M_get()							(USB_OTG_M_GPIO->IDR & USB_OTG_M_Pin)
#define USB_OTG_M_set(value)					if (value) { USB_OTG_M_set_ACTIVE(); } else { USB_OTG_M_set_INACTIVE(); }
#define USB_OTG_M_toggle()						USB_OTG_M_GPIO->ODR ^= USB_OTG_M_Pin
#define USB_OTG_M_pulse()						do { USB_OTG_M_set_ACTIVE(); delay_loopcnt(LOOPCOUNT_50NS); USB_OTG_M_set_INACTIVE(); } while (0)
#define USB_OTG_M_npulse()						do { USB_OTG_M_set_INACTIVE(); delay_loopcnt(LOOPCOUNT_50NS); USB_OTG_M_set_ACTIVE(); } while (0)
#define USB_OTG_M_GPIO_Definition				{ .gpio = USB_OTG_M_GPIO, .name = "USB_OTG_M", .pin = USB_OTG_M_Pin, .pin_name = "A11" }

// PA12: USB_OTG_P (IO) -- {}
#define USB_OTG_P_GPIO							GPIOA
#define USB_OTG_P_PinSource						GPIO_PinSource12
#define USB_OTG_P_Pin							GPIO_Pin_12
#define USB_OTG_P_set_HIGH()					USB_OTG_P_GPIO->BSRRL = USB_OTG_P_Pin
#define USB_OTG_P_set_LOW()						USB_OTG_P_GPIO->BSRRH = USB_OTG_P_Pin
#define USB_OTG_P_set_ACTIVE()					USB_OTG_P_set_HIGH()
#define USB_OTG_P_set_INACTIVE()				USB_OTG_P_set_LOW()
#define USB_OTG_P_get()							(USB_OTG_P_GPIO->IDR & USB_OTG_P_Pin)
#define USB_OTG_P_set(value)					if (value) { USB_OTG_P_set_ACTIVE(); } else { USB_OTG_P_set_INACTIVE(); }
#define USB_OTG_P_toggle()						USB_OTG_P_GPIO->ODR ^= USB_OTG_P_Pin
#define USB_OTG_P_pulse()						do { USB_OTG_P_set_ACTIVE(); delay_loopcnt(LOOPCOUNT_50NS); USB_OTG_P_set_INACTIVE(); } while (0)
#define USB_OTG_P_npulse()						do { USB_OTG_P_set_INACTIVE(); delay_loopcnt(LOOPCOUNT_50NS); USB_OTG_P_set_ACTIVE(); } while (0)
#define USB_OTG_P_GPIO_Definition				{ .gpio = USB_OTG_P_GPIO, .name = "USB_OTG_P", .pin = USB_OTG_P_Pin, .pin_name = "A12" }

// PA10: USB_OTG_ID (IN) -- {}
#define USB_OTG_ID_GPIO							GPIOA
#define USB_OTG_ID_PinSource					GPIO_PinSource10
#define USB_OTG_ID_Pin							GPIO_Pin_10
#define USB_OTG_ID_set_HIGH()					USB_OTG_ID_GPIO->BSRRL = USB_OTG_ID_Pin
#define USB_OTG_ID_set_LOW()					USB_OTG_ID_GPIO->BSRRH = USB_OTG_ID_Pin
#define USB_OTG_ID_set_ACTIVE()					USB_OTG_ID_set_HIGH()
#define USB_OTG_ID_set_INACTIVE()				USB_OTG_ID_set_LOW()
#define USB_OTG_ID_get()						(USB_OTG_ID_GPIO->IDR & USB_OTG_ID_Pin)
#define USB_OTG_ID_set(value)					if (value) { USB_OTG_ID_set_ACTIVE(); } else { USB_OTG_ID_set_INACTIVE(); }
#define USB_OTG_ID_toggle()						USB_OTG_ID_GPIO->ODR ^= USB_OTG_ID_Pin
#define USB_OTG_ID_pulse()						do { USB_OTG_ID_set_ACTIVE(); delay_loopcnt(LOOPCOUNT_50NS); USB_OTG_ID_set_INACTIVE(); } while (0)
#define USB_OTG_ID_npulse()						do { USB_OTG_ID_set_INACTIVE(); delay_loopcnt(LOOPCOUNT_50NS); USB_OTG_ID_set_ACTIVE(); } while (0)
#define USB_OTG_ID_GPIO_Definition				{ .gpio = USB_OTG_ID_GPIO, .name = "USB_OTG_ID", .pin = USB_OTG_ID_Pin, .pin_name = "A10" }

// PA0: UserButton (IN) -- {'pulldown': True, 'init': True}
#define UserButton_GPIO							GPIOA
#define UserButton_PinSource					GPIO_PinSource0
#define UserButton_Pin							GPIO_Pin_0
#define UserButton_set_HIGH()					UserButton_GPIO->BSRRL = UserButton_Pin
#define UserButton_set_LOW()					UserButton_GPIO->BSRRH = UserButton_Pin
#define UserButton_set_ACTIVE()					UserButton_set_HIGH()
#define UserButton_set_INACTIVE()				UserButton_set_LOW()
#define UserButton_get()						(UserButton_GPIO->IDR & UserButton_Pin)
#define UserButton_set(value)					if (value) { UserButton_set_ACTIVE(); } else { UserButton_set_INACTIVE(); }
#define UserButton_toggle()						UserButton_GPIO->ODR ^= UserButton_Pin
#define UserButton_pulse()						do { UserButton_set_ACTIVE(); delay_loopcnt(LOOPCOUNT_50NS); UserButton_set_INACTIVE(); } while (0)
#define UserButton_npulse()						do { UserButton_set_INACTIVE(); delay_loopcnt(LOOPCOUNT_50NS); UserButton_set_ACTIVE(); } while (0)
#define UserButton_GPIO_Definition				{ .gpio = UserButton_GPIO, .name = "UserButton", .pin = UserButton_Pin, .pin_name = "A0" }

// PD12: LEDGreen (OUT) -- {'init': True}
#define LEDGreen_GPIO							GPIOD
#define LEDGreen_PinSource						GPIO_PinSource12
#define LEDGreen_Pin							GPIO_Pin_12
#define LEDGreen_set_HIGH()						LEDGreen_GPIO->BSRRL = LEDGreen_Pin
#define LEDGreen_set_LOW()						LEDGreen_GPIO->BSRRH = LEDGreen_Pin
#define LEDGreen_set_ACTIVE()					LEDGreen_set_HIGH()
#define LEDGreen_set_INACTIVE()					LEDGreen_set_LOW()
#define LEDGreen_get()							(LEDGreen_GPIO->IDR & LEDGreen_Pin)
#define LEDGreen_set(value)						if (value) { LEDGreen_set_ACTIVE(); } else { LEDGreen_set_INACTIVE(); }
#define LEDGreen_toggle()						LEDGreen_GPIO->ODR ^= LEDGreen_Pin
#define LEDGreen_pulse()						do { LEDGreen_set_ACTIVE(); delay_loopcnt(LOOPCOUNT_50NS); LEDGreen_set_INACTIVE(); } while (0)
#define LEDGreen_npulse()						do { LEDGreen_set_INACTIVE(); delay_loopcnt(LOOPCOUNT_50NS); LEDGreen_set_ACTIVE(); } while (0)
#define LEDGreen_GPIO_Definition				{ .gpio = LEDGreen_GPIO, .name = "LEDGreen", .pin = LEDGreen_Pin, .pin_name = "D12" }

// PD13: LEDOrange (OUT) -- {'init': True}
#define LEDOrange_GPIO							GPIOD
#define LEDOrange_PinSource						GPIO_PinSource13
#define LEDOrange_Pin							GPIO_Pin_13
#define LEDOrange_set_HIGH()					LEDOrange_GPIO->BSRRL = LEDOrange_Pin
#define LEDOrange_set_LOW()						LEDOrange_GPIO->BSRRH = LEDOrange_Pin
#define LEDOrange_set_ACTIVE()					LEDOrange_set_HIGH()
#define LEDOrange_set_INACTIVE()				LEDOrange_set_LOW()
#define LEDOrange_get()							(LEDOrange_GPIO->IDR & LEDOrange_Pin)
#define LEDOrange_set(value)					if (value) { LEDOrange_set_ACTIVE(); } else { LEDOrange_set_INACTIVE(); }
#define LEDOrange_toggle()						LEDOrange_GPIO->ODR ^= LEDOrange_Pin
#define LEDOrange_pulse()						do { LEDOrange_set_ACTIVE(); delay_loopcnt(LOOPCOUNT_50NS); LEDOrange_set_INACTIVE(); } while (0)
#define LEDOrange_npulse()						do { LEDOrange_set_INACTIVE(); delay_loopcnt(LOOPCOUNT_50NS); LEDOrange_set_ACTIVE(); } while (0)
#define LEDOrange_GPIO_Definition				{ .gpio = LEDOrange_GPIO, .name = "LEDOrange", .pin = LEDOrange_Pin, .pin_name = "D13" }

// PD14: LEDRed (OUT) -- {'init': True}
#define LEDRed_GPIO								GPIOD
#define LEDRed_PinSource						GPIO_PinSource14
#define LEDRed_Pin								GPIO_Pin_14
#define LEDRed_set_HIGH()						LEDRed_GPIO->BSRRL = LEDRed_Pin
#define LEDRed_set_LOW()						LEDRed_GPIO->BSRRH = LEDRed_Pin
#define LEDRed_set_ACTIVE()						LEDRed_set_HIGH()
#define LEDRed_set_INACTIVE()					LEDRed_set_LOW()
#define LEDRed_get()							(LEDRed_GPIO->IDR & LEDRed_Pin)
#define LEDRed_set(value)						if (value) { LEDRed_set_ACTIVE(); } else { LEDRed_set_INACTIVE(); }
#define LEDRed_toggle()							LEDRed_GPIO->ODR ^= LEDRed_Pin
#define LEDRed_pulse()							do { LEDRed_set_ACTIVE(); delay_loopcnt(LOOPCOUNT_50NS); LEDRed_set_INACTIVE(); } while (0)
#define LEDRed_npulse()							do { LEDRed_set_INACTIVE(); delay_loopcnt(LOOPCOUNT_50NS); LEDRed_set_ACTIVE(); } while (0)
#define LEDRed_GPIO_Definition					{ .gpio = LEDRed_GPIO, .name = "LEDRed", .pin = LEDRed_Pin, .pin_name = "D14" }

// PD15: LEDBlue (OUT) -- {'init': True}
#define LEDBlue_GPIO							GPIOD
#define LEDBlue_PinSource						GPIO_PinSource15
#define LEDBlue_Pin								GPIO_Pin_15
#define LEDBlue_set_HIGH()						LEDBlue_GPIO->BSRRL = LEDBlue_Pin
#define LEDBlue_set_LOW()						LEDBlue_GPIO->BSRRH = LEDBlue_Pin
#define LEDBlue_set_ACTIVE()					LEDBlue_set_HIGH()
#define LEDBlue_set_INACTIVE()					LEDBlue_set_LOW()
#define LEDBlue_get()							(LEDBlue_GPIO->IDR & LEDBlue_Pin)
#define LEDBlue_set(value)						if (value) { LEDBlue_set_ACTIVE(); } else { LEDBlue_set_INACTIVE(); }
#define LEDBlue_toggle()						LEDBlue_GPIO->ODR ^= LEDBlue_Pin
#define LEDBlue_pulse()							do { LEDBlue_set_ACTIVE(); delay_loopcnt(LOOPCOUNT_50NS); LEDBlue_set_INACTIVE(); } while (0)
#define LEDBlue_npulse()						do { LEDBlue_set_INACTIVE(); delay_loopcnt(LOOPCOUNT_50NS); LEDBlue_set_ACTIVE(); } while (0)
#define LEDBlue_GPIO_Definition					{ .gpio = LEDBlue_GPIO, .name = "LEDBlue", .pin = LEDBlue_Pin, .pin_name = "D15" }

// PE0: MEMS_INT1 (IN) -- {}
#define MEMS_INT1_GPIO							GPIOE
#define MEMS_INT1_PinSource						GPIO_PinSource0
#define MEMS_INT1_Pin							GPIO_Pin_0
#define MEMS_INT1_set_HIGH()					MEMS_INT1_GPIO->BSRRL = MEMS_INT1_Pin
#define MEMS_INT1_set_LOW()						MEMS_INT1_GPIO->BSRRH = MEMS_INT1_Pin
#define MEMS_INT1_set_ACTIVE()					MEMS_INT1_set_HIGH()
#define MEMS_INT1_set_INACTIVE()				MEMS_INT1_set_LOW()
#define MEMS_INT1_get()							(MEMS_INT1_GPIO->IDR & MEMS_INT1_Pin)
#define MEMS_INT1_set(value)					if (value) { MEMS_INT1_set_ACTIVE(); } else { MEMS_INT1_set_INACTIVE(); }
#define MEMS_INT1_toggle()						MEMS_INT1_GPIO->ODR ^= MEMS_INT1_Pin
#define MEMS_INT1_pulse()						do { MEMS_INT1_set_ACTIVE(); delay_loopcnt(LOOPCOUNT_50NS); MEMS_INT1_set_INACTIVE(); } while (0)
#define MEMS_INT1_npulse()						do { MEMS_INT1_set_INACTIVE(); delay_loopcnt(LOOPCOUNT_50NS); MEMS_INT1_set_ACTIVE(); } while (0)
#define MEMS_INT1_GPIO_Definition				{ .gpio = MEMS_INT1_GPIO, .name = "MEMS_INT1", .pin = MEMS_INT1_Pin, .pin_name = "E0" }

// PE1: MEMS_INT2 (IN) -- {}
#define MEMS_INT2_GPIO							GPIOE
#define MEMS_INT2_PinSource						GPIO_PinSource1
#define MEMS_INT2_Pin							GPIO_Pin_1
#define MEMS_INT2_set_HIGH()					MEMS_INT2_GPIO->BSRRL = MEMS_INT2_Pin
#define MEMS_INT2_set_LOW()						MEMS_INT2_GPIO->BSRRH = MEMS_INT2_Pin
#define MEMS_INT2_set_ACTIVE()					MEMS_INT2_set_HIGH()
#define MEMS_INT2_set_INACTIVE()				MEMS_INT2_set_LOW()
#define MEMS_INT2_get()							(MEMS_INT2_GPIO->IDR & MEMS_INT2_Pin)
#define MEMS_INT2_set(value)					if (value) { MEMS_INT2_set_ACTIVE(); } else { MEMS_INT2_set_INACTIVE(); }
#define MEMS_INT2_toggle()						MEMS_INT2_GPIO->ODR ^= MEMS_INT2_Pin
#define MEMS_INT2_pulse()						do { MEMS_INT2_set_ACTIVE(); delay_loopcnt(LOOPCOUNT_50NS); MEMS_INT2_set_INACTIVE(); } while (0)
#define MEMS_INT2_npulse()						do { MEMS_INT2_set_INACTIVE(); delay_loopcnt(LOOPCOUNT_50NS); MEMS_INT2_set_ACTIVE(); } while (0)
#define MEMS_INT2_GPIO_Definition				{ .gpio = MEMS_INT2_GPIO, .name = "MEMS_INT2", .pin = MEMS_INT2_Pin, .pin_name = "E1" }

// PA7: MEMS_MOSI (OUT) -- {}
#define MEMS_MOSI_GPIO							GPIOA
#define MEMS_MOSI_PinSource						GPIO_PinSource7
#define MEMS_MOSI_Pin							GPIO_Pin_7
#define MEMS_MOSI_set_HIGH()					MEMS_MOSI_GPIO->BSRRL = MEMS_MOSI_Pin
#define MEMS_MOSI_set_LOW()						MEMS_MOSI_GPIO->BSRRH = MEMS_MOSI_Pin
#define MEMS_MOSI_set_ACTIVE()					MEMS_MOSI_set_HIGH()
#define MEMS_MOSI_set_INACTIVE()				MEMS_MOSI_set_LOW()
#define MEMS_MOSI_get()							(MEMS_MOSI_GPIO->IDR & MEMS_MOSI_Pin)
#define MEMS_MOSI_set(value)					if (value) { MEMS_MOSI_set_ACTIVE(); } else { MEMS_MOSI_set_INACTIVE(); }
#define MEMS_MOSI_toggle()						MEMS_MOSI_GPIO->ODR ^= MEMS_MOSI_Pin
#define MEMS_MOSI_pulse()						do { MEMS_MOSI_set_ACTIVE(); delay_loopcnt(LOOPCOUNT_50NS); MEMS_MOSI_set_INACTIVE(); } while (0)
#define MEMS_MOSI_npulse()						do { MEMS_MOSI_set_INACTIVE(); delay_loopcnt(LOOPCOUNT_50NS); MEMS_MOSI_set_ACTIVE(); } while (0)
#define MEMS_MOSI_GPIO_Definition				{ .gpio = MEMS_MOSI_GPIO, .name = "MEMS_MOSI", .pin = MEMS_MOSI_Pin, .pin_name = "A7" }

// PA6: MEMS_MISO (IN) -- {}
#define MEMS_MISO_GPIO							GPIOA
#define MEMS_MISO_PinSource						GPIO_PinSource6
#define MEMS_MISO_Pin							GPIO_Pin_6
#define MEMS_MISO_set_HIGH()					MEMS_MISO_GPIO->BSRRL = MEMS_MISO_Pin
#define MEMS_MISO_set_LOW()						MEMS_MISO_GPIO->BSRRH = MEMS_MISO_Pin
#define MEMS_MISO_set_ACTIVE()					MEMS_MISO_set_HIGH()
#define MEMS_MISO_set_INACTIVE()				MEMS_MISO_set_LOW()
#define MEMS_MISO_get()							(MEMS_MISO_GPIO->IDR & MEMS_MISO_Pin)
#define MEMS_MISO_set(value)					if (value) { MEMS_MISO_set_ACTIVE(); } else { MEMS_MISO_set_INACTIVE(); }
#define MEMS_MISO_toggle()						MEMS_MISO_GPIO->ODR ^= MEMS_MISO_Pin
#define MEMS_MISO_pulse()						do { MEMS_MISO_set_ACTIVE(); delay_loopcnt(LOOPCOUNT_50NS); MEMS_MISO_set_INACTIVE(); } while (0)
#define MEMS_MISO_npulse()						do { MEMS_MISO_set_INACTIVE(); delay_loopcnt(LOOPCOUNT_50NS); MEMS_MISO_set_ACTIVE(); } while (0)
#define MEMS_MISO_GPIO_Definition				{ .gpio = MEMS_MISO_GPIO, .name = "MEMS_MISO", .pin = MEMS_MISO_Pin, .pin_name = "A6" }

// PA5: MEMS_SCK (OUT) -- {}
#define MEMS_SCK_GPIO							GPIOA
#define MEMS_SCK_PinSource						GPIO_PinSource5
#define MEMS_SCK_Pin							GPIO_Pin_5
#define MEMS_SCK_set_HIGH()						MEMS_SCK_GPIO->BSRRL = MEMS_SCK_Pin
#define MEMS_SCK_set_LOW()						MEMS_SCK_GPIO->BSRRH = MEMS_SCK_Pin
#define MEMS_SCK_set_ACTIVE()					MEMS_SCK_set_HIGH()
#define MEMS_SCK_set_INACTIVE()					MEMS_SCK_set_LOW()
#define MEMS_SCK_get()							(MEMS_SCK_GPIO->IDR & MEMS_SCK_Pin)
#define MEMS_SCK_set(value)						if (value) { MEMS_SCK_set_ACTIVE(); } else { MEMS_SCK_set_INACTIVE(); }
#define MEMS_SCK_toggle()						MEMS_SCK_GPIO->ODR ^= MEMS_SCK_Pin
#define MEMS_SCK_pulse()						do { MEMS_SCK_set_ACTIVE(); delay_loopcnt(LOOPCOUNT_50NS); MEMS_SCK_set_INACTIVE(); } while (0)
#define MEMS_SCK_npulse()						do { MEMS_SCK_set_INACTIVE(); delay_loopcnt(LOOPCOUNT_50NS); MEMS_SCK_set_ACTIVE(); } while (0)
#define MEMS_SCK_GPIO_Definition				{ .gpio = MEMS_SCK_GPIO, .name = "MEMS_SCK", .pin = MEMS_SCK_Pin, .pin_name = "A5" }

// PE3: MEMS_I2C_SPI (OUT) -- {}
#define MEMS_I2C_SPI_GPIO						GPIOE
#define MEMS_I2C_SPI_PinSource					GPIO_PinSource3
#define MEMS_I2C_SPI_Pin						GPIO_Pin_3
#define MEMS_I2C_SPI_set_HIGH()					MEMS_I2C_SPI_GPIO->BSRRL = MEMS_I2C_SPI_Pin
#define MEMS_I2C_SPI_set_LOW()					MEMS_I2C_SPI_GPIO->BSRRH = MEMS_I2C_SPI_Pin
#define MEMS_I2C_SPI_set_ACTIVE()				MEMS_I2C_SPI_set_HIGH()
#define MEMS_I2C_SPI_set_INACTIVE()				MEMS_I2C_SPI_set_LOW()
#define MEMS_I2C_SPI_get()						(MEMS_I2C_SPI_GPIO->IDR & MEMS_I2C_SPI_Pin)
#define MEMS_I2C_SPI_set(value)					if (value) { MEMS_I2C_SPI_set_ACTIVE(); } else { MEMS_I2C_SPI_set_INACTIVE(); }
#define MEMS_I2C_SPI_toggle()					MEMS_I2C_SPI_GPIO->ODR ^= MEMS_I2C_SPI_Pin
#define MEMS_I2C_SPI_pulse()					do { MEMS_I2C_SPI_set_ACTIVE(); delay_loopcnt(LOOPCOUNT_50NS); MEMS_I2C_SPI_set_INACTIVE(); } while (0)
#define MEMS_I2C_SPI_npulse()					do { MEMS_I2C_SPI_set_INACTIVE(); delay_loopcnt(LOOPCOUNT_50NS); MEMS_I2C_SPI_set_ACTIVE(); } while (0)
#define MEMS_I2C_SPI_GPIO_Definition			{ .gpio = MEMS_I2C_SPI_GPIO, .name = "MEMS_I2C_SPI", .pin = MEMS_I2C_SPI_Pin, .pin_name = "E3" }

// PD4: AudioReset (OUT) -- {}
#define AudioReset_GPIO							GPIOD
#define AudioReset_PinSource					GPIO_PinSource4
#define AudioReset_Pin							GPIO_Pin_4
#define AudioReset_set_HIGH()					AudioReset_GPIO->BSRRL = AudioReset_Pin
#define AudioReset_set_LOW()					AudioReset_GPIO->BSRRH = AudioReset_Pin
#define AudioReset_set_ACTIVE()					AudioReset_set_HIGH()
#define AudioReset_set_INACTIVE()				AudioReset_set_LOW()
#define AudioReset_get()						(AudioReset_GPIO->IDR & AudioReset_Pin)
#define AudioReset_set(value)					if (value) { AudioReset_set_ACTIVE(); } else { AudioReset_set_INACTIVE(); }
#define AudioReset_toggle()						AudioReset_GPIO->ODR ^= AudioReset_Pin
#define AudioReset_pulse()						do { AudioReset_set_ACTIVE(); delay_loopcnt(LOOPCOUNT_50NS); AudioReset_set_INACTIVE(); } while (0)
#define AudioReset_npulse()						do { AudioReset_set_INACTIVE(); delay_loopcnt(LOOPCOUNT_50NS); AudioReset_set_ACTIVE(); } while (0)
#define AudioReset_GPIO_Definition				{ .gpio = AudioReset_GPIO, .name = "AudioReset", .pin = AudioReset_Pin, .pin_name = "D4" }
// PD4: Display9_CS (OUT) -- {'activelow': True, 'speed': '25', 'init': True, 'connect': 'Display 15'}
#define Display9_CS_GPIO						GPIOD
#define Display9_CS_PinSource					GPIO_PinSource4
#define Display9_CS_Pin							GPIO_Pin_4
#define Display9_CS_set_HIGH()					Display9_CS_GPIO->BSRRL = Display9_CS_Pin
#define Display9_CS_set_LOW()					Display9_CS_GPIO->BSRRH = Display9_CS_Pin
#define Display9_CS_set_ACTIVE()				Display9_CS_set_LOW()
#define Display9_CS_set_INACTIVE()				Display9_CS_set_HIGH()
#define Display9_CS_get()						(!(Display9_CS_GPIO->IDR & Display9_CS_Pin))
#define Display9_CS_set(value)					if (value) { Display9_CS_set_ACTIVE(); } else { Display9_CS_set_INACTIVE(); }
#define Display9_CS_toggle()					Display9_CS_GPIO->ODR ^= Display9_CS_Pin
#define Display9_CS_pulse()						do { Display9_CS_set_ACTIVE(); delay_loopcnt(LOOPCOUNT_50NS); Display9_CS_set_INACTIVE(); } while (0)
#define Display9_CS_npulse()					do { Display9_CS_set_INACTIVE(); delay_loopcnt(LOOPCOUNT_50NS); Display9_CS_set_ACTIVE(); } while (0)
#define Display9_CS_GPIO_Definition				{ .connect = "Display 15", .gpio = Display9_CS_GPIO, .name = "Display9_CS", .pin = Display9_CS_Pin, .pin_name = "D4" }

// PC3: AudioPDM_OUT (IN) -- {}
#define AudioPDM_OUT_GPIO						GPIOC
#define AudioPDM_OUT_PinSource					GPIO_PinSource3
#define AudioPDM_OUT_Pin						GPIO_Pin_3
#define AudioPDM_OUT_set_HIGH()					AudioPDM_OUT_GPIO->BSRRL = AudioPDM_OUT_Pin
#define AudioPDM_OUT_set_LOW()					AudioPDM_OUT_GPIO->BSRRH = AudioPDM_OUT_Pin
#define AudioPDM_OUT_set_ACTIVE()				AudioPDM_OUT_set_HIGH()
#define AudioPDM_OUT_set_INACTIVE()				AudioPDM_OUT_set_LOW()
#define AudioPDM_OUT_get()						(AudioPDM_OUT_GPIO->IDR & AudioPDM_OUT_Pin)
#define AudioPDM_OUT_set(value)					if (value) { AudioPDM_OUT_set_ACTIVE(); } else { AudioPDM_OUT_set_INACTIVE(); }
#define AudioPDM_OUT_toggle()					AudioPDM_OUT_GPIO->ODR ^= AudioPDM_OUT_Pin
#define AudioPDM_OUT_pulse()					do { AudioPDM_OUT_set_ACTIVE(); delay_loopcnt(LOOPCOUNT_50NS); AudioPDM_OUT_set_INACTIVE(); } while (0)
#define AudioPDM_OUT_npulse()					do { AudioPDM_OUT_set_INACTIVE(); delay_loopcnt(LOOPCOUNT_50NS); AudioPDM_OUT_set_ACTIVE(); } while (0)
#define AudioPDM_OUT_GPIO_Definition			{ .gpio = AudioPDM_OUT_GPIO, .name = "AudioPDM_OUT", .pin = AudioPDM_OUT_Pin, .pin_name = "C3" }

// PC7: Audio_MCK (?) -- {}
#define Audio_MCK_GPIO							GPIOC
#define Audio_MCK_PinSource						GPIO_PinSource7
#define Audio_MCK_Pin							GPIO_Pin_7
#define Audio_MCK_set_HIGH()					Audio_MCK_GPIO->BSRRL = Audio_MCK_Pin
#define Audio_MCK_set_LOW()						Audio_MCK_GPIO->BSRRH = Audio_MCK_Pin
#define Audio_MCK_set_ACTIVE()					Audio_MCK_set_HIGH()
#define Audio_MCK_set_INACTIVE()				Audio_MCK_set_LOW()
#define Audio_MCK_get()							(Audio_MCK_GPIO->IDR & Audio_MCK_Pin)
#define Audio_MCK_set(value)					if (value) { Audio_MCK_set_ACTIVE(); } else { Audio_MCK_set_INACTIVE(); }
#define Audio_MCK_toggle()						Audio_MCK_GPIO->ODR ^= Audio_MCK_Pin
#define Audio_MCK_pulse()						do { Audio_MCK_set_ACTIVE(); delay_loopcnt(LOOPCOUNT_50NS); Audio_MCK_set_INACTIVE(); } while (0)
#define Audio_MCK_npulse()						do { Audio_MCK_set_INACTIVE(); delay_loopcnt(LOOPCOUNT_50NS); Audio_MCK_set_ACTIVE(); } while (0)
#define Audio_MCK_GPIO_Definition				{ .gpio = Audio_MCK_GPIO, .name = "Audio_MCK", .pin = Audio_MCK_Pin, .pin_name = "C7" }

// PC10: Audio_SCK (?) -- {}
#define Audio_SCK_GPIO							GPIOC
#define Audio_SCK_PinSource						GPIO_PinSource10
#define Audio_SCK_Pin							GPIO_Pin_10
#define Audio_SCK_set_HIGH()					Audio_SCK_GPIO->BSRRL = Audio_SCK_Pin
#define Audio_SCK_set_LOW()						Audio_SCK_GPIO->BSRRH = Audio_SCK_Pin
#define Audio_SCK_set_ACTIVE()					Audio_SCK_set_HIGH()
#define Audio_SCK_set_INACTIVE()				Audio_SCK_set_LOW()
#define Audio_SCK_get()							(Audio_SCK_GPIO->IDR & Audio_SCK_Pin)
#define Audio_SCK_set(value)					if (value) { Audio_SCK_set_ACTIVE(); } else { Audio_SCK_set_INACTIVE(); }
#define Audio_SCK_toggle()						Audio_SCK_GPIO->ODR ^= Audio_SCK_Pin
#define Audio_SCK_pulse()						do { Audio_SCK_set_ACTIVE(); delay_loopcnt(LOOPCOUNT_50NS); Audio_SCK_set_INACTIVE(); } while (0)
#define Audio_SCK_npulse()						do { Audio_SCK_set_INACTIVE(); delay_loopcnt(LOOPCOUNT_50NS); Audio_SCK_set_ACTIVE(); } while (0)
#define Audio_SCK_GPIO_Definition				{ .gpio = Audio_SCK_GPIO, .name = "Audio_SCK", .pin = Audio_SCK_Pin, .pin_name = "C10" }
// PC10: IOMux_SCK (OUT) -- {'af': 'SPI3', 'speed': '25', 'init': True, 'connect': 'IOMux 3'}
#define IOMux_SCK_GPIO							GPIOC
#define IOMux_SCK_PinSource						GPIO_PinSource10
#define IOMux_SCK_Pin							GPIO_Pin_10
#define IOMux_SCK_set_HIGH()					IOMux_SCK_GPIO->BSRRL = IOMux_SCK_Pin
#define IOMux_SCK_set_LOW()						IOMux_SCK_GPIO->BSRRH = IOMux_SCK_Pin
#define IOMux_SCK_set_ACTIVE()					IOMux_SCK_set_HIGH()
#define IOMux_SCK_set_INACTIVE()				IOMux_SCK_set_LOW()
#define IOMux_SCK_get()							(IOMux_SCK_GPIO->IDR & IOMux_SCK_Pin)
#define IOMux_SCK_set(value)					if (value) { IOMux_SCK_set_ACTIVE(); } else { IOMux_SCK_set_INACTIVE(); }
#define IOMux_SCK_toggle()						IOMux_SCK_GPIO->ODR ^= IOMux_SCK_Pin
#define IOMux_SCK_pulse()						do { IOMux_SCK_set_ACTIVE(); delay_loopcnt(LOOPCOUNT_50NS); IOMux_SCK_set_INACTIVE(); } while (0)
#define IOMux_SCK_npulse()						do { IOMux_SCK_set_INACTIVE(); delay_loopcnt(LOOPCOUNT_50NS); IOMux_SCK_set_ACTIVE(); } while (0)
#define IOMux_SCK_GPIO_Definition				{ .connect = "IOMux 3", .gpio = IOMux_SCK_GPIO, .name = "IOMux_SCK", .pin = IOMux_SCK_Pin, .pin_name = "C10" }

// PC12: Audio_SD (?) -- {}
#define Audio_SD_GPIO							GPIOC
#define Audio_SD_PinSource						GPIO_PinSource12
#define Audio_SD_Pin							GPIO_Pin_12
#define Audio_SD_set_HIGH()						Audio_SD_GPIO->BSRRL = Audio_SD_Pin
#define Audio_SD_set_LOW()						Audio_SD_GPIO->BSRRH = Audio_SD_Pin
#define Audio_SD_set_ACTIVE()					Audio_SD_set_HIGH()
#define Audio_SD_set_INACTIVE()					Audio_SD_set_LOW()
#define Audio_SD_get()							(Audio_SD_GPIO->IDR & Audio_SD_Pin)
#define Audio_SD_set(value)						if (value) { Audio_SD_set_ACTIVE(); } else { Audio_SD_set_INACTIVE(); }
#define Audio_SD_toggle()						Audio_SD_GPIO->ODR ^= Audio_SD_Pin
#define Audio_SD_pulse()						do { Audio_SD_set_ACTIVE(); delay_loopcnt(LOOPCOUNT_50NS); Audio_SD_set_INACTIVE(); } while (0)
#define Audio_SD_npulse()						do { Audio_SD_set_INACTIVE(); delay_loopcnt(LOOPCOUNT_50NS); Audio_SD_set_ACTIVE(); } while (0)
#define Audio_SD_GPIO_Definition				{ .gpio = Audio_SD_GPIO, .name = "Audio_SD", .pin = Audio_SD_Pin, .pin_name = "C12" }
// PC12: IOMux_MOSI (OUT) -- {'af': 'SPI3', 'speed': '25', 'init': True, 'connect': 'IOMux 7'}
#define IOMux_MOSI_GPIO							GPIOC
#define IOMux_MOSI_PinSource					GPIO_PinSource12
#define IOMux_MOSI_Pin							GPIO_Pin_12
#define IOMux_MOSI_set_HIGH()					IOMux_MOSI_GPIO->BSRRL = IOMux_MOSI_Pin
#define IOMux_MOSI_set_LOW()					IOMux_MOSI_GPIO->BSRRH = IOMux_MOSI_Pin
#define IOMux_MOSI_set_ACTIVE()					IOMux_MOSI_set_HIGH()
#define IOMux_MOSI_set_INACTIVE()				IOMux_MOSI_set_LOW()
#define IOMux_MOSI_get()						(IOMux_MOSI_GPIO->IDR & IOMux_MOSI_Pin)
#define IOMux_MOSI_set(value)					if (value) { IOMux_MOSI_set_ACTIVE(); } else { IOMux_MOSI_set_INACTIVE(); }
#define IOMux_MOSI_toggle()						IOMux_MOSI_GPIO->ODR ^= IOMux_MOSI_Pin
#define IOMux_MOSI_pulse()						do { IOMux_MOSI_set_ACTIVE(); delay_loopcnt(LOOPCOUNT_50NS); IOMux_MOSI_set_INACTIVE(); } while (0)
#define IOMux_MOSI_npulse()						do { IOMux_MOSI_set_INACTIVE(); delay_loopcnt(LOOPCOUNT_50NS); IOMux_MOSI_set_ACTIVE(); } while (0)
#define IOMux_MOSI_GPIO_Definition				{ .connect = "IOMux 7", .gpio = IOMux_MOSI_GPIO, .name = "IOMux_MOSI", .pin = IOMux_MOSI_Pin, .pin_name = "C12" }

// PA4: Audio_WS (?) -- {}
#define Audio_WS_GPIO							GPIOA
#define Audio_WS_PinSource						GPIO_PinSource4
#define Audio_WS_Pin							GPIO_Pin_4
#define Audio_WS_set_HIGH()						Audio_WS_GPIO->BSRRL = Audio_WS_Pin
#define Audio_WS_set_LOW()						Audio_WS_GPIO->BSRRH = Audio_WS_Pin
#define Audio_WS_set_ACTIVE()					Audio_WS_set_HIGH()
#define Audio_WS_set_INACTIVE()					Audio_WS_set_LOW()
#define Audio_WS_get()							(Audio_WS_GPIO->IDR & Audio_WS_Pin)
#define Audio_WS_set(value)						if (value) { Audio_WS_set_ACTIVE(); } else { Audio_WS_set_INACTIVE(); }
#define Audio_WS_toggle()						Audio_WS_GPIO->ODR ^= Audio_WS_Pin
#define Audio_WS_pulse()						do { Audio_WS_set_ACTIVE(); delay_loopcnt(LOOPCOUNT_50NS); Audio_WS_set_INACTIVE(); } while (0)
#define Audio_WS_npulse()						do { Audio_WS_set_INACTIVE(); delay_loopcnt(LOOPCOUNT_50NS); Audio_WS_set_ACTIVE(); } while (0)
#define Audio_WS_GPIO_Definition				{ .gpio = Audio_WS_GPIO, .name = "Audio_WS", .pin = Audio_WS_Pin, .pin_name = "A4" }

// PB6: Audio_SCL (IO) -- {}
#define Audio_SCL_GPIO							GPIOB
#define Audio_SCL_PinSource						GPIO_PinSource6
#define Audio_SCL_Pin							GPIO_Pin_6
#define Audio_SCL_set_HIGH()					Audio_SCL_GPIO->BSRRL = Audio_SCL_Pin
#define Audio_SCL_set_LOW()						Audio_SCL_GPIO->BSRRH = Audio_SCL_Pin
#define Audio_SCL_set_ACTIVE()					Audio_SCL_set_HIGH()
#define Audio_SCL_set_INACTIVE()				Audio_SCL_set_LOW()
#define Audio_SCL_get()							(Audio_SCL_GPIO->IDR & Audio_SCL_Pin)
#define Audio_SCL_set(value)					if (value) { Audio_SCL_set_ACTIVE(); } else { Audio_SCL_set_INACTIVE(); }
#define Audio_SCL_toggle()						Audio_SCL_GPIO->ODR ^= Audio_SCL_Pin
#define Audio_SCL_pulse()						do { Audio_SCL_set_ACTIVE(); delay_loopcnt(LOOPCOUNT_50NS); Audio_SCL_set_INACTIVE(); } while (0)
#define Audio_SCL_npulse()						do { Audio_SCL_set_INACTIVE(); delay_loopcnt(LOOPCOUNT_50NS); Audio_SCL_set_ACTIVE(); } while (0)
#define Audio_SCL_GPIO_Definition				{ .gpio = Audio_SCL_GPIO, .name = "Audio_SCL", .pin = Audio_SCL_Pin, .pin_name = "B6" }
// PB6: EEPROM_SCL (IO) -- {'af': 'I2C1', 'init': True}
#define EEPROM_SCL_GPIO							GPIOB
#define EEPROM_SCL_PinSource					GPIO_PinSource6
#define EEPROM_SCL_Pin							GPIO_Pin_6
#define EEPROM_SCL_set_HIGH()					EEPROM_SCL_GPIO->BSRRL = EEPROM_SCL_Pin
#define EEPROM_SCL_set_LOW()					EEPROM_SCL_GPIO->BSRRH = EEPROM_SCL_Pin
#define EEPROM_SCL_set_ACTIVE()					EEPROM_SCL_set_HIGH()
#define EEPROM_SCL_set_INACTIVE()				EEPROM_SCL_set_LOW()
#define EEPROM_SCL_get()						(EEPROM_SCL_GPIO->IDR & EEPROM_SCL_Pin)
#define EEPROM_SCL_set(value)					if (value) { EEPROM_SCL_set_ACTIVE(); } else { EEPROM_SCL_set_INACTIVE(); }
#define EEPROM_SCL_toggle()						EEPROM_SCL_GPIO->ODR ^= EEPROM_SCL_Pin
#define EEPROM_SCL_pulse()						do { EEPROM_SCL_set_ACTIVE(); delay_loopcnt(LOOPCOUNT_50NS); EEPROM_SCL_set_INACTIVE(); } while (0)
#define EEPROM_SCL_npulse()						do { EEPROM_SCL_set_INACTIVE(); delay_loopcnt(LOOPCOUNT_50NS); EEPROM_SCL_set_ACTIVE(); } while (0)
#define EEPROM_SCL_GPIO_Definition				{ .gpio = EEPROM_SCL_GPIO, .name = "EEPROM_SCL", .pin = EEPROM_SCL_Pin, .pin_name = "B6" }

// PB9: Audio_SDA (IO) -- {}
#define Audio_SDA_GPIO							GPIOB
#define Audio_SDA_PinSource						GPIO_PinSource9
#define Audio_SDA_Pin							GPIO_Pin_9
#define Audio_SDA_set_HIGH()					Audio_SDA_GPIO->BSRRL = Audio_SDA_Pin
#define Audio_SDA_set_LOW()						Audio_SDA_GPIO->BSRRH = Audio_SDA_Pin
#define Audio_SDA_set_ACTIVE()					Audio_SDA_set_HIGH()
#define Audio_SDA_set_INACTIVE()				Audio_SDA_set_LOW()
#define Audio_SDA_get()							(Audio_SDA_GPIO->IDR & Audio_SDA_Pin)
#define Audio_SDA_set(value)					if (value) { Audio_SDA_set_ACTIVE(); } else { Audio_SDA_set_INACTIVE(); }
#define Audio_SDA_toggle()						Audio_SDA_GPIO->ODR ^= Audio_SDA_Pin
#define Audio_SDA_pulse()						do { Audio_SDA_set_ACTIVE(); delay_loopcnt(LOOPCOUNT_50NS); Audio_SDA_set_INACTIVE(); } while (0)
#define Audio_SDA_npulse()						do { Audio_SDA_set_INACTIVE(); delay_loopcnt(LOOPCOUNT_50NS); Audio_SDA_set_ACTIVE(); } while (0)
#define Audio_SDA_GPIO_Definition				{ .gpio = Audio_SDA_GPIO, .name = "Audio_SDA", .pin = Audio_SDA_Pin, .pin_name = "B9" }
// PB9: EEPROM_SDA (IO) -- {'af': 'I2C1', 'init': True}
#define EEPROM_SDA_GPIO							GPIOB
#define EEPROM_SDA_PinSource					GPIO_PinSource9
#define EEPROM_SDA_Pin							GPIO_Pin_9
#define EEPROM_SDA_set_HIGH()					EEPROM_SDA_GPIO->BSRRL = EEPROM_SDA_Pin
#define EEPROM_SDA_set_LOW()					EEPROM_SDA_GPIO->BSRRH = EEPROM_SDA_Pin
#define EEPROM_SDA_set_ACTIVE()					EEPROM_SDA_set_HIGH()
#define EEPROM_SDA_set_INACTIVE()				EEPROM_SDA_set_LOW()
#define EEPROM_SDA_get()						(EEPROM_SDA_GPIO->IDR & EEPROM_SDA_Pin)
#define EEPROM_SDA_set(value)					if (value) { EEPROM_SDA_set_ACTIVE(); } else { EEPROM_SDA_set_INACTIVE(); }
#define EEPROM_SDA_toggle()						EEPROM_SDA_GPIO->ODR ^= EEPROM_SDA_Pin
#define EEPROM_SDA_pulse()						do { EEPROM_SDA_set_ACTIVE(); delay_loopcnt(LOOPCOUNT_50NS); EEPROM_SDA_set_INACTIVE(); } while (0)
#define EEPROM_SDA_npulse()						do { EEPROM_SDA_set_INACTIVE(); delay_loopcnt(LOOPCOUNT_50NS); EEPROM_SDA_set_ACTIVE(); } while (0)
#define EEPROM_SDA_GPIO_Definition				{ .gpio = EEPROM_SDA_GPIO, .name = "EEPROM_SDA", .pin = EEPROM_SDA_Pin, .pin_name = "B9" }

// PB10: Audio_CLK_IN (OUT) -- {}
#define Audio_CLK_IN_GPIO						GPIOB
#define Audio_CLK_IN_PinSource					GPIO_PinSource10
#define Audio_CLK_IN_Pin						GPIO_Pin_10
#define Audio_CLK_IN_set_HIGH()					Audio_CLK_IN_GPIO->BSRRL = Audio_CLK_IN_Pin
#define Audio_CLK_IN_set_LOW()					Audio_CLK_IN_GPIO->BSRRH = Audio_CLK_IN_Pin
#define Audio_CLK_IN_set_ACTIVE()				Audio_CLK_IN_set_HIGH()
#define Audio_CLK_IN_set_INACTIVE()				Audio_CLK_IN_set_LOW()
#define Audio_CLK_IN_get()						(Audio_CLK_IN_GPIO->IDR & Audio_CLK_IN_Pin)
#define Audio_CLK_IN_set(value)					if (value) { Audio_CLK_IN_set_ACTIVE(); } else { Audio_CLK_IN_set_INACTIVE(); }
#define Audio_CLK_IN_toggle()					Audio_CLK_IN_GPIO->ODR ^= Audio_CLK_IN_Pin
#define Audio_CLK_IN_pulse()					do { Audio_CLK_IN_set_ACTIVE(); delay_loopcnt(LOOPCOUNT_50NS); Audio_CLK_IN_set_INACTIVE(); } while (0)
#define Audio_CLK_IN_npulse()					do { Audio_CLK_IN_set_INACTIVE(); delay_loopcnt(LOOPCOUNT_50NS); Audio_CLK_IN_set_ACTIVE(); } while (0)
#define Audio_CLK_IN_GPIO_Definition			{ .gpio = Audio_CLK_IN_GPIO, .name = "Audio_CLK_IN", .pin = Audio_CLK_IN_Pin, .pin_name = "B10" }

// PB13: Display_SCK (OUT) -- {'af': 'SPI2', 'speed': '25', 'init': True, 'connect': 'Display 3'}
#define Display_SCK_GPIO						GPIOB
#define Display_SCK_PinSource					GPIO_PinSource13
#define Display_SCK_Pin							GPIO_Pin_13
#define Display_SCK_set_HIGH()					Display_SCK_GPIO->BSRRL = Display_SCK_Pin
#define Display_SCK_set_LOW()					Display_SCK_GPIO->BSRRH = Display_SCK_Pin
#define Display_SCK_set_ACTIVE()				Display_SCK_set_HIGH()
#define Display_SCK_set_INACTIVE()				Display_SCK_set_LOW()
#define Display_SCK_get()						(Display_SCK_GPIO->IDR & Display_SCK_Pin)
#define Display_SCK_set(value)					if (value) { Display_SCK_set_ACTIVE(); } else { Display_SCK_set_INACTIVE(); }
#define Display_SCK_toggle()					Display_SCK_GPIO->ODR ^= Display_SCK_Pin
#define Display_SCK_pulse()						do { Display_SCK_set_ACTIVE(); delay_loopcnt(LOOPCOUNT_50NS); Display_SCK_set_INACTIVE(); } while (0)
#define Display_SCK_npulse()					do { Display_SCK_set_INACTIVE(); delay_loopcnt(LOOPCOUNT_50NS); Display_SCK_set_ACTIVE(); } while (0)
#define Display_SCK_GPIO_Definition				{ .connect = "Display 3", .gpio = Display_SCK_GPIO, .name = "Display_SCK", .pin = Display_SCK_Pin, .pin_name = "B13" }

// PB15: Display_MOSI (OUT) -- {'af': 'SPI2', 'speed': '25', 'init': True, 'connect': 'Display 4'}
#define Display_MOSI_GPIO						GPIOB
#define Display_MOSI_PinSource					GPIO_PinSource15
#define Display_MOSI_Pin						GPIO_Pin_15
#define Display_MOSI_set_HIGH()					Display_MOSI_GPIO->BSRRL = Display_MOSI_Pin
#define Display_MOSI_set_LOW()					Display_MOSI_GPIO->BSRRH = Display_MOSI_Pin
#define Display_MOSI_set_ACTIVE()				Display_MOSI_set_HIGH()
#define Display_MOSI_set_INACTIVE()				Display_MOSI_set_LOW()
#define Display_MOSI_get()						(Display_MOSI_GPIO->IDR & Display_MOSI_Pin)
#define Display_MOSI_set(value)					if (value) { Display_MOSI_set_ACTIVE(); } else { Display_MOSI_set_INACTIVE(); }
#define Display_MOSI_toggle()					Display_MOSI_GPIO->ODR ^= Display_MOSI_Pin
#define Display_MOSI_pulse()					do { Display_MOSI_set_ACTIVE(); delay_loopcnt(LOOPCOUNT_50NS); Display_MOSI_set_INACTIVE(); } while (0)
#define Display_MOSI_npulse()					do { Display_MOSI_set_INACTIVE(); delay_loopcnt(LOOPCOUNT_50NS); Display_MOSI_set_ACTIVE(); } while (0)
#define Display_MOSI_GPIO_Definition			{ .connect = "Display 4", .gpio = Display_MOSI_GPIO, .name = "Display_MOSI", .pin = Display_MOSI_Pin, .pin_name = "B15" }

// PD1: Display_RESET (OUT) -- {'activelow': True, 'init': True, 'connect': 'Display 5'}
#define Display_RESET_GPIO						GPIOD
#define Display_RESET_PinSource					GPIO_PinSource1
#define Display_RESET_Pin						GPIO_Pin_1
#define Display_RESET_set_HIGH()				Display_RESET_GPIO->BSRRL = Display_RESET_Pin
#define Display_RESET_set_LOW()					Display_RESET_GPIO->BSRRH = Display_RESET_Pin
#define Display_RESET_set_ACTIVE()				Display_RESET_set_LOW()
#define Display_RESET_set_INACTIVE()			Display_RESET_set_HIGH()
#define Display_RESET_get()						(!(Display_RESET_GPIO->IDR & Display_RESET_Pin))
#define Display_RESET_set(value)				if (value) { Display_RESET_set_ACTIVE(); } else { Display_RESET_set_INACTIVE(); }
#define Display_RESET_toggle()					Display_RESET_GPIO->ODR ^= Display_RESET_Pin
#define Display_RESET_pulse()					do { Display_RESET_set_ACTIVE(); delay_loopcnt(LOOPCOUNT_50NS); Display_RESET_set_INACTIVE(); } while (0)
#define Display_RESET_npulse()					do { Display_RESET_set_INACTIVE(); delay_loopcnt(LOOPCOUNT_50NS); Display_RESET_set_ACTIVE(); } while (0)
#define Display_RESET_GPIO_Definition			{ .connect = "Display 5", .gpio = Display_RESET_GPIO, .name = "Display_RESET", .pin = Display_RESET_Pin, .pin_name = "D1" }

// PD2: Display_DC (OUT) -- {'connect': 'Display 6', 'comment': 'Data/!Control', 'init': True}
#define Display_DC_GPIO							GPIOD
#define Display_DC_PinSource					GPIO_PinSource2
#define Display_DC_Pin							GPIO_Pin_2
#define Display_DC_set_HIGH()					Display_DC_GPIO->BSRRL = Display_DC_Pin
#define Display_DC_set_LOW()					Display_DC_GPIO->BSRRH = Display_DC_Pin
#define Display_DC_set_ACTIVE()					Display_DC_set_HIGH()
#define Display_DC_set_INACTIVE()				Display_DC_set_LOW()
#define Display_DC_get()						(Display_DC_GPIO->IDR & Display_DC_Pin)
#define Display_DC_set(value)					if (value) { Display_DC_set_ACTIVE(); } else { Display_DC_set_INACTIVE(); }
#define Display_DC_toggle()						Display_DC_GPIO->ODR ^= Display_DC_Pin
#define Display_DC_pulse()						do { Display_DC_set_ACTIVE(); delay_loopcnt(LOOPCOUNT_50NS); Display_DC_set_INACTIVE(); } while (0)
#define Display_DC_npulse()						do { Display_DC_set_INACTIVE(); delay_loopcnt(LOOPCOUNT_50NS); Display_DC_set_ACTIVE(); } while (0)
#define Display_DC_GPIO_Definition				{ .comment = "Data/!Control", .connect = "Display 6", .gpio = Display_DC_GPIO, .name = "Display_DC", .pin = Display_DC_Pin, .pin_name = "D2" }

// PB0: Display1_CS (OUT) -- {'activelow': True, 'speed': '25', 'init': True, 'connect': 'Display 7'}
#define Display1_CS_GPIO						GPIOB
#define Display1_CS_PinSource					GPIO_PinSource0
#define Display1_CS_Pin							GPIO_Pin_0
#define Display1_CS_set_HIGH()					Display1_CS_GPIO->BSRRL = Display1_CS_Pin
#define Display1_CS_set_LOW()					Display1_CS_GPIO->BSRRH = Display1_CS_Pin
#define Display1_CS_set_ACTIVE()				Display1_CS_set_LOW()
#define Display1_CS_set_INACTIVE()				Display1_CS_set_HIGH()
#define Display1_CS_get()						(!(Display1_CS_GPIO->IDR & Display1_CS_Pin))
#define Display1_CS_set(value)					if (value) { Display1_CS_set_ACTIVE(); } else { Display1_CS_set_INACTIVE(); }
#define Display1_CS_toggle()					Display1_CS_GPIO->ODR ^= Display1_CS_Pin
#define Display1_CS_pulse()						do { Display1_CS_set_ACTIVE(); delay_loopcnt(LOOPCOUNT_50NS); Display1_CS_set_INACTIVE(); } while (0)
#define Display1_CS_npulse()					do { Display1_CS_set_INACTIVE(); delay_loopcnt(LOOPCOUNT_50NS); Display1_CS_set_ACTIVE(); } while (0)
#define Display1_CS_GPIO_Definition				{ .connect = "Display 7", .gpio = Display1_CS_GPIO, .name = "Display1_CS", .pin = Display1_CS_Pin, .pin_name = "B0" }

// PB1: Display2_CS (OUT) -- {'activelow': True, 'speed': '25', 'init': True, 'connect': 'Display 8'}
#define Display2_CS_GPIO						GPIOB
#define Display2_CS_PinSource					GPIO_PinSource1
#define Display2_CS_Pin							GPIO_Pin_1
#define Display2_CS_set_HIGH()					Display2_CS_GPIO->BSRRL = Display2_CS_Pin
#define Display2_CS_set_LOW()					Display2_CS_GPIO->BSRRH = Display2_CS_Pin
#define Display2_CS_set_ACTIVE()				Display2_CS_set_LOW()
#define Display2_CS_set_INACTIVE()				Display2_CS_set_HIGH()
#define Display2_CS_get()						(!(Display2_CS_GPIO->IDR & Display2_CS_Pin))
#define Display2_CS_set(value)					if (value) { Display2_CS_set_ACTIVE(); } else { Display2_CS_set_INACTIVE(); }
#define Display2_CS_toggle()					Display2_CS_GPIO->ODR ^= Display2_CS_Pin
#define Display2_CS_pulse()						do { Display2_CS_set_ACTIVE(); delay_loopcnt(LOOPCOUNT_50NS); Display2_CS_set_INACTIVE(); } while (0)
#define Display2_CS_npulse()					do { Display2_CS_set_INACTIVE(); delay_loopcnt(LOOPCOUNT_50NS); Display2_CS_set_ACTIVE(); } while (0)
#define Display2_CS_GPIO_Definition				{ .connect = "Display 8", .gpio = Display2_CS_GPIO, .name = "Display2_CS", .pin = Display2_CS_Pin, .pin_name = "B1" }

// PB2: Display3_CS (OUT) -- {'activelow': True, 'speed': '25', 'init': True, 'connect': 'Display 9'}
#define Display3_CS_GPIO						GPIOB
#define Display3_CS_PinSource					GPIO_PinSource2
#define Display3_CS_Pin							GPIO_Pin_2
#define Display3_CS_set_HIGH()					Display3_CS_GPIO->BSRRL = Display3_CS_Pin
#define Display3_CS_set_LOW()					Display3_CS_GPIO->BSRRH = Display3_CS_Pin
#define Display3_CS_set_ACTIVE()				Display3_CS_set_LOW()
#define Display3_CS_set_INACTIVE()				Display3_CS_set_HIGH()
#define Display3_CS_get()						(!(Display3_CS_GPIO->IDR & Display3_CS_Pin))
#define Display3_CS_set(value)					if (value) { Display3_CS_set_ACTIVE(); } else { Display3_CS_set_INACTIVE(); }
#define Display3_CS_toggle()					Display3_CS_GPIO->ODR ^= Display3_CS_Pin
#define Display3_CS_pulse()						do { Display3_CS_set_ACTIVE(); delay_loopcnt(LOOPCOUNT_50NS); Display3_CS_set_INACTIVE(); } while (0)
#define Display3_CS_npulse()					do { Display3_CS_set_INACTIVE(); delay_loopcnt(LOOPCOUNT_50NS); Display3_CS_set_ACTIVE(); } while (0)
#define Display3_CS_GPIO_Definition				{ .connect = "Display 9", .gpio = Display3_CS_GPIO, .name = "Display3_CS", .pin = Display3_CS_Pin, .pin_name = "B2" }

// PB3: Display4_CS (OUT) -- {'activelow': True, 'speed': '25', 'init': True, 'connect': 'Display 10'}
#define Display4_CS_GPIO						GPIOB
#define Display4_CS_PinSource					GPIO_PinSource3
#define Display4_CS_Pin							GPIO_Pin_3
#define Display4_CS_set_HIGH()					Display4_CS_GPIO->BSRRL = Display4_CS_Pin
#define Display4_CS_set_LOW()					Display4_CS_GPIO->BSRRH = Display4_CS_Pin
#define Display4_CS_set_ACTIVE()				Display4_CS_set_LOW()
#define Display4_CS_set_INACTIVE()				Display4_CS_set_HIGH()
#define Display4_CS_get()						(!(Display4_CS_GPIO->IDR & Display4_CS_Pin))
#define Display4_CS_set(value)					if (value) { Display4_CS_set_ACTIVE(); } else { Display4_CS_set_INACTIVE(); }
#define Display4_CS_toggle()					Display4_CS_GPIO->ODR ^= Display4_CS_Pin
#define Display4_CS_pulse()						do { Display4_CS_set_ACTIVE(); delay_loopcnt(LOOPCOUNT_50NS); Display4_CS_set_INACTIVE(); } while (0)
#define Display4_CS_npulse()					do { Display4_CS_set_INACTIVE(); delay_loopcnt(LOOPCOUNT_50NS); Display4_CS_set_ACTIVE(); } while (0)
#define Display4_CS_GPIO_Definition				{ .connect = "Display 10", .gpio = Display4_CS_GPIO, .name = "Display4_CS", .pin = Display4_CS_Pin, .pin_name = "B3" }

// PB4: Display5_CS (OUT) -- {'activelow': True, 'speed': '25', 'init': True, 'connect': 'Display 11'}
#define Display5_CS_GPIO						GPIOB
#define Display5_CS_PinSource					GPIO_PinSource4
#define Display5_CS_Pin							GPIO_Pin_4
#define Display5_CS_set_HIGH()					Display5_CS_GPIO->BSRRL = Display5_CS_Pin
#define Display5_CS_set_LOW()					Display5_CS_GPIO->BSRRH = Display5_CS_Pin
#define Display5_CS_set_ACTIVE()				Display5_CS_set_LOW()
#define Display5_CS_set_INACTIVE()				Display5_CS_set_HIGH()
#define Display5_CS_get()						(!(Display5_CS_GPIO->IDR & Display5_CS_Pin))
#define Display5_CS_set(value)					if (value) { Display5_CS_set_ACTIVE(); } else { Display5_CS_set_INACTIVE(); }
#define Display5_CS_toggle()					Display5_CS_GPIO->ODR ^= Display5_CS_Pin
#define Display5_CS_pulse()						do { Display5_CS_set_ACTIVE(); delay_loopcnt(LOOPCOUNT_50NS); Display5_CS_set_INACTIVE(); } while (0)
#define Display5_CS_npulse()					do { Display5_CS_set_INACTIVE(); delay_loopcnt(LOOPCOUNT_50NS); Display5_CS_set_ACTIVE(); } while (0)
#define Display5_CS_GPIO_Definition				{ .connect = "Display 11", .gpio = Display5_CS_GPIO, .name = "Display5_CS", .pin = Display5_CS_Pin, .pin_name = "B4" }

// PB5: Display6_CS (OUT) -- {'activelow': True, 'speed': '25', 'init': True, 'connect': 'Display 12'}
#define Display6_CS_GPIO						GPIOB
#define Display6_CS_PinSource					GPIO_PinSource5
#define Display6_CS_Pin							GPIO_Pin_5
#define Display6_CS_set_HIGH()					Display6_CS_GPIO->BSRRL = Display6_CS_Pin
#define Display6_CS_set_LOW()					Display6_CS_GPIO->BSRRH = Display6_CS_Pin
#define Display6_CS_set_ACTIVE()				Display6_CS_set_LOW()
#define Display6_CS_set_INACTIVE()				Display6_CS_set_HIGH()
#define Display6_CS_get()						(!(Display6_CS_GPIO->IDR & Display6_CS_Pin))
#define Display6_CS_set(value)					if (value) { Display6_CS_set_ACTIVE(); } else { Display6_CS_set_INACTIVE(); }
#define Display6_CS_toggle()					Display6_CS_GPIO->ODR ^= Display6_CS_Pin
#define Display6_CS_pulse()						do { Display6_CS_set_ACTIVE(); delay_loopcnt(LOOPCOUNT_50NS); Display6_CS_set_INACTIVE(); } while (0)
#define Display6_CS_npulse()					do { Display6_CS_set_INACTIVE(); delay_loopcnt(LOOPCOUNT_50NS); Display6_CS_set_ACTIVE(); } while (0)
#define Display6_CS_GPIO_Definition				{ .connect = "Display 12", .gpio = Display6_CS_GPIO, .name = "Display6_CS", .pin = Display6_CS_Pin, .pin_name = "B5" }

// PB7: Display7_CS (OUT) -- {'activelow': True, 'speed': '25', 'init': True, 'connect': 'Display 13'}
#define Display7_CS_GPIO						GPIOB
#define Display7_CS_PinSource					GPIO_PinSource7
#define Display7_CS_Pin							GPIO_Pin_7
#define Display7_CS_set_HIGH()					Display7_CS_GPIO->BSRRL = Display7_CS_Pin
#define Display7_CS_set_LOW()					Display7_CS_GPIO->BSRRH = Display7_CS_Pin
#define Display7_CS_set_ACTIVE()				Display7_CS_set_LOW()
#define Display7_CS_set_INACTIVE()				Display7_CS_set_HIGH()
#define Display7_CS_get()						(!(Display7_CS_GPIO->IDR & Display7_CS_Pin))
#define Display7_CS_set(value)					if (value) { Display7_CS_set_ACTIVE(); } else { Display7_CS_set_INACTIVE(); }
#define Display7_CS_toggle()					Display7_CS_GPIO->ODR ^= Display7_CS_Pin
#define Display7_CS_pulse()						do { Display7_CS_set_ACTIVE(); delay_loopcnt(LOOPCOUNT_50NS); Display7_CS_set_INACTIVE(); } while (0)
#define Display7_CS_npulse()					do { Display7_CS_set_INACTIVE(); delay_loopcnt(LOOPCOUNT_50NS); Display7_CS_set_ACTIVE(); } while (0)
#define Display7_CS_GPIO_Definition				{ .connect = "Display 13", .gpio = Display7_CS_GPIO, .name = "Display7_CS", .pin = Display7_CS_Pin, .pin_name = "B7" }

// PD3: Display8_CS (OUT) -- {'activelow': True, 'speed': '25', 'init': True, 'connect': 'Display 14'}
#define Display8_CS_GPIO						GPIOD
#define Display8_CS_PinSource					GPIO_PinSource3
#define Display8_CS_Pin							GPIO_Pin_3
#define Display8_CS_set_HIGH()					Display8_CS_GPIO->BSRRL = Display8_CS_Pin
#define Display8_CS_set_LOW()					Display8_CS_GPIO->BSRRH = Display8_CS_Pin
#define Display8_CS_set_ACTIVE()				Display8_CS_set_LOW()
#define Display8_CS_set_INACTIVE()				Display8_CS_set_HIGH()
#define Display8_CS_get()						(!(Display8_CS_GPIO->IDR & Display8_CS_Pin))
#define Display8_CS_set(value)					if (value) { Display8_CS_set_ACTIVE(); } else { Display8_CS_set_INACTIVE(); }
#define Display8_CS_toggle()					Display8_CS_GPIO->ODR ^= Display8_CS_Pin
#define Display8_CS_pulse()						do { Display8_CS_set_ACTIVE(); delay_loopcnt(LOOPCOUNT_50NS); Display8_CS_set_INACTIVE(); } while (0)
#define Display8_CS_npulse()					do { Display8_CS_set_INACTIVE(); delay_loopcnt(LOOPCOUNT_50NS); Display8_CS_set_ACTIVE(); } while (0)
#define Display8_CS_GPIO_Definition				{ .connect = "Display 14", .gpio = Display8_CS_GPIO, .name = "Display8_CS", .pin = Display8_CS_Pin, .pin_name = "D3" }

// PD7: Display10_CS (OUT) -- {'activelow': True, 'speed': '25', 'init': True, 'connect': 'Display 16'}
#define Display10_CS_GPIO						GPIOD
#define Display10_CS_PinSource					GPIO_PinSource7
#define Display10_CS_Pin						GPIO_Pin_7
#define Display10_CS_set_HIGH()					Display10_CS_GPIO->BSRRL = Display10_CS_Pin
#define Display10_CS_set_LOW()					Display10_CS_GPIO->BSRRH = Display10_CS_Pin
#define Display10_CS_set_ACTIVE()				Display10_CS_set_LOW()
#define Display10_CS_set_INACTIVE()				Display10_CS_set_HIGH()
#define Display10_CS_get()						(!(Display10_CS_GPIO->IDR & Display10_CS_Pin))
#define Display10_CS_set(value)					if (value) { Display10_CS_set_ACTIVE(); } else { Display10_CS_set_INACTIVE(); }
#define Display10_CS_toggle()					Display10_CS_GPIO->ODR ^= Display10_CS_Pin
#define Display10_CS_pulse()					do { Display10_CS_set_ACTIVE(); delay_loopcnt(LOOPCOUNT_50NS); Display10_CS_set_INACTIVE(); } while (0)
#define Display10_CS_npulse()					do { Display10_CS_set_INACTIVE(); delay_loopcnt(LOOPCOUNT_50NS); Display10_CS_set_ACTIVE(); } while (0)
#define Display10_CS_GPIO_Definition			{ .connect = "Display 16", .gpio = Display10_CS_GPIO, .name = "Display10_CS", .pin = Display10_CS_Pin, .pin_name = "D7" }

// PD0: Display11_CS (OUT) -- {'activelow': True, 'speed': '25', 'init': True, 'connect': 'Display 17'}
#define Display11_CS_GPIO						GPIOD
#define Display11_CS_PinSource					GPIO_PinSource0
#define Display11_CS_Pin						GPIO_Pin_0
#define Display11_CS_set_HIGH()					Display11_CS_GPIO->BSRRL = Display11_CS_Pin
#define Display11_CS_set_LOW()					Display11_CS_GPIO->BSRRH = Display11_CS_Pin
#define Display11_CS_set_ACTIVE()				Display11_CS_set_LOW()
#define Display11_CS_set_INACTIVE()				Display11_CS_set_HIGH()
#define Display11_CS_get()						(!(Display11_CS_GPIO->IDR & Display11_CS_Pin))
#define Display11_CS_set(value)					if (value) { Display11_CS_set_ACTIVE(); } else { Display11_CS_set_INACTIVE(); }
#define Display11_CS_toggle()					Display11_CS_GPIO->ODR ^= Display11_CS_Pin
#define Display11_CS_pulse()					do { Display11_CS_set_ACTIVE(); delay_loopcnt(LOOPCOUNT_50NS); Display11_CS_set_INACTIVE(); } while (0)
#define Display11_CS_npulse()					do { Display11_CS_set_INACTIVE(); delay_loopcnt(LOOPCOUNT_50NS); Display11_CS_set_ACTIVE(); } while (0)
#define Display11_CS_GPIO_Definition			{ .connect = "Display 17", .gpio = Display11_CS_GPIO, .name = "Display11_CS", .pin = Display11_CS_Pin, .pin_name = "D0" }

// PD8: Display12_CS (OUT) -- {'activelow': True, 'speed': '25', 'init': True, 'connect': 'Display 18'}
#define Display12_CS_GPIO						GPIOD
#define Display12_CS_PinSource					GPIO_PinSource8
#define Display12_CS_Pin						GPIO_Pin_8
#define Display12_CS_set_HIGH()					Display12_CS_GPIO->BSRRL = Display12_CS_Pin
#define Display12_CS_set_LOW()					Display12_CS_GPIO->BSRRH = Display12_CS_Pin
#define Display12_CS_set_ACTIVE()				Display12_CS_set_LOW()
#define Display12_CS_set_INACTIVE()				Display12_CS_set_HIGH()
#define Display12_CS_get()						(!(Display12_CS_GPIO->IDR & Display12_CS_Pin))
#define Display12_CS_set(value)					if (value) { Display12_CS_set_ACTIVE(); } else { Display12_CS_set_INACTIVE(); }
#define Display12_CS_toggle()					Display12_CS_GPIO->ODR ^= Display12_CS_Pin
#define Display12_CS_pulse()					do { Display12_CS_set_ACTIVE(); delay_loopcnt(LOOPCOUNT_50NS); Display12_CS_set_INACTIVE(); } while (0)
#define Display12_CS_npulse()					do { Display12_CS_set_INACTIVE(); delay_loopcnt(LOOPCOUNT_50NS); Display12_CS_set_ACTIVE(); } while (0)
#define Display12_CS_GPIO_Definition			{ .connect = "Display 18", .gpio = Display12_CS_GPIO, .name = "Display12_CS", .pin = Display12_CS_Pin, .pin_name = "D8" }

// PC1: IOMux_Out_OE (OUT) -- {'activelow': True, 'init': True, 'connect': 'IOMux 5'}
#define IOMux_Out_OE_GPIO						GPIOC
#define IOMux_Out_OE_PinSource					GPIO_PinSource1
#define IOMux_Out_OE_Pin						GPIO_Pin_1
#define IOMux_Out_OE_set_HIGH()					IOMux_Out_OE_GPIO->BSRRL = IOMux_Out_OE_Pin
#define IOMux_Out_OE_set_LOW()					IOMux_Out_OE_GPIO->BSRRH = IOMux_Out_OE_Pin
#define IOMux_Out_OE_set_ACTIVE()				IOMux_Out_OE_set_LOW()
#define IOMux_Out_OE_set_INACTIVE()				IOMux_Out_OE_set_HIGH()
#define IOMux_Out_OE_get()						(!(IOMux_Out_OE_GPIO->IDR & IOMux_Out_OE_Pin))
#define IOMux_Out_OE_set(value)					if (value) { IOMux_Out_OE_set_ACTIVE(); } else { IOMux_Out_OE_set_INACTIVE(); }
#define IOMux_Out_OE_toggle()					IOMux_Out_OE_GPIO->ODR ^= IOMux_Out_OE_Pin
#define IOMux_Out_OE_pulse()					do { IOMux_Out_OE_set_ACTIVE(); delay_loopcnt(LOOPCOUNT_50NS); IOMux_Out_OE_set_INACTIVE(); } while (0)
#define IOMux_Out_OE_npulse()					do { IOMux_Out_OE_set_INACTIVE(); delay_loopcnt(LOOPCOUNT_50NS); IOMux_Out_OE_set_ACTIVE(); } while (0)
#define IOMux_Out_OE_GPIO_Definition			{ .connect = "IOMux 5", .gpio = IOMux_Out_OE_GPIO, .name = "IOMux_Out_OE", .pin = IOMux_Out_OE_Pin, .pin_name = "C1" }

// PC2: IOMux_Out_STCP (OUT) -- {'connect': 'IOMux 6', 'init': True}
#define IOMux_Out_STCP_GPIO						GPIOC
#define IOMux_Out_STCP_PinSource				GPIO_PinSource2
#define IOMux_Out_STCP_Pin						GPIO_Pin_2
#define IOMux_Out_STCP_set_HIGH()				IOMux_Out_STCP_GPIO->BSRRL = IOMux_Out_STCP_Pin
#define IOMux_Out_STCP_set_LOW()				IOMux_Out_STCP_GPIO->BSRRH = IOMux_Out_STCP_Pin
#define IOMux_Out_STCP_set_ACTIVE()				IOMux_Out_STCP_set_HIGH()
#define IOMux_Out_STCP_set_INACTIVE()			IOMux_Out_STCP_set_LOW()
#define IOMux_Out_STCP_get()					(IOMux_Out_STCP_GPIO->IDR & IOMux_Out_STCP_Pin)
#define IOMux_Out_STCP_set(value)				if (value) { IOMux_Out_STCP_set_ACTIVE(); } else { IOMux_Out_STCP_set_INACTIVE(); }
#define IOMux_Out_STCP_toggle()					IOMux_Out_STCP_GPIO->ODR ^= IOMux_Out_STCP_Pin
#define IOMux_Out_STCP_pulse()					do { IOMux_Out_STCP_set_ACTIVE(); delay_loopcnt(LOOPCOUNT_50NS); IOMux_Out_STCP_set_INACTIVE(); } while (0)
#define IOMux_Out_STCP_npulse()					do { IOMux_Out_STCP_set_INACTIVE(); delay_loopcnt(LOOPCOUNT_50NS); IOMux_Out_STCP_set_ACTIVE(); } while (0)
#define IOMux_Out_STCP_GPIO_Definition			{ .connect = "IOMux 6", .gpio = IOMux_Out_STCP_GPIO, .name = "IOMux_Out_STCP", .pin = IOMux_Out_STCP_Pin, .pin_name = "C2" }

// PC11: IOMux_MISO (IN) -- {'af': 'SPI3', 'speed': '25', 'init': True, 'connect': 'IOMux 8'}
#define IOMux_MISO_GPIO							GPIOC
#define IOMux_MISO_PinSource					GPIO_PinSource11
#define IOMux_MISO_Pin							GPIO_Pin_11
#define IOMux_MISO_set_HIGH()					IOMux_MISO_GPIO->BSRRL = IOMux_MISO_Pin
#define IOMux_MISO_set_LOW()					IOMux_MISO_GPIO->BSRRH = IOMux_MISO_Pin
#define IOMux_MISO_set_ACTIVE()					IOMux_MISO_set_HIGH()
#define IOMux_MISO_set_INACTIVE()				IOMux_MISO_set_LOW()
#define IOMux_MISO_get()						(IOMux_MISO_GPIO->IDR & IOMux_MISO_Pin)
#define IOMux_MISO_set(value)					if (value) { IOMux_MISO_set_ACTIVE(); } else { IOMux_MISO_set_INACTIVE(); }
#define IOMux_MISO_toggle()						IOMux_MISO_GPIO->ODR ^= IOMux_MISO_Pin
#define IOMux_MISO_pulse()						do { IOMux_MISO_set_ACTIVE(); delay_loopcnt(LOOPCOUNT_50NS); IOMux_MISO_set_INACTIVE(); } while (0)
#define IOMux_MISO_npulse()						do { IOMux_MISO_set_INACTIVE(); delay_loopcnt(LOOPCOUNT_50NS); IOMux_MISO_set_ACTIVE(); } while (0)
#define IOMux_MISO_GPIO_Definition				{ .connect = "IOMux 8", .gpio = IOMux_MISO_GPIO, .name = "IOMux_MISO", .pin = IOMux_MISO_Pin, .pin_name = "C11" }

// PD6: USART_RX (IN) -- {'af': 'USART2', 'init': True}
#define USART_RX_GPIO							GPIOD
#define USART_RX_PinSource						GPIO_PinSource6
#define USART_RX_Pin							GPIO_Pin_6
#define USART_RX_set_HIGH()						USART_RX_GPIO->BSRRL = USART_RX_Pin
#define USART_RX_set_LOW()						USART_RX_GPIO->BSRRH = USART_RX_Pin
#define USART_RX_set_ACTIVE()					USART_RX_set_HIGH()
#define USART_RX_set_INACTIVE()					USART_RX_set_LOW()
#define USART_RX_get()							(USART_RX_GPIO->IDR & USART_RX_Pin)
#define USART_RX_set(value)						if (value) { USART_RX_set_ACTIVE(); } else { USART_RX_set_INACTIVE(); }
#define USART_RX_toggle()						USART_RX_GPIO->ODR ^= USART_RX_Pin
#define USART_RX_pulse()						do { USART_RX_set_ACTIVE(); delay_loopcnt(LOOPCOUNT_50NS); USART_RX_set_INACTIVE(); } while (0)
#define USART_RX_npulse()						do { USART_RX_set_INACTIVE(); delay_loopcnt(LOOPCOUNT_50NS); USART_RX_set_ACTIVE(); } while (0)
#define USART_RX_GPIO_Definition				{ .gpio = USART_RX_GPIO, .name = "USART_RX", .pin = USART_RX_Pin, .pin_name = "D6" }

// Display_CS: Group of 12 pins (Display1_CS, Display2_CS, Display3_CS, Display4_CS, Display5_CS, Display6_CS, Display7_CS, Display8_CS, Display9_CS, Display10_CS, Display11_CS, Display12_CS)
#define Display_CS_set_ACTIVE()					do { GPIOB->BSRRH = Display1_CS_Pin | Display2_CS_Pin | Display3_CS_Pin | Display4_CS_Pin | Display5_CS_Pin | Display6_CS_Pin | Display7_CS_Pin; GPIOD->BSRRH = Display8_CS_Pin | Display9_CS_Pin | Display10_CS_Pin | Display11_CS_Pin | Display12_CS_Pin; } while (0)
#define Display_CS_set_INACTIVE()				do { GPIOB->BSRRL = Display1_CS_Pin | Display2_CS_Pin | Display3_CS_Pin | Display4_CS_Pin | Display5_CS_Pin | Display6_CS_Pin | Display7_CS_Pin; GPIOD->BSRRL = Display8_CS_Pin | Display9_CS_Pin | Display10_CS_Pin | Display11_CS_Pin | Display12_CS_Pin; } while (0)

void pinmap_initialize(void);

#endif
