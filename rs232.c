#include <stm32f4xx_usart.h>

#include "rs232.h"

void USART2_IRQHandler(void) {
	if (USART_GetITStatus(USART2, USART_IT_RXNE) == SET) {
		uint8_t rxChar = USART_ReceiveData(USART2);
		rs232_transmitchar(rxChar + 1);
		rs232_transmitchar('\r');
		rs232_transmitchar('\n');
	}
}

void rs232_transmitchar(char c) {
	while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);	
	USART_SendData(USART2, c);
}
