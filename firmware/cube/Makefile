.PHONY: all setup clean

LIBRARY_PATH := STM32Cube_FW_F4_V1.15.0
PREFIX := arm-none-eabi-
CFLAGS := $(CFLAGS) -std=c11 -Wall -Os -mcpu=cortex-m4 -g
CFLAGS += -include ../library_config.h
CFLAGS += -ffunction-sections -fdata-sections
CFLAGS += -I. -I.. -I$(LIBRARY_PATH)/Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I$(LIBRARY_PATH)/Drivers/STM32F4xx_HAL_Driver/Inc -I$(LIBRARY_PATH)/Drivers/CMSIS/Device/ST/STM32F4xx/Include
CFLAGS += -I../ext-st/include -I../ext-st/include-cmsis -I../ext-st/include-cmsis-dev

LIBRARY_SRCS := $(wildcard $(LIBRARY_PATH)/Middlewares/ST/STM32_USB_Device_Library/Core/Src/*.c) $(wildcard $(LIBRARY_PATH)/Drivers/STM32F4xx_HAL_Driver/Src/*.c) usbd_conf.c
LIBRARY_OBJS := $(LIBRARY_SRCS:.c=.o)

all: cube.a

clean:
	rm -f ${LIBRARY_OBJS}
	rm -f cube.a

cube.a: $(LIBRARY_OBJS)
	${PREFIX}ar r $@ $(LIBRARY_OBJS)

.c.o:
	${PREFIX}gcc -c ${CFLAGS} -o $@ $<
