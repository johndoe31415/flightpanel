#include <stm32f4xx_gpio.h>
#include <stm32f4xx_rcc.h>
#include <stm32f4xx_tim.h>
#include <stm32f4xx_usart.h>
#include <stm32f4xx_flash.h>
#include <misc.h>

#define PLL_M      8
#define PLL_N      336
#define PLL_P      2
#define PLL_Q      7

#define UART_TX_PIN                 GPIO_Pin_5
#define UART_TX_GPIO_PORT           GPIOD
#define UART_TX_GPIO_CLK            RCC_AHB1Periph_GPIOD
#define UART_TX_SOURCE              GPIO_PinSource5
#define UART_TX_AF                  GPIO_AF_USART2

#define UART_RX_PIN                 GPIO_Pin_6
#define UART_RX_GPIO_PORT           GPIOD
#define UART_RX_GPIO_CLK            RCC_AHB1Periph_GPIOD
#define UART_RX_SOURCE              GPIO_PinSource6
#define UART_RX_AF                  GPIO_AF_USART2

#define UART_IRQn                   USART2_IRQn


static void init_clock(void) {
	/* Enable HSE */
	RCC->CR |= RCC_CR_HSEON;

	/* Wait until HSE ready */
	while (!(RCC->CR & RCC_CR_HSERDY));

	/* Enable high performance mode, System frequency up to 168 MHz */
	RCC->APB1ENR |= RCC_APB1ENR_PWREN;
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

	/* Wait till the main PLL is used as system clock source */
	while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS ) != RCC_CFGR_SWS_PLL);
}


static void init_gpio(void) {
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
}

static void init_uart(void) {
        /* Connect PXx to USARTx_Tx*/
        GPIO_PinAFConfig(GPIOD, UART_TX_SOURCE, GPIO_AF_USART2);
        GPIO_PinAFConfig(GPIOD, UART_RX_SOURCE, GPIO_AF_USART2);
        
        GPIO_InitTypeDef GPIO_InitStructure;
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
        GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
        GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
        GPIO_Init(GPIOD, &GPIO_InitStructure);
        
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
        GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
        GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
        GPIO_Init(GPIOD, &GPIO_InitStructure);

        /* USART configuration */
        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

        USART_InitTypeDef USART_InitStructure;
        USART_InitStructure.USART_BaudRate = 115200;
        USART_InitStructure.USART_WordLength = USART_WordLength_8b;
        USART_InitStructure.USART_StopBits = USART_StopBits_1;
        USART_InitStructure.USART_Parity = USART_Parity_No;
        USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
        USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
        USART_Init(USART2, &USART_InitStructure);
        
        /* Enable the USARTx Interrupt */
        NVIC_InitTypeDef NVIC_InitStructure;
        NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
        NVIC_Init(&NVIC_InitStructure);

        /* Enable USART */
        USART_Cmd(USART2, ENABLE);
        
        USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
}

void SystemInit() {
	init_clock();
	init_gpio();
	init_uart();
}
