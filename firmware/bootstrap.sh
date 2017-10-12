#!/bin/bash -e
#
#	flightpanel - A Cortex-M4 based USB flight panel for flight simulators.
#	Copyright (C) 2017-2017 Johannes Bauer
#
#	This file is part of flightpanel.
#
#	flightpanel is free software; you can redistribute it and/or modify
#	it under the terms of the GNU General Public License as published by
#	the Free Software Foundation; this program is ONLY licensed under
#	version 3 of the License, later versions are explicitly excluded.
#
#	flightpanel is distributed in the hope that it will be useful,
#	but WITHOUT ANY WARRANTY; without even the implied warranty of
#	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#	GNU General Public License for more details.
#
#	You should have received a copy of the GNU General Public License
#	along with flightpanel; if not, write to the Free Software
#	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
#
#	Johannes Bauer <JohannesBauer@gmx.de>


PERIPH_ZIPFILE="en.stm32f4_dsp_stdperiph_lib.zip"
PERIPH_SUBDIR="STM32F4xx_DSP_StdPeriph_Lib_V1.8.0"
CUBE_ZIPFILE="en.stm32cubef4.zip"
CUBE_SUBDIR="STM32Cube_FW_F4_V1.15.0"

################################################################################

if [ ! -f "$PERIPH_ZIPFILE" ]; then
	echo "You need to download the STM32F4 standard peripheral library and/or"
	echo "change the filename that is on top of this file (currently looking"
	echo "for ${PERIPH_ZIPFILE})"
	exit 1
fi

if [ ! -f "$CUBE_ZIPFILE" ]; then
	echo "You need to download the STM32F4 cube library and/or change the"
	echo "filename that is on top of this file (currently looking for"
	echo "for ${CUBE_ZIPFILE})"
	exit 1
fi

# Convert to absolute paths
PERIPH_ZIPFILE=`readlink -f "${PERIPH_ZIPFILE}"`
CUBE_ZIPFILE=`readlink -f "${CUBE_ZIPFILE}"`

cd ext-st
rm -fr "$PERIPH_SUBDIR"
echo "Unzipping ${PERIPH_ZIPFILE}..."
unzip -o "$PERIPH_ZIPFILE" >/dev/null
if [ ! -d "$PERIPH_SUBDIR" ]; then
	echo "Extracted the standard peripheral library successfully, but it did not"
	echo "create the expected subdirectory ${PERIPH_SUBDIR}. Please check the top"
	echo "of the bootstrap script."
	exit 1
fi

# Fix broken ZIP file permissions
find "$PERIPH_SUBDIR" -type d -exec chmod 755 {} \+
find "$PERIPH_SUBDIR" -type f -exec chmod 644 {} \+

# Do some adaptions and create symlinks
rm -f "${PERIPH_SUBDIR}/Libraries/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_fmc.c"
ln -sf "${PERIPH_SUBDIR}/Libraries/STM32F4xx_StdPeriph_Driver/inc" include
ln -sf "${PERIPH_SUBDIR}/Libraries/CMSIS/Include" include-cmsis
ln -sf "${PERIPH_SUBDIR}/Libraries/CMSIS/Device/ST/STM32F4xx/Include" include-cmsis-dev

cd ../cube
rm -fr "$CUBE_SUBDIR"
echo "Unzipping ${CUBE_ZIPFILE}..."
unzip -o "$CUBE_ZIPFILE" >/dev/null
if [ ! -d "$CUBE_SUBDIR" ]; then
	echo "Extracted the Cube library successfully, but it did not create the"
	echo "expected subdirectory ${CUBE_SUBDIR}. Please check the top"
	echo "of the bootstrap script."
	exit 1
fi

# Fix broken ZIP file permissions
find "$CUBE_SUBDIR" -type d -exec chmod 755 {} \+
find "$CUBE_SUBDIR" -type f -exec chmod 644 {} \+

# Do some adaptions and create symlinks
rm -f "${CUBE_SUBDIR}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_rtc.c"
rm -f "${CUBE_SUBDIR}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_timebase_rtc_wakeup_template.c"
rm -f "${CUBE_SUBDIR}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_timebase_rtc_alarm_template.c"
rm -f "${CUBE_SUBDIR}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_timebase_tim_template.c"
rm -f "${CUBE_SUBDIR}/Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_conf_template.c"
rm -f "${CUBE_SUBDIR}/Middlewares/ST/STM32_USB_Device_Library/Core/Inc/usbd_conf_template.h"
cp -n "${CUBE_SUBDIR}/Projects/STM32F4-Discovery/Demonstrations/Inc/usbd_conf.h" .
cp -n "${CUBE_SUBDIR}/Projects/STM32F4-Discovery/Demonstrations/Src/usbd_conf.c" .
sed -i 's/\(NVIC_SetPriority\s*(\s*SysTick_IRQn.*\)/\/\/\0    \/\/ Disable high prio systick because of USART2 deadlock/g' usbd_conf.c
sed "s/.*#define assert_param.*//g;" "${CUBE_SUBDIR}/Projects/STM32F401-Discovery/Examples/BSP/Inc/stm32f4xx_hal_conf.h" >stm32f4xx_hal_conf.h
sed -i "s/.*#define assert_param.*//g;" "${CUBE_SUBDIR}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_utils.c"
ln -fs "${CUBE_SUBDIR}/Middlewares/ST/STM32_USB_Device_Library/Core/Inc/" include-core
ln -fs "${CUBE_SUBDIR}/Drivers/STM32F4xx_HAL_Driver/Inc" include-hal
ln -fs "${CUBE_SUBDIR}/Drivers/CMSIS/Device/ST/STM32F4xx/Include" include-cmsis

#NVIC_SetPriority (SysTick_IRQn
