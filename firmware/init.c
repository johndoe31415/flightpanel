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


static void init_gpio(void) {
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure = {
		.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15,
		.GPIO_Mode = GPIO_Mode_OUT,
		.GPIO_OType = GPIO_OType_PP,
		.GPIO_Speed = GPIO_Speed_2MHz,
		.GPIO_PuPd = GPIO_PuPd_NOPULL,
	};
	GPIO_Init(GPIOD, &GPIO_InitStructure);
}

static void init_uart(void) {
	/* Connect PXx to USARTx_Tx*/
	GPIO_PinAFConfig(GPIOD, UART_TX_SOURCE, GPIO_AF_USART2);
	GPIO_PinAFConfig(GPIOD, UART_RX_SOURCE, GPIO_AF_USART2);

	{
		GPIO_InitTypeDef GPIO_InitStructure = {
			.GPIO_Pin = GPIO_Pin_5,
			.GPIO_Mode = GPIO_Mode_AF,
			.GPIO_OType = GPIO_OType_PP,
			.GPIO_Speed = GPIO_Speed_2MHz,
			.GPIO_PuPd = GPIO_PuPd_NOPULL,
		};
		GPIO_Init(GPIOD, &GPIO_InitStructure);
	}
	{
		GPIO_InitTypeDef GPIO_InitStructure = {
			.GPIO_Pin = GPIO_Pin_6,
			.GPIO_Mode = GPIO_Mode_AF,
			.GPIO_OType = GPIO_OType_OD,
			.GPIO_Speed = GPIO_Speed_2MHz,
			.GPIO_PuPd = GPIO_PuPd_NOPULL,
		};
		GPIO_Init(GPIOD, &GPIO_InitStructure);
	}

	/* USART configuration */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
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
		.NVIC_IRQChannelPreemptionPriority = 0,
		.NVIC_IRQChannelSubPriority = 0,
		.NVIC_IRQChannelCmd = ENABLE,
	};
	NVIC_Init(&NVIC_InitStructure);
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);

	/* Send a null byte (first byte will be lost */
	USART_SendData(USART2, 0);
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
		.NVIC_IRQChannelPreemptionPriority = 0,
		.NVIC_IRQChannelSubPriority = 0,
		.NVIC_IRQChannelCmd = ENABLE,
	};
	NVIC_Init(&NVIC_InitStructure);
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
}

static void init_rotary_encoders(void) {
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure = {
		.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11,
		.GPIO_Mode = GPIO_Mode_IN,
		.GPIO_OType = GPIO_OType_OD,
		.GPIO_Speed = GPIO_Speed_2MHz,
		.GPIO_PuPd = GPIO_PuPd_UP,
	};
	GPIO_Init(GPIOD, &GPIO_InitStructure);
}

static void init_spi(void) {
	// SPI2 with PinPack 2 (PB13 = SCK, PB14 = MISO, PB15 = MOSI) on APB1, GPIOB on AHB1
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);

	GPIO_PinAFConfig(GPIOB, GPIO_PinSource13, GPIO_AF_SPI2);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource14, GPIO_AF_SPI2);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource15, GPIO_AF_SPI2);

	{
		GPIO_InitTypeDef GPIO_InitStructure = {
			.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_15,
			.GPIO_Mode = GPIO_Mode_AF,
			.GPIO_OType = GPIO_OType_PP,
			.GPIO_Speed = GPIO_Speed_25MHz,
			.GPIO_PuPd = GPIO_PuPd_NOPULL,
		};
		GPIO_Init(GPIOB, &GPIO_InitStructure);
	}
	{
		GPIO_InitTypeDef GPIO_InitStructure = {
			.GPIO_Pin = GPIO_Pin_14,
			.GPIO_Mode = GPIO_Mode_AF,
			.GPIO_OType = GPIO_OType_OD,
			.GPIO_Speed = GPIO_Speed_2MHz,
			.GPIO_PuPd = GPIO_PuPd_NOPULL,
		};
		GPIO_Init(GPIOB, &GPIO_InitStructure);
	}

	SPI_InitTypeDef SPI_InitStructure = {
		.SPI_Direction = SPI_Direction_2Lines_FullDuplex,
		.SPI_Mode = SPI_Mode_Master,
		.SPI_DataSize = SPI_DataSize_8b,
		.SPI_CPOL = SPI_CPOL_Low,
		.SPI_CPHA = SPI_CPHA_1Edge,
		.SPI_NSS = SPI_NSS_Soft,
		.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4,
		.SPI_FirstBit = SPI_FirstBit_MSB,
		.SPI_CRCPolynomial = 1,
	};
	SPI_Init(SPI2, &SPI_InitStructure);
	SPI_Cmd(SPI2, ENABLE);
}

