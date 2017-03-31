#ifndef __SPI_H__
#define __SPI_H__

#include <stdint.h>
#include <stm32f4xx_spi.h>
#include <stm32f4xx_dma.h>

/*************** AUTO GENERATED SECTION FOLLOWS ***************/
void DisplaySPI_DMAStream_TX_IRQHandler(void);
void IOMuxSPI_DMAStream_TX_IRQHandler(void);
void spi_tx_data(SPI_TypeDef *SPIx, const uint8_t *data, int length);
void spi_tx_data_dma(SPI_TypeDef *SPIx, DMA_Stream_TypeDef *DMAy_Streamx, const void *data, int length);
void spi_tx_rx_data_dma(SPI_TypeDef *SPIx, DMA_Stream_TypeDef *DMAy_Streamx_TX, const void *tx_data, DMA_Stream_TypeDef *DMAy_Streamx_RX, void *rx_data, int length);
/***************  AUTO GENERATED SECTION ENDS   ***************/

#endif
