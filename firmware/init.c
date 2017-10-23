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

#include <stm32f4xx_gpio.h>
#include <stm32f4xx_rcc.h>
#include <stm32f4xx_tim.h>
#include <stm32f4xx_usart.h>
#include <stm32f4xx_flash.h>
#include <stm32f4xx_spi.h>
#include <stm32f4xx_dma.h>
#include <stm32f4xx_i2c.h>
#include <misc.h>
#include "pinmap.h"
#include "init.h"
#include "rs232.h"
#include "stm32f407_adc.h"

#define PLL_M					8
#define PLL_N					336
#define PLL_P					2
#define PLL_Q					7

#define RUNNING_CLK_FREQUENCY	168000000UL

uint32_t SystemCoreClock = RUNNING_CLK_FREQUENCY;

#define TIM3_CLK_FREQUENCY		2000
#define TIM3_PRESCALER			(RUNNING_CLK_FREQUENCY / 2 / TIM3_CLK_FREQUENCY)
#define TIM3_PERIOD				(TIM3_CLK_FREQUENCY - 1)

#define UART_TX_PIN				GPIO_Pin_5
#define UART_TX_GPIO_PORT		GPIOD
#define UART_TX_GPIO_CLK		RCC_AHB1Periph_GPIOD
#define UART_TX_SOURCE			GPIO_PinSource5
#define UART_TX_AF				GPIO_AF_USART2

#define UART_RX_PIN				GPIO_Pin_6
#define UART_RX_GPIO_PORT		GPIOD
#define UART_RX_GPIO_CLK		RCC_AHB1Periph_GPIOD
#define UART_RX_SOURCE			GPIO_PinSource6
#define UART_RX_AF				GPIO_AF_USART2

#define UART_IRQn				USART2_IRQn

static void init_clock(void) {
	/* Enable HSE */
	RCC->CR |= RCC_CR_HSEON;

	/* Wait until HSE ready */
	while (!(RCC->CR & RCC_CR_HSERDY));

	/* Enable APB1 */
	RCC->APB1ENR |= RCC_APB1ENR_PWREN;

	/* HSICAL 192 */
	PWR->CR |= PWR_CR_PMODE;

	/* HCLK = SYSCLK / 1 */
	RCC->CFGR |= RCC_CFGR_HPRE_DIV1;

	/* PCLK2 = HCLK / 2 */
	RCC->CFGR |= RCC_CFGR_PPRE2_DIV2;

	/* PCLK1 = HCLK / 4 */
	RCC->CFGR |= RCC_CFGR_PPRE1_DIV4;

	/* Configure the main PLL */
	RCC->PLLCFGR =  (PLL_Q << 24) |  (((PLL_P >> 1) - 1) << 16) | (PLL_N << 6) | PLL_M | RCC_PLLCFGR_PLLSRC_HSE;

	/* Enable the main PLL */
	RCC->CR |= RCC_CR_PLLON;

	/* Wait until the main PLL is ready */
	while (!(RCC->CR & RCC_CR_PLLRDY));

	/* Configure Flash prefetch, Instruction cache, Data cache and wait state */
	FLASH->ACR = FLASH_ACR_ICEN | FLASH_ACR_DCEN | FLASH_ACR_LATENCY_5WS;

	/* Select the main PLL as system clock source */
	RCC->CFGR &= ~RCC_CFGR_SW;
	RCC->CFGR |= RCC_CFGR_SW_PLL;

	/* Wait until the main PLL is used as system clock source */
	while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS ) != RCC_CFGR_SWS_PLL);
}

static void init_nvic(void) {
	/* 4 Bits preemtion priority, 0 bits for sub priority */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
}

