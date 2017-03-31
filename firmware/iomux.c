#include <stdio.h>
#include <stdint.h>
#include "iomux.h"
#include "fncmap.h"
#include "spi.h"

#define BYTECOUNT		8
static uint8_t outputs[BYTECOUNT];
static uint8_t inputs[BYTECOUNT];

/* Trigger IOMux transfer */
void iomux_trigger(void) {
	spi_tx_rx_data_dma(IOMuxSPI_SPI, IOMuxSPI_DMAStream_TX, outputs, IOMuxSPI_DMAStream_RX, inputs, sizeof(outputs));
	//spi_tx_data_dma(IOMuxSPI_SPI, IOMuxSPI_DMAStream_TX, outputs, sizeof(outputs));
}

void iomux_dma_finished(void) {
#if 0
	for (int i = 0; i < sizeof(inputs); i++) {
		printf("%02x ", inputs[i]);
	}
	printf("\n");
#endif
}
