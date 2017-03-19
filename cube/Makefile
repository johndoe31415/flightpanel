.PHONY: all setup clean

LIBRARY_PATH := STM32Cube_FW_F4_V1.15.0
PREFIX := arm-none-eabi-
CFLAGS := -std=c11 -Wall -Os -mcpu=cortex-m4 -g
CFLAGS += -include ../library_config.h -ffunction-sections -fdata-sections
CFLAGS += -I. -I.. -I$(LIBRARY_PATH)/Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I$(LIBRARY_PATH)/Middlewares/ST/STM32_USB_Device_Library/Class/HID/Inc -I$(LIBRARY_PATH)/Drivers/STM32F4xx_HAL_Driver/Inc -I$(LIBRARY_PATH)/Drivers/CMSIS/Device/ST/STM32F4xx/Include
CFLAGS += -I../ext-st/include -I../ext-st/include-cmsis -I../ext-st/include-cmsis-dev

LIBRARY_SRCS := $(wildcard $(LIBRARY_PATH)/Middlewares/ST/STM32_USB_Device_Library/Core/Src/*.c) $(wildcard $(LIBRARY_PATH)/Middlewares/ST/STM32_USB_Device_Library/Class/HID/Src/*.c) $(wildcard $(LIBRARY_PATH)/Drivers/STM32F4xx_HAL_Driver/Src/*.c) usbd_conf.c
LIBRARY_OBJS := $(LIBRARY_SRCS:.c=.o)

all: cube.a

setup:
	rm -f ${LIBRARY_PATH}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_rtc.c
	rm -f ${LIBRARY_PATH}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_timebase_rtc_wakeup_template.c
	rm -f ${LIBRARY_PATH}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_timebase_rtc_alarm_template.c
	rm -f ${LIBRARY_PATH}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_timebase_tim_template.c
	rm -f ${LIBRARY_PATH}/Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_conf_template.c
	rm -f ${LIBRARY_PATH}/Middlewares/ST/STM32_USB_Device_Library/Core/Inc/usbd_conf_template.h
	cp -n ${LIBRARY_PATH}/Projects/STM32F4-Discovery/Demonstrations/Inc/usbd_conf.h .
	cp -n ${LIBRARY_PATH}/Projects/STM32F4-Discovery/Demonstrations/Src/usbd_conf.c .
	cp -n ${LIBRARY_PATH}/Projects/STM32F401-Discovery/Examples/BSP/Inc/stm32f4xx_hal_conf.h .
	ln -fs ${LIBRARY_PATH}/Middlewares/ST/STM32_USB_Device_Library/Core/Inc/ include-core
	ln -fs ${LIBRARY_PATH}/Middlewares/ST/STM32_USB_Device_Library/Class/HID/Inc/ include-hid
	ln -fs ${LIBRARY_PATH}/Drivers/STM32F4xx_HAL_Driver/Inc include-hal
	ln -fs ${LIBRARY_PATH}/Drivers/CMSIS/Device/ST/STM32F4xx/Include include-cmsis

clean:
	rm -f ${LIBRARY_OBJS}
	rm -f cube.a

cube.a: $(LIBRARY_OBJS)
	${PREFIX}ar r $@ $(LIBRARY_OBJS)

.c.o:
	${PREFIX}gcc -c ${CFLAGS} -o $@ $<