static void init_uart(void) {
	/* USART configuration */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

	USART_InitTypeDef USART_InitStructure = {
		.USART_BaudRate = 115200,
		.USART_WordLength = USART_WordLength_8b,
		.USART_StopBits = USART_StopBits_1,
		.USART_Parity = USART_Parity_No,
		.USART_HardwareFlowControl = USART_HardwareFlowControl_None,
		.USART_Mode = USART_Mode_Rx | USART_Mode_Tx,
	};
	USART_Init(USART2, &USART_InitStructure);

	/* Enable USART */
	USART_Cmd(USART2, ENABLE);

	/* Enable the USARTx Interrupt */
	NVIC_InitTypeDef NVIC_InitStructure = {
		.NVIC_IRQChannel = USART2_IRQn,
		.NVIC_IRQChannelPreemptionPriority = 15,
		.NVIC_IRQChannelSubPriority = 0,
		.NVIC_IRQChannelCmd = ENABLE,
	};
	NVIC_Init(&NVIC_InitStructure);
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);	// RX register not empty
	USART_ITConfig(USART2, USART_IT_TC, ENABLE);	// TX complete

	/* Send a null byte (first byte will be lost) */
	rs232_transmitchar(0);
}

static void init_timer(void) {
	/* Enable clock */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

	/* Setup time base */
	TIM_TimeBaseInitTypeDef t = {
		.TIM_Prescaler = TIM3_PRESCALER,
		.TIM_CounterMode  = TIM_CounterMode_Up,
		.TIM_Period = TIM3_PERIOD,
		.TIM_ClockDivision = TIM_CKD_DIV1,
		.TIM_RepetitionCounter = 0,
	};
	TIM_TimeBaseInit(TIM3, &t);
	TIM_Cmd(TIM3, ENABLE);

	/* Enable IRQ */
	NVIC_InitTypeDef NVIC_InitStructure = {
		.NVIC_IRQChannel = TIM3_IRQn,
		.NVIC_IRQChannelPreemptionPriority = 15,
		.NVIC_IRQChannelSubPriority = 0,
		.NVIC_IRQChannelCmd = ENABLE,
	};
	NVIC_Init(&NVIC_InitStructure);
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
}

static void init_display_spi(void) {
	// SPI2 with PinPack 2 (PB13 = SCK, PB14 = MISO, PB15 = MOSI) on APB1, GPIOB on AHB1
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);

	SPI_InitTypeDef SPI_InitStructure = {
		.SPI_Direction = SPI_Direction_2Lines_FullDuplex,
		.SPI_Mode = SPI_Mode_Master,
		.SPI_DataSize = SPI_DataSize_8b,
		.SPI_CPOL = SPI_CPOL_Low,
		.SPI_CPHA = SPI_CPHA_1Edge,
		.SPI_NSS = SPI_NSS_Soft,
		.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8,
		.SPI_FirstBit = SPI_FirstBit_MSB,
		.SPI_CRCPolynomial = 1,
	};
	SPI_Init(SPI2, &SPI_InitStructure);
	SPI_Cmd(SPI2, ENABLE);
}


