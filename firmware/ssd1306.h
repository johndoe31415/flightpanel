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

#ifndef __SSD1306_H__
#define __SSD1306_H__

#include <stm32f4xx_gpio.h>
#include <stdbool.h>
#include "surface.h"

#define DISPLAY_WIDTH		128
#define DISPLAY_HEIGHT		32

struct ssd1306_display_t {
	GPIO_TypeDef *CS_GPIO;
	uint32_t CS_Pin;
	const struct surface_t *surface;
};

/*************** AUTO GENERATED SECTION FOLLOWS ***************/
void ssd1306_set_data(const struct ssd1306_display_t *display);
void ssd1306_set_control(const struct ssd1306_display_t *display);
void ssd1306_init(const struct ssd1306_display_t *display);
/***************  AUTO GENERATED SECTION ENDS   ***************/

#endif
