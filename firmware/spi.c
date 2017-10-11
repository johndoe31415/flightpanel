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

#include <stdio.h>
#include <stm32f4xx_spi.h>
#include <stm32f4xx_dma.h>
#include "spi.h"
#include "fncmap.h"
#include "dsr_tasks.h"
#include "displays.h"
#include "fault.h"
#include "pinmap.h"

void DisplaySPI_DMAStream_TX_IRQHandler(void);
void DisplaySPI_DMAStream_TX_IRQHandler(void) {
	if (DMA_GetITStatus(DisplaySPI_DMAStream_TX, DisplaySPI_DMAStream_TX_TCIF)) {
		DMA_ClearITPendingBit(DisplaySPI_DMAStream_TX, DisplaySPI_DMAStream_TX_TCIF);
		SPI_I2S_DMACmd(DisplaySPI_SPI, SPI_I2S_DMAReq_Tx, DISABLE);
		DMA_Cmd(DisplaySPI_DMAStream_TX, DISABLE);
	}
}

void DisplaySPI_DMAStream_RX_IRQHandler(void);
void DisplaySPI_DMAStream_RX_IRQHandler(void) {
	if (DMA_GetITStatus(DisplaySPI_DMAStream_RX, DisplaySPI_DMAStream_RX_TCIF)) {
		DMA_ClearITPendingBit(DisplaySPI_DMAStream_TX, DisplaySPI_DMAStream_RX_TCIF);
		SPI_I2S_DMACmd(DisplaySPI_SPI, SPI_I2S_DMAReq_Rx, DISABLE);
		DMA_Cmd(DisplaySPI_DMAStream_RX, DISABLE);
		isr_display_dma_finished();
		dsr_mark_pending(DSR_TASK_DISPLAY_UPDATE_FINISHED);
	}
}

void IOMuxSPI_DMAStream_TX_IRQHandler(void);
void IOMuxSPI_DMAStream_TX_IRQHandler(void) {
	if (DMA_GetITStatus(IOMuxSPI_DMAStream_TX, IOMuxSPI_DMAStream_TX_TCIF)) {
		DMA_ClearITPendingBit(IOMuxSPI_DMAStream_TX, IOMuxSPI_DMAStream_TX_TCIF);
		SPI_I2S_DMACmd(IOMuxSPI_SPI, SPI_I2S_DMAReq_Tx, DISABLE);
		DMA_Cmd(IOMuxSPI_DMAStream_TX, DISABLE);
	}
}

void IOMuxSPI_DMAStream_RX_IRQHandler(void);
void IOMuxSPI_DMAStream_RX_IRQHandler(void) {
	if (DMA_GetITStatus(IOMuxSPI_DMAStream_RX, IOMuxSPI_DMAStream_RX_TCIF)) {
		DMA_ClearITPendingBit(IOMuxSPI_DMAStream_RX, IOMuxSPI_DMAStream_RX_TCIF);
		SPI_I2S_DMACmd(IOMuxSPI_SPI, SPI_I2S_DMAReq_Rx, DISABLE);
		DMA_Cmd(IOMuxSPI_DMAStream_RX, DISABLE);
		dsr_mark_pending(DSR_TASK_IOMPLEX_FINISHED);
	}
}

static uint8_t spi_transmit_byte(SPI_TypeDef *SPIx, const uint8_t byte) {
	// Wait until SPI register flushed
	while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE) == RESET);

	// Send byte
	SPI_I2S_SendData(SPIx, byte);

	// Wait until byte has been received in exchange
	while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_RXNE) == RESET);

	// Return byte from data register
	return SPI_I2S_ReceiveData(SPIx);
}

void spi_tx_data(SPI_TypeDef *SPIx, const uint8_t *data, const int length) {
	for (int i = 0; i < length; i++) {
		spi_transmit_byte(SPIx, data[i]);
	}
}

bool spi_dma_tx_rx_ready(DMA_Stream_TypeDef *DMAy_Streamx_TX, DMA_Stream_TypeDef *DMAy_Streamx_RX) {
	return ((DMA_GetCmdStatus(DMAy_Streamx_TX) != ENABLE) && (DMA_GetCmdStatus(DMAy_Streamx_RX) != ENABLE));
}

void spi_tx_rx_data_dma(SPI_TypeDef *SPIx, DMA_Stream_TypeDef *DMAy_Streamx_TX, const void *tx_data, DMA_Stream_TypeDef *DMAy_Streamx_RX, void *rx_data, const int length) {
	if ((DMA_GetCmdStatus(DMAy_Streamx_TX) == ENABLE) || (DMA_GetCmdStatus(DMAy_Streamx_RX) == ENABLE)) {
		/* Transmission still in progress. */
		printf("rejecting SPI transfer, DMA of SPI %p still ongoing (TX %d, RX %d)\n", SPIx, DMA_GetCmdStatus(DMAy_Streamx_TX), DMA_GetCmdStatus(DMAy_Streamx_RX));
		return;
	}

	if (tx_data) {
		DMAy_Streamx_TX->M0AR = (uint32_t)tx_data;
	}
	DMAy_Streamx_TX->NDTR = length;
	if (rx_data) {
		DMAy_Streamx_RX->M0AR = (uint32_t)rx_data;
	}
	DMAy_Streamx_RX->NDTR = length;

	DMA_Cmd(DMAy_Streamx_TX, ENABLE);
	DMA_Cmd(DMAy_Streamx_RX, ENABLE);
	SPI_I2S_DMACmd(SPIx, SPI_I2S_DMAReq_Tx | SPI_I2S_DMAReq_Rx, ENABLE);
}

