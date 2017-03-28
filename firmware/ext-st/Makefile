.PHONY: all setup clean

LIBRARY_PATH := STM32F4xx_DSP_StdPeriph_Lib_V1.8.0
PREFIX := arm-none-eabi-
CFLAGS := -std=c11 -Wall -Os -mcpu=cortex-m4 -g
CFLAGS += -include ../library_config.h -ffunction-sections -fdata-sections
CFLAGS += -I. -I$(LIBRARY_PATH)/Libraries/STM32F4xx_StdPeriph_Driver/inc/ -I${LIBRARY_PATH}/Libraries/CMSIS/Include -I${LIBRARY_PATH}/Libraries/CMSIS/Device/ST/STM32F4xx/Include/

LIBRARY_SRCS := $(wildcard $(LIBRARY_PATH)/Libraries/STM32F4xx_StdPeriph_Driver/src/*.c)
LIBRARY_OBJS := $(LIBRARY_SRCS:.c=.o)

all: stdperiph.a init.o stm32f407.ld

clean:
	rm -f ${LIBRARY_OBJS}
	rm -f init.o stm32f407.ld stdperiph.a include include-cmsis include-cmsis-dev

stdperiph.a: $(LIBRARY_OBJS)
	${PREFIX}ar r $@ $(LIBRARY_OBJS)

init.o: $(LIBRARY_PATH)/Libraries/CMSIS/Device/ST/STM32F4xx/Source/Templates/gcc_ride7/startup_stm32f40_41xxx.s
	${PREFIX}gcc -c ${CFLAGS} -o $@ $<

stm32f407.ld: $(LIBRARY_PATH)/Project/STM32F4xx_StdPeriph_Templates/SW4STM32/STM32F40_41xxx/STM32F417IGHx_FLASH.ld
	cp -n $< $@

.c.o:
	${PREFIX}gcc -c ${CFLAGS} -o $@ $<
