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
#include "boundedbuffer.h"
#include "debugconsole.h"

#define RS232_TX_BUFSIZE		256
static bool in_usart_irq;
static struct bounded_buffer_t rs232_tx_buffer = {
	.bufsize = RS232_TX_BUFSIZE,
	.data = (uint8_t[RS232_TX_BUFSIZE]) { },
};

static bool usart_tx_in_progress(void) {
	/* Transmit data register not empty */
	return !(USART2->SR & (1 << 7));
}

void rs232_debug_setleds(void) {
	LEDGreen_set(usart_tx_in_progress());
	LEDBlue_set(in_usart_irq);
	LEDRed_set(rs232_tx_buffer.fill > 0);
}

static void send_next_byte(void) {
	if (usart_tx_in_progress()) {
		return;
	}

	int16_t next_byte = boundedbuffer_getbyte(&rs232_tx_buffer);
	if (next_byte != -1) {
		/* Transmit data register is empty and we have a byte to send */
		USART_SendData(USART2, next_byte);
	}
}

static void handle_usart_rxc_irq(void) {
	/* Receive complete interrupt */
	if (USART_GetITStatus(USART2, USART_IT_RXNE) == SET) {
		USART_ClearITPendingBit(USART2, USART_IT_RXNE);
		uint8_t rx_char = USART_ReceiveData(USART2);
		debugconsole_rxchar(rx_char);
	}
}

static void handle_usart_txc_irq(void) {
	/* Transmission complete interrupt */
	if (USART_GetITStatus(USART2, USART_IT_TXE) == SET) {
		/* Transmission complete, try to send next byte */
		USART_ClearITPendingBit(USART2, USART_IT_TXE);
		send_next_byte();
	}
}

void USART2_IRQHandler(void) {
	in_usart_irq = true;
	handle_usart_txc_irq();
	handle_usart_rxc_irq();
	in_usart_irq = false;
}

static void rs232_buffer_lock(void) {
	if (in_usart_irq) {
		return;
	}
	USART_ITConfig(USART2, USART_IT_RXNE, DISABLE);	// RX register not empty
	USART_ITConfig(USART2, USART_IT_TXE, DISABLE);	// TX complete
}

static void rs232_buffer_unlock(void) {
	if (in_usart_irq) {
		return;
	}
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);	// RX register not empty
	USART_ITConfig(USART2, USART_IT_TXE, ENABLE);	// TX complete
}

void rs232_transmitchar(char c) {
	bool put_in_buffer;
	do {
		rs232_buffer_lock();
		put_in_buffer = boundedbuffer_putbyte(&rs232_tx_buffer, c);
		if (!put_in_buffer) {
			/* Buffer is full. If this rs232_transmitchar() was issued from an
			 * ISR, then we would usually deadlock here. Therefore, manually
			 * check for IRQ flag here in order to send out buffered data and
			 * clear the TX buffer queue.
			*/
			handle_usart_txc_irq();
			send_next_byte();
		}
		rs232_buffer_unlock();
	} while (!put_in_buffer);

	/* Now that the character is in the TX buffer, trigger actual transmission */
	send_next_byte();
}
