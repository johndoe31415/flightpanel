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

#ifndef __SPI_H__
#define __SPI_H__

#include <stdint.h>
#include <stdbool.h>
#include <stm32f4xx_spi.h>
#include <stm32f4xx_dma.h>

/*************** AUTO GENERATED SECTION FOLLOWS ***************/
void DisplaySPI_DMAStream_TX_IRQHandler(void);
void DisplaySPI_DMAStream_RX_IRQHandler(void);
void IOMuxSPI_DMAStream_TX_IRQHandler(void);
void IOMuxSPI_DMAStream_RX_IRQHandler(void);
void spi_tx_data(SPI_TypeDef *SPIx, const uint8_t *data, const int length);
bool spi_dma_tx_rx_ready(DMA_Stream_TypeDef *DMAy_Streamx_TX, DMA_Stream_TypeDef *DMAy_Streamx_RX);
void spi_tx_rx_data_dma(SPI_TypeDef *SPIx, DMA_Stream_TypeDef *DMAy_Streamx_TX, const void *tx_data, DMA_Stream_TypeDef *DMAy_Streamx_RX, void *rx_data, const int length);
/***************  AUTO GENERATED SECTION ENDS   ***************/

#endif
