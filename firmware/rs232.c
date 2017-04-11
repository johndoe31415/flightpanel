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

#include <stm32f4xx_usart.h>

#include "rs232.h"
#include "pinmap.h"

void USART2_IRQHandler(void) {
	if (USART_GetITStatus(USART2, USART_IT_RXNE) == SET) {
		uint8_t rxChar = USART_ReceiveData(USART2);
//		rs232_transmitchar(rxChar + 1);
//		rs232_transmitchar('\r');
//		rs232_transmitchar('\n');

		if ((rxChar >= 'a') && (rxChar <= 'a' + 0xf)) {
			rxChar -= 'a';
			/*
			Dbg1_SetTo(rxChar & 0x01);
			Dbg2_SetTo(rxChar & 0x02);
			Dbg3_SetTo(rxChar & 0x04);
			Dbg4_SetTo(rxChar & 0x08);
			*/
		}

	}
}

void rs232_transmitchar(char c) {
	while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);
	USART_SendData(USART2, c);
}
