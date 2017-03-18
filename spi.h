#ifndef __SPI_H__
#define __SPI_H__

#include <stdint.h>

/*************** AUTO GENERATED SECTION FOLLOWS ***************/
void DMA1_Stream4_IRQHandler(void);
void spi_tx_data(const uint8_t *data, int length);
void spi_tx_data_dma(const void *data, int length);
/***************  AUTO GENERATED SECTION ENDS   ***************/

#endif
