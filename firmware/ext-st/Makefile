.PHONY: all setup clean

LIBRARY_PATH := STM32F4xx_DSP_StdPeriph_Lib_V1.8.0
PREFIX := arm-none-eabi-
CFLAGS := $(CFLAGS) -std=c11 -Wall -Os -mcpu=cortex-m4 -g3
CFLAGS += -include ../library_config.h
CFLAGS += -ffunction-sections -fdata-sections
CFLAGS += -I. -I$(LIBRARY_PATH)/Libraries/STM32F4xx_StdPeriph_Driver/inc/ -I${LIBRARY_PATH}/Libraries/CMSIS/Include -I${LIBRARY_PATH}/Libraries/CMSIS/Device/ST/STM32F4xx/Include/

LIBRARY_SRCS := $(wildcard $(LIBRARY_PATH)/Libraries/STM32F4xx_StdPeriph_Driver/src/*.c)
LIBRARY_OBJS := $(LIBRARY_SRCS:.c=.o)

all: stdperiph.a

clean:
	rm -f ${LIBRARY_OBJS}
	rm -f reset.o stdperiph.a

stdperiph.a: $(LIBRARY_OBJS)
	${PREFIX}ar r $@ $(LIBRARY_OBJS)

.c.o:
	${PREFIX}gcc -c ${CFLAGS} -o $@ $<
