#include <stdio.h>
#include <stm32f4xx_spi.h>
#include <stm32f4xx_dma.h>
#include "spi.h"
#include "displays.h"
#include "iomux.h"
#include "fncmap.h"

void DisplaySPI_DMAStream_TX_IRQHandler(void);
void DisplaySPI_DMAStream_TX_IRQHandler(void) {
	if (DMA_GetITStatus(DisplaySPI_DMAStream_TX, DisplaySPI_DMAStream_TX_IRQFlag)) {
		DMA_ClearITPendingBit(DisplaySPI_DMAStream_TX, DisplaySPI_DMAStream_TX_IRQFlag);
		DMA_Cmd(DisplaySPI_DMAStream_TX, DISABLE);
		display_dma_finished();
	}
}

void IOMuxSPI_DMAStream_TX_IRQHandler(void);
void IOMuxSPI_DMAStream_TX_IRQHandler(void) {
	if (DMA_GetITStatus(IOMuxSPI_DMAStream_TX, IOMuxSPI_DMAStream_TX_IRQFlag)) {
		DMA_ClearITPendingBit(IOMuxSPI_DMAStream_TX, IOMuxSPI_DMAStream_TX_IRQFlag);
		DMA_Cmd(IOMuxSPI_DMAStream_TX, DISABLE);
	}
}

void IOMuxSPI_DMAStream_RX_IRQHandler(void);
void IOMuxSPI_DMAStream_RX_IRQHandler(void) {
	if (DMA_GetITStatus(IOMuxSPI_DMAStream_RX, IOMuxSPI_DMAStream_RX_IRQFlag)) {
		DMA_ClearITPendingBit(IOMuxSPI_DMAStream_RX, IOMuxSPI_DMAStream_RX_IRQFlag);
		DMA_Cmd(IOMuxSPI_DMAStream_RX, DISABLE);
		iomux_dma_finished();
	}
}

static uint8_t spi_transmit_byte(SPI_TypeDef *SPIx, uint8_t byte) {
	// Wait until SPI register flushed
	while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE) == RESET);

	// Send byte
	SPI_I2S_SendData(SPIx, byte);

	// Wait until byte has been received in exchange
	while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_RXNE) == RESET);

	// Return byte from data register
	return SPI_I2S_ReceiveData(SPIx);
}

void spi_tx_data(SPI_TypeDef *SPIx, const uint8_t *data, int length) {
	for (int i = 0; i < length; i++) {
		spi_transmit_byte(SPIx, data[i]);
	}
}

void spi_tx_data_dma(SPI_TypeDef *SPIx, DMA_Stream_TypeDef *DMAy_Streamx, const void *data, int length) {
	if (DMA_GetCmdStatus(DMAy_Streamx) == ENABLE) {
		/* Transmission still in progress. */
		printf("rejecting SPI transfer, DMA still ongoing\n");
		return;
	}

	DMAy_Streamx->M0AR = (uint32_t)data;
	DMAy_Streamx->NDTR = length;
	DMA_Cmd(DMAy_Streamx, ENABLE);
	SPI_I2S_DMACmd(SPIx, SPI_I2S_DMAReq_Tx, ENABLE);
}

void spi_tx_rx_data_dma(SPI_TypeDef *SPIx, DMA_Stream_TypeDef *DMAy_Streamx_TX, const void *tx_data, DMA_Stream_TypeDef *DMAy_Streamx_RX, void *rx_data, int length) {
	if ((DMA_GetCmdStatus(DMAy_Streamx_TX) == ENABLE) || (DMA_GetCmdStatus(DMAy_Streamx_RX) == ENABLE)) {
		/* Transmission still in progress. */
		printf("rejecting SPI transfer, DMA still ongoing\n");
		return;
	}

	DMAy_Streamx_TX->M0AR = (uint32_t)tx_data;
	DMAy_Streamx_TX->NDTR = length;
	DMAy_Streamx_RX->M0AR = (uint32_t)rx_data;
	DMAy_Streamx_RX->NDTR = length;

	DMA_Cmd(DMAy_Streamx_TX, ENABLE);
	DMA_Cmd(DMAy_Streamx_RX, ENABLE);
	SPI_I2S_DMACmd(SPIx, SPI_I2S_DMAReq_Tx, ENABLE);
	SPI_I2S_DMACmd(SPIx, SPI_I2S_DMAReq_Rx, ENABLE);
}


