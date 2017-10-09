#include <stm32f4xx_gpio.h>
#include <stm32f4xx_rcc.h>
#include "pinmap.h"

void pinmap_initialize(void) {
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);

	GPIO_PinAFConfig(GPIOB, Display_SCK_PinSource, GPIO_AF_SPI2);
	GPIO_PinAFConfig(GPIOB, Display_MOSI_PinSource, GPIO_AF_SPI2);
	GPIO_PinAFConfig(GPIOC, IOMux_SCK_PinSource, GPIO_AF_SPI3);
	GPIO_PinAFConfig(GPIOC, IOMux_MOSI_PinSource, GPIO_AF_SPI3);
	GPIO_PinAFConfig(GPIOC, IOMux_MISO_PinSource, GPIO_AF_SPI3);
	GPIO_PinAFConfig(GPIOB, EEPROM_SDA_PinSource, GPIO_AF_I2C1);
	GPIO_PinAFConfig(GPIOB, EEPROM_SCL_PinSource, GPIO_AF_I2C1);
	GPIO_PinAFConfig(GPIOD, USART_TX_PinSource, GPIO_AF_USART2);
	GPIO_PinAFConfig(GPIOD, USART_RX_PinSource, GPIO_AF_USART2);

	{	// 1 pin(s) on PORTA with mode IN, otype OD, speed 2 MHz and pullup DOWN
		*((uint32_t*)(&GPIOA->BSRRL)) = (UserButton_Pin << 16);
		GPIO_InitTypeDef GPIO_InitStructure = {
			.GPIO_Pin = UserButton_Pin,
			.GPIO_Mode = GPIO_Mode_IN,
			.GPIO_OType = GPIO_OType_OD,
			.GPIO_Speed = GPIO_Speed_2MHz,
			.GPIO_PuPd = GPIO_PuPd_DOWN,
		};
		GPIO_Init(GPIOA, &GPIO_InitStructure);
	}
	{	// 2 pin(s) on PORTB with mode AF, otype OD, speed 2 MHz and pullup NOPULL
		*((uint32_t*)(&GPIOB->BSRRL)) = (EEPROM_SDA_Pin << 16) | (EEPROM_SCL_Pin << 16);
		GPIO_InitTypeDef GPIO_InitStructure = {
			.GPIO_Pin = EEPROM_SDA_Pin | EEPROM_SCL_Pin,
			.GPIO_Mode = GPIO_Mode_AF,
			.GPIO_OType = GPIO_OType_OD,
			.GPIO_Speed = GPIO_Speed_2MHz,
			.GPIO_PuPd = GPIO_PuPd_NOPULL,
		};
		GPIO_Init(GPIOB, &GPIO_InitStructure);
	}
	{	// 2 pin(s) on PORTB with mode AF, otype PP, speed 25 MHz and pullup NOPULL
		*((uint32_t*)(&GPIOB->BSRRL)) = (Display_SCK_Pin << 16) | (Display_MOSI_Pin << 16);
		GPIO_InitTypeDef GPIO_InitStructure = {
			.GPIO_Pin = Display_SCK_Pin | Display_MOSI_Pin,
			.GPIO_Mode = GPIO_Mode_AF,
			.GPIO_OType = GPIO_OType_PP,
			.GPIO_Speed = GPIO_Speed_25MHz,
			.GPIO_PuPd = GPIO_PuPd_NOPULL,
		};
		GPIO_Init(GPIOB, &GPIO_InitStructure);
	}
	{	// 7 pin(s) on PORTB with mode OUT, otype PP, speed 25 MHz and pullup NOPULL
		*((uint32_t*)(&GPIOB->BSRRL)) = Display1_CS_Pin | Display2_CS_Pin | Display3_CS_Pin | Display4_CS_Pin | Display5_CS_Pin | Display6_CS_Pin | Display7_CS_Pin;
		GPIO_InitTypeDef GPIO_InitStructure = {
			.GPIO_Pin = Display1_CS_Pin | Display2_CS_Pin | Display3_CS_Pin | Display4_CS_Pin | Display5_CS_Pin | Display6_CS_Pin | Display7_CS_Pin,
			.GPIO_Mode = GPIO_Mode_OUT,
			.GPIO_OType = GPIO_OType_PP,
			.GPIO_Speed = GPIO_Speed_25MHz,
			.GPIO_PuPd = GPIO_PuPd_NOPULL,
		};
		GPIO_Init(GPIOB, &GPIO_InitStructure);
	}
	{	// 1 pin(s) on PORTC with mode AF, otype OD, speed 25 MHz and pullup NOPULL
		*((uint32_t*)(&GPIOC->BSRRL)) = (IOMux_MISO_Pin << 16);
		GPIO_InitTypeDef GPIO_InitStructure = {
			.GPIO_Pin = IOMux_MISO_Pin,
			.GPIO_Mode = GPIO_Mode_AF,
			.GPIO_OType = GPIO_OType_OD,
			.GPIO_Speed = GPIO_Speed_25MHz,
			.GPIO_PuPd = GPIO_PuPd_NOPULL,
		};
		GPIO_Init(GPIOC, &GPIO_InitStructure);
	}
	{	// 2 pin(s) on PORTC with mode AF, otype PP, speed 25 MHz and pullup NOPULL
		*((uint32_t*)(&GPIOC->BSRRL)) = (IOMux_SCK_Pin << 16) | (IOMux_MOSI_Pin << 16);
		GPIO_InitTypeDef GPIO_InitStructure = {
			.GPIO_Pin = IOMux_SCK_Pin | IOMux_MOSI_Pin,
			.GPIO_Mode = GPIO_Mode_AF,
			.GPIO_OType = GPIO_OType_PP,
			.GPIO_Speed = GPIO_Speed_25MHz,
			.GPIO_PuPd = GPIO_PuPd_NOPULL,
		};
		GPIO_Init(GPIOC, &GPIO_InitStructure);
	}
	{	// 3 pin(s) on PORTC with mode OUT, otype PP, speed 2 MHz and pullup NOPULL
		*((uint32_t*)(&GPIOC->BSRRL)) = IOMux_Out_OE_Pin | IOMux_In_PE_Pin | (IOMux_Out_STCP_Pin << 16);
		GPIO_InitTypeDef GPIO_InitStructure = {
			.GPIO_Pin = IOMux_Out_OE_Pin | IOMux_Out_STCP_Pin | IOMux_In_PE_Pin,
			.GPIO_Mode = GPIO_Mode_OUT,
			.GPIO_OType = GPIO_OType_PP,
			.GPIO_Speed = GPIO_Speed_2MHz,
			.GPIO_PuPd = GPIO_PuPd_NOPULL,
		};
		GPIO_Init(GPIOC, &GPIO_InitStructure);
	}
	{	// 1 pin(s) on PORTD with mode AF, otype OD, speed 2 MHz and pullup NOPULL
		*((uint32_t*)(&GPIOD->BSRRL)) = (USART_RX_Pin << 16);
		GPIO_InitTypeDef GPIO_InitStructure = {
			.GPIO_Pin = USART_RX_Pin,
			.GPIO_Mode = GPIO_Mode_AF,
			.GPIO_OType = GPIO_OType_OD,
			.GPIO_Speed = GPIO_Speed_2MHz,
			.GPIO_PuPd = GPIO_PuPd_NOPULL,
		};
		GPIO_Init(GPIOD, &GPIO_InitStructure);
	}
	{	// 1 pin(s) on PORTD with mode AF, otype PP, speed 2 MHz and pullup NOPULL
		*((uint32_t*)(&GPIOD->BSRRL)) = (USART_TX_Pin << 16);
		GPIO_InitTypeDef GPIO_InitStructure = {
			.GPIO_Pin = USART_TX_Pin,
			.GPIO_Mode = GPIO_Mode_AF,
			.GPIO_OType = GPIO_OType_PP,
			.GPIO_Speed = GPIO_Speed_2MHz,
			.GPIO_PuPd = GPIO_PuPd_NOPULL,
		};
		GPIO_Init(GPIOD, &GPIO_InitStructure);
	}
	{	// 6 pin(s) on PORTD with mode OUT, otype PP, speed 2 MHz and pullup NOPULL
		*((uint32_t*)(&GPIOD->BSRRL)) = Display_RESET_Pin | (LEDGreen_Pin << 16) | (LEDOrange_Pin << 16) | (LEDRed_Pin << 16) | (LEDBlue_Pin << 16) | (Display_DC_Pin << 16);
		GPIO_InitTypeDef GPIO_InitStructure = {
			.GPIO_Pin = LEDGreen_Pin | LEDOrange_Pin | LEDRed_Pin | LEDBlue_Pin | Display_RESET_Pin | Display_DC_Pin,
			.GPIO_Mode = GPIO_Mode_OUT,
			.GPIO_OType = GPIO_OType_PP,
			.GPIO_Speed = GPIO_Speed_2MHz,
			.GPIO_PuPd = GPIO_PuPd_NOPULL,
		};
		GPIO_Init(GPIOD, &GPIO_InitStructure);
	}
	{	// 5 pin(s) on PORTD with mode OUT, otype PP, speed 25 MHz and pullup NOPULL
		*((uint32_t*)(&GPIOD->BSRRL)) = Display8_CS_Pin | Display9_CS_Pin | Display10_CS_Pin | Display11_CS_Pin | Display12_CS_Pin;
		GPIO_InitTypeDef GPIO_InitStructure = {
			.GPIO_Pin = Display8_CS_Pin | Display9_CS_Pin | Display10_CS_Pin | Display11_CS_Pin | Display12_CS_Pin,
			.GPIO_Mode = GPIO_Mode_OUT,
			.GPIO_OType = GPIO_OType_PP,
			.GPIO_Speed = GPIO_Speed_25MHz,
			.GPIO_PuPd = GPIO_PuPd_NOPULL,
		};
		GPIO_Init(GPIOD, &GPIO_InitStructure);
	}
	{	// 3 pin(s) on PORTE with mode OUT, otype PP, speed 2 MHz and pullup NOPULL
		*((uint32_t*)(&GPIOE->BSRRL)) = RGB_LED_Green_Pin | RGB_LED_Red_Pin | RGB_LED_Blue_Pin;
		GPIO_InitTypeDef GPIO_InitStructure = {
			.GPIO_Pin = RGB_LED_Green_Pin | RGB_LED_Red_Pin | RGB_LED_Blue_Pin,
			.GPIO_Mode = GPIO_Mode_OUT,
			.GPIO_OType = GPIO_OType_PP,
			.GPIO_Speed = GPIO_Speed_2MHz,
			.GPIO_PuPd = GPIO_PuPd_NOPULL,
		};
		GPIO_Init(GPIOE, &GPIO_InitStructure);
	}
}
