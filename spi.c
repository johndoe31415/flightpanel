#include <stm32f4xx_spi.h>
#include "spi.h"

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
