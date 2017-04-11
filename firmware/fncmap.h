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

#ifndef __FNCMAP_H__
#define __FNCMAP_H__

#include <stm32f4xx_spi.h>
#include <stm32f4xx_dma.h>

#define DisplaySPI_DMAStream_TX_IRQHandler		DMA1_Stream4_IRQHandler
#define DisplaySPI_DMAStream_TX_IRQFlag			DMA_IT_TCIF4
#define DisplaySPI_DMAStream_TX					DMA1_Stream4
#define DisplaySPI_SPI							SPI2

#define IOMuxSPI_DMAStream_TX_IRQHandler		DMA1_Stream5_IRQHandler
#define IOMuxSPI_DMAStream_TX_IRQFlag			DMA_IT_TCIF5
#define IOMuxSPI_DMAStream_TX					DMA1_Stream5
#define IOMuxSPI_DMAStream_RX_IRQHandler		DMA1_Stream2_IRQHandler
#define IOMuxSPI_DMAStream_RX_IRQFlag			DMA_IT_TCIF2
#define IOMuxSPI_DMAStream_RX					DMA1_Stream2
#define IOMuxSPI_SPI							SPI3

#endif
