#ifndef __SSD1306_H__
#define __SSD1306_H__

#include <stm32f4xx_gpio.h>
#include <stdbool.h>
#include "surface.h"

#define DISPLAY_WIDTH		128
#define DISPLAY_HEIGHT		32

struct ssd1306_display_t {
	GPIO_TypeDef *nSS_GPIO;
	uint32_t nSS_Pin;
	const struct surface_t *surface;
};

/*************** AUTO GENERATED SECTION FOLLOWS ***************/
void ssd1306_set_data(const struct ssd1306_display_t *display);
void ssd1306_set_control(const struct ssd1306_display_t *display);
void ssd1306_init(const struct ssd1306_display_t *display);
/***************  AUTO GENERATED SECTION ENDS   ***************/

#endif
