#ifndef __I2C_H__
#define __I2C_H__

#include <stm32f4xx_i2c.h>

/*************** AUTO GENERATED SECTION FOLLOWS ***************/
bool i2c_start(I2C_TypeDef *I2Cx, int address);
void i2c_scanbus(I2C_TypeDef *I2Cx);
/***************  AUTO GENERATED SECTION ENDS   ***************/

#endif