static void init_spi_dma(void) {
	/* DMA1 Stream3 Channel 0: SPI2 RX (unused) */
	/* DMA1 Stream4 Channel 1: SPI2 TX */

	// Enable DMA2 clock
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);

	/* TX */
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

	/* Enable IRQ */
	NVIC_InitTypeDef NVIC_InitStructure = {
		.NVIC_IRQChannel = DMA1_Stream4_IRQn,
		.NVIC_IRQChannelPreemptionPriority = 0,
		.NVIC_IRQChannelSubPriority = 0,
		.NVIC_IRQChannelCmd = ENABLE,
	};
	NVIC_Init(&NVIC_InitStructure);
	DMA_ITConfig(DMA1_Stream4, DMA_IT_TC, ENABLE);
}

#if 0
static void init_usb(void) {
	/* VBUS = PA9, ID = PA10, D- = PA11, D+ = PA12 */
	/* PowerSwitchOn = PC0, OverCurrent = PD5 */

	// PA8 9 11 12?

    /* Enable all GPIO clocks */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

	/* Enable alternate USB function */
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource11, GPIO_AF_OTG1_FS);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource12, GPIO_AF_OTG1_FS);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_OTG1_FS);

    /* USB Data */
	{
		GPIO_InitTypeDef GPIO_InitStructure = {
			.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12           | GPIO_Pin_8 | GPIO_Pin_9,
			.GPIO_Mode = GPIO_Mode_AF,
			.GPIO_OType = GPIO_OType_PP,
			.GPIO_Speed = GPIO_Speed_100MHz,
			.GPIO_PuPd = GPIO_PuPd_NOPULL,
		};
		GPIO_Init(GPIOA, &GPIO_InitStructure);
	}

#if 0
	/* VBUS */
	{
		GPIO_InitTypeDef GPIO_InitStructure = {
			.GPIO_Pin = GPIO_Pin_9,
			.GPIO_Mode = GPIO_Mode_IN,
			.GPIO_OType = GPIO_OType_OD,
			.GPIO_Speed = GPIO_Speed_2MHz,
			.GPIO_PuPd = GPIO_PuPd_NOPULL,
		};
		GPIO_Init(GPIOA, &GPIO_InitStructure);
	}
#endif

	/* ID */
	{
		GPIO_InitTypeDef GPIO_InitStructure = {
			.GPIO_Pin = GPIO_Pin_10,
			.GPIO_Mode = GPIO_Mode_AF,
			.GPIO_OType = GPIO_OType_OD,
			.GPIO_Speed = GPIO_Speed_50MHz,
			.GPIO_PuPd = GPIO_PuPd_UP,
		};
		GPIO_Init(GPIOA, &GPIO_InitStructure);
	}

	/* Enable OTG Full Speed */
	RCC_AHB2PeriphClockCmd(RCC_AHB2Periph_OTG_FS, ENABLE);

	/* Enable system configuration controller clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

	/* Enable IRQ */
	NVIC_InitTypeDef NVIC_InitStructure = {
		.NVIC_IRQChannel = OTG_FS_IRQn,
		.NVIC_IRQChannelPreemptionPriority = 0,
		.NVIC_IRQChannelSubPriority = 0,
		.NVIC_IRQChannelCmd = ENABLE,
	};
	NVIC_Init(&NVIC_InitStructure);
}
#endif

static void init_systick(void) {
	SysTick_Config(RUNNING_CLK_FREQUENCY / 1000);
}

static void init_i2c(void) {
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);

	/* Enable alternate I2C function */
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_I2C1);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource9, GPIO_AF_I2C1);

	GPIO_InitTypeDef GPIO_InitStructure = {
		.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_9,
		.GPIO_Mode = GPIO_Mode_AF,
		.GPIO_OType = GPIO_OType_OD,
		.GPIO_Speed = GPIO_Speed_2MHz,
		.GPIO_PuPd = GPIO_PuPd_NOPULL,
	};
	GPIO_Init(GPIOB, &GPIO_InitStructure);

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

static void init_debug(void) {
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure = {
		.GPIO_Pin = Dbg1_Pin | Dbg2_Pin | Dbg3_Pin | Dbg4_Pin,
		.GPIO_Mode = GPIO_Mode_OUT,
		.GPIO_OType = GPIO_OType_PP,
		.GPIO_Speed = GPIO_Speed_2MHz,
		.GPIO_PuPd = GPIO_PuPd_NOPULL,
	};
	GPIO_Init(Dbg1_GPIO, &GPIO_InitStructure);
}

void SystemInit() {
	__disable_irq();
	init_clock();
	init_gpio();
	init_uart();
	init_timer();
	init_rotary_encoders();
	init_spi();
	init_spi_dma();
	init_systick();
	init_i2c();
	init_crc();
	init_debug();
//	init_usb();
	__enable_irq();
}
