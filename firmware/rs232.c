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
static bool tx_in_progress;
static bool in_usart_irq;
static struct bounded_buffer_t rs232_tx_buffer = {
	.bufsize = RS232_TX_BUFSIZE,
	.data = (uint8_t[RS232_TX_BUFSIZE]) { },
};

void rs232_debug_setleds(void) {
	LEDGreen_set(tx_in_progress);
	LEDBlue_set(in_usart_irq);
	LEDRed_set(rs232_tx_buffer.fill > 0);
}

void USART2_IRQHandler(void) {
	in_usart_irq = true;

	/* Receive complete interrupt */
	if (USART_GetITStatus(USART2, USART_IT_RXNE) == SET) {
		USART_ClearITPendingBit(USART2, USART_IT_RXNE);
		uint8_t rx_char = USART_ReceiveData(USART2);
		debugconsole_rxchar(rx_char);
	}

	/* Transmission complete interrupt */
	if (USART_GetITStatus(USART2, USART_IT_TC) == SET) {
		USART_ClearITPendingBit(USART2, USART_IT_TC);
		/* Transmission complete, try to send next byte */
		int16_t next_byte = boundedbuffer_getbyte(&rs232_tx_buffer);
		if (next_byte == -1) {
			/* No next byte, we're finished */
			tx_in_progress = false;
		} else {
			/* Keep sending */
			USART_SendData(USART2, next_byte);
		}
	}

	in_usart_irq = false;
}

static void rs232_buffer_lock(void) {
	if (in_usart_irq) {
		return;
	}
	USART_ITConfig(USART2, USART_IT_RXNE, DISABLE);	// RX register not empty
	USART_ITConfig(USART2, USART_IT_TC, DISABLE);	// TX complete
}

static void rs232_buffer_unlock(void) {
	if (in_usart_irq) {
		return;
	}
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);	// RX register not empty
	USART_ITConfig(USART2, USART_IT_TC, ENABLE);	// TX complete
}

void rs232_transmitchar(char c) {
	bool put_in_buffer;
	bool isr_priority_raised = false;
	do {
		rs232_buffer_lock();
		put_in_buffer = boundedbuffer_putbyte(&rs232_tx_buffer, c);
		if (!put_in_buffer && !isr_priority_raised) {
			/* Buffer is full. If this rs232_transmitchar() was issued from an
			 * ISR, then we would usually deadlock here. Therefore, raise
			 * priority of the USART2 IRQ so that it can preempt the
			 * (potentially) currently running handler and clear the TX buffer
			 * by sending out data. Lower the priority afterwards again.
			*/
			isr_priority_raised = true;
			NVIC_SetPriority(USART2_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));
		}
		rs232_buffer_unlock();
	} while (!put_in_buffer);
	if (isr_priority_raised) {
		/* If we raised ISR priority so that transmission could go through,
		 * then lower it back again */
		NVIC_SetPriority(USART2_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 15, 0));
	}

	/* Now that the character is in the TX buffer, trigger actual transmission */
	rs232_buffer_lock();
	if (!tx_in_progress) {
		tx_in_progress = true;
		USART_SendData(USART2, boundedbuffer_getbyte(&rs232_tx_buffer));
	}
	rs232_buffer_unlock();
}
