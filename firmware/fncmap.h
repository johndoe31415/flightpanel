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
