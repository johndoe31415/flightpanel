#ifndef __SSD1306_H__
#define __SSD1306_H__

#include <stm32f4xx_gpio.h>

#define DISPLAY_WIDTH		128
#define DISPLAY_HEIGHT		32

struct ssd1306_display {
	GPIO_TypeDef *nSS_GPIO;
	uint32_t nSS_Pin;
	uint8_t display_data[DISPLAY_WIDTH * DISPLAY_HEIGHT / 8];
};

/*************** AUTO GENERATED SECTION FOLLOWS ***************/
void ssd1306_init(const struct ssd1306_display *display);
/***************  AUTO GENERATED SECTION ENDS   ***************/

#endif