static void init_display_spi_dma(void) {
	/* DMA1 Stream4 Channel 0: SPI2 TX */
	/* DMA1 Stream3 Channel 0: SPI2 RX (dummy reads to be able to use ISR) */

	// Enable DMA1 clock
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);

	/* TX */
	{
		DMA_InitTypeDef DMA_InitStructure = {
			.DMA_Channel = DMA_Channel_0,
			.DMA_PeripheralBaseAddr = (uint32_t)&(SPI2->DR),
			.DMA_Memory0BaseAddr = 0,
			.DMA_DIR = DMA_DIR_MemoryToPeripheral,
			.DMA_BufferSize = 1,
			.DMA_PeripheralInc = DMA_PeripheralInc_Disable,
			.DMA_MemoryInc = DMA_MemoryInc_Enable,
			.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte,
			.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte,
			.DMA_Mode = DMA_Mode_Normal,
			.DMA_Priority = DMA_Priority_Medium,
			.DMA_FIFOMode = DMA_FIFOMode_Disable,
			.DMA_MemoryBurst = DMA_MemoryBurst_Single,
			.DMA_PeripheralBurst = DMA_PeripheralBurst_Single,
		};
		DMA_Init(DMA1_Stream4, &DMA_InitStructure);
	}

	/* RX dummy values */
	{
		static uint8_t display_rx_dummy;
		DMA_InitTypeDef DMA_InitStructure = {
			.DMA_Channel = DMA_Channel_0,
			.DMA_PeripheralBaseAddr = (uint32_t)&(SPI2->DR),
			.DMA_Memory0BaseAddr = (uint32_t)&display_rx_dummy,
			.DMA_DIR = DMA_DIR_PeripheralToMemory,
			.DMA_BufferSize = 1,
			.DMA_PeripheralInc = DMA_PeripheralInc_Disable,
			.DMA_MemoryInc = DMA_MemoryInc_Disable,
			.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte,
			.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte,
			.DMA_Mode = DMA_Mode_Normal,
			.DMA_Priority = DMA_Priority_Medium,
			.DMA_FIFOMode = DMA_FIFOMode_Disable,
			.DMA_MemoryBurst = DMA_MemoryBurst_Single,
			.DMA_PeripheralBurst = DMA_PeripheralBurst_Single,
		};
		DMA_Init(DMA1_Stream3, &DMA_InitStructure);
	}

	/* Enable IRQ */
	{
		NVIC_InitTypeDef NVIC_InitStructure = {
			.NVIC_IRQChannel = DMA1_Stream4_IRQn,
			.NVIC_IRQChannelPreemptionPriority = 1,
			.NVIC_IRQChannelSubPriority = 0,
			.NVIC_IRQChannelCmd = ENABLE,
		};
		NVIC_Init(&NVIC_InitStructure);
	}
	{
		NVIC_InitTypeDef NVIC_InitStructure = {
			.NVIC_IRQChannel = DMA1_Stream3_IRQn,
			.NVIC_IRQChannelPreemptionPriority = 1,
			.NVIC_IRQChannelSubPriority = 0,
			.NVIC_IRQChannelCmd = ENABLE,
		};
		NVIC_Init(&NVIC_InitStructure);
	}
	DMA_ITConfig(DMA1_Stream4, DMA_IT_TC, ENABLE);
	DMA_ITConfig(DMA1_Stream3, DMA_IT_TC, ENABLE);
}


static void init_iomux_spi(void) {
	// SPI3 with PinPack ? (PC10 = SCK, PC11 = MISO, PC12 = MOSI) on APB1, GPIOC on AHB1
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3, ENABLE);

	SPI_InitTypeDef SPI_InitStructure = {
		.SPI_Direction = SPI_Direction_2Lines_FullDuplex,
		.SPI_Mode = SPI_Mode_Master,
		.SPI_DataSize = SPI_DataSize_8b,
		.SPI_CPOL = SPI_CPOL_Low,
		.SPI_CPHA = SPI_CPHA_1Edge,
		.SPI_NSS = SPI_NSS_Soft,
		.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_32,
		.SPI_FirstBit = SPI_FirstBit_MSB,
		.SPI_CRCPolynomial = 1,
	};
	SPI_Init(SPI3, &SPI_InitStructure);
	SPI_Cmd(SPI3, ENABLE);
}

void reinit_iomux_spi_sck_AF(bool use_af) {
	GPIO_InitTypeDef GPIO_InitStructure = {
		.GPIO_Pin = GPIO_Pin_10,
		.GPIO_Mode = use_af ? GPIO_Mode_AF : GPIO_Mode_OUT,
		.GPIO_OType = GPIO_OType_PP,
		.GPIO_Speed = GPIO_Speed_25MHz,
		.GPIO_PuPd = GPIO_PuPd_NOPULL,
	};
	GPIO_Init(GPIOC, &GPIO_InitStructure);
}

