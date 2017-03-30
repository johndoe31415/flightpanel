#include <stm32f4xx_spi.h>
#include <stm32f4xx_dma.h>
#include "spi.h"
#include "displays.h"

void DMA1_Stream4_IRQHandler(void) {
	if (DMA_GetITStatus(DMA1_Stream4, DMA_IT_TCIF4)) {
		DMA_ClearITPendingBit(DMA1_Stream4, DMA_IT_TCIF4);
		DMA_Cmd(DMA1_Stream4, DISABLE);
		display_dma_finished();
	}
}

static uint8_t spi_transmit_byte(uint8_t byte) {
	// Wait until SPI register flushed
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);

	// Send byte
	SPI_I2S_SendData(SPI2, byte);

	// Wait until byte has been received in exchange
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET);

	// Return byte from data register
	return SPI_I2S_ReceiveData(SPI2);
}

void spi_tx_data(const uint8_t *data, int length) {
	for (int i = 0; i < length; i++) {
		spi_transmit_byte(data[i]);
	}
}

void spi_tx_data_dma(const void *data, int length) {
	DMA1_Stream4->M0AR = (uint32_t)data;
	DMA1_Stream4->NDTR = length;
	DMA_Cmd(DMA1_Stream4, ENABLE);
	SPI_I2S_DMACmd(SPI2, SPI_I2S_DMAReq_Tx, ENABLE);
}
