#ifndef __I2C_H__
#define __I2C_H__

#include <stm32f4xx_i2c.h>
#include <stdint.h>
#include <stdbool.h>

/*************** AUTO GENERATED SECTION FOLLOWS ***************/
void i2c_stop(I2C_TypeDef *I2Cx);
bool i2c_start(I2C_TypeDef *I2Cx, int address, bool do_read);
int i2c_receive_byte(I2C_TypeDef *I2Cx, bool ack_byte);
bool i2c_receive_bytes(I2C_TypeDef *I2Cx, uint8_t *buffer, int length);
void i2c_transmit_byte(I2C_TypeDef *I2Cx, uint8_t data);
void i2c_scanbus(I2C_TypeDef *I2Cx);
/***************  AUTO GENERATED SECTION ENDS   ***************/

#endif