static void init_iomux_spi_dma(void) {
	/* DMA1 Stream5 Channel 0: SPI3 TX */
	/* DMA1 Stream2 Channel 0: SPI3 RX */

	// Enable DMA1 clock
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);

	/* TX */
	{
		DMA_InitTypeDef DMA_InitStructure = {
			.DMA_Channel = DMA_Channel_0,
			.DMA_PeripheralBaseAddr = (uint32_t)&(SPI3->DR),
			.DMA_Memory0BaseAddr = 0,
			.DMA_DIR = DMA_DIR_MemoryToPeripheral,
			.DMA_BufferSize = 1,
			.DMA_PeripheralInc = DMA_PeripheralInc_Disable,
			.DMA_MemoryInc = DMA_MemoryInc_Enable,
			.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte,
			.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte,
			.DMA_Mode = DMA_Mode_Normal,
			.DMA_Priority = DMA_Priority_Medium,
			.DMA_FIFOMode = DMA_FIFOMode_Disable,
			.DMA_MemoryBurst = DMA_MemoryBurst_Single,
			.DMA_PeripheralBurst = DMA_PeripheralBurst_Single,
		};
		DMA_Init(DMA1_Stream5, &DMA_InitStructure);
	}

	/* RX */
	{
		DMA_InitTypeDef DMA_InitStructure = {
			.DMA_Channel = DMA_Channel_0,
			.DMA_PeripheralBaseAddr = (uint32_t)&(SPI3->DR),
			.DMA_Memory0BaseAddr = 0,
			.DMA_DIR = DMA_DIR_PeripheralToMemory,
			.DMA_BufferSize = 1,
			.DMA_PeripheralInc = DMA_PeripheralInc_Disable,
			.DMA_MemoryInc = DMA_MemoryInc_Enable,
			.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte,
			.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte,
			.DMA_Mode = DMA_Mode_Normal,
			.DMA_Priority = DMA_Priority_Medium,
			.DMA_FIFOMode = DMA_FIFOMode_Disable,
			.DMA_MemoryBurst = DMA_MemoryBurst_Single,
			.DMA_PeripheralBurst = DMA_PeripheralBurst_Single,
		};
		DMA_Init(DMA1_Stream2, &DMA_InitStructure);
	}

	/* Enable IRQ */
	NVIC_InitTypeDef NVIC_InitStructure = {
		.NVIC_IRQChannel = DMA1_Stream5_IRQn,
		.NVIC_IRQChannelPreemptionPriority = 1,
		.NVIC_IRQChannelSubPriority = 0,
		.NVIC_IRQChannelCmd = ENABLE,
	};
	NVIC_Init(&NVIC_InitStructure);
	NVIC_InitTypeDef NVIC_InitStructure2 = {
		.NVIC_IRQChannel = DMA1_Stream2_IRQn,
		.NVIC_IRQChannelPreemptionPriority = 1,
		.NVIC_IRQChannelSubPriority = 0,
		.NVIC_IRQChannelCmd = ENABLE,
	};
	NVIC_Init(&NVIC_InitStructure2);
	DMA_ITConfig(DMA1_Stream5, DMA_IT_TC, ENABLE);
	DMA_ITConfig(DMA1_Stream2, DMA_IT_TC, ENABLE);
}

static void init_usb(void) {
	/* Enable OTG Full Speed */
	RCC_AHB2PeriphClockCmd(RCC_AHB2Periph_OTG_FS, ENABLE);

	/* Enable system configuration controller clock */
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

	/* Enable IRQ */
	NVIC_InitTypeDef NVIC_InitStructure = {
		.NVIC_IRQChannel = OTG_FS_IRQn,
		.NVIC_IRQChannelPreemptionPriority = 15,
		.NVIC_IRQChannelSubPriority = 0,
		.NVIC_IRQChannelCmd = ENABLE,
	};
	NVIC_Init(&NVIC_InitStructure);
}

static void init_systick(void) {
	SysTick_Config(RUNNING_CLK_FREQUENCY / 1000);
}

static void init_i2c(void) {
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);

	I2C_InitTypeDef I2C_InitStructure = {
		.I2C_ClockSpeed = 400000,
		.I2C_Mode = I2C_Mode_I2C,
		.I2C_DutyCycle = I2C_DutyCycle_16_9,
		.I2C_Ack = I2C_Ack_Enable,
		.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit,
	};
	I2C_Init(I2C1, &I2C_InitStructure);
	I2C_Cmd(I2C1, ENABLE);
}

static void init_crc(void) {
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_CRC, ENABLE);
}

void SystemInit(void) {
	__disable_irq();
	init_clock();
	init_nvic();
	pinmap_initialize();
	init_uart();
	init_display_spi();
	init_display_spi_dma();
	init_iomux_spi();
	init_iomux_spi_dma();
	init_i2c();
	init_crc();
//	init_debug();
	init_usb();
	init_stm32f407_adc();
	init_systick();
	init_timer();
	__enable_irq();
}
