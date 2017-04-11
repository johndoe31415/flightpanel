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

#ifndef __I2C_H__
#define __I2C_H__

#include <stm32f4xx_i2c.h>
#include <stdint.h>
#include <stdbool.h>

/*************** AUTO GENERATED SECTION FOLLOWS ***************/
bool i2c_start(I2C_TypeDef *I2Cx, int address, bool do_read);
void i2c_stop(I2C_TypeDef *I2Cx);
int i2c_receive_byte(I2C_TypeDef *I2Cx, bool ack_byte);
bool i2c_receive_bytes(I2C_TypeDef *I2Cx, uint8_t *buffer, int length);
bool i2c_transmit_byte(I2C_TypeDef *I2Cx, uint8_t data);
void i2c_scanbus(I2C_TypeDef *I2Cx);
/***************  AUTO GENERATED SECTION ENDS   ***************/

#endif
