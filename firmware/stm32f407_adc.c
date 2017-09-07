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

#include <stdio.h>
#include <stm32f4xx_adc.h>
#include <stm32f4xx_rcc.h>

#include "stm32f407_adc.h"
#include "stm32f407_tempsensor.h"

void init_stm32f407_adc(void) {
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

	/* ADC clock selection: 168 MHz / 8 = 21 MHz
	 * Must be inbetween 0.6 MHz and 38 Mhz (typ. 30 MHz) */
	ADC_CommonInitTypeDef ADC_CommonInitStructure = {
		.ADC_Mode = ADC_Mode_Independent,
		.ADC_Prescaler = ADC_Prescaler_Div8,
		.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled,
		.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles,
	};
	ADC_CommonInit(&ADC_CommonInitStructure);

	ADC_InitTypeDef ADC_InitStructure = {
		.ADC_Resolution = ADC_Resolution_12b,
		.ADC_ScanConvMode = DISABLE,
		.ADC_ContinuousConvMode = DISABLE,
		.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None,
		.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1,
		.ADC_DataAlign = ADC_DataAlign_Right,
		.ADC_NbrOfConversion = 1,
	};
	ADC_Init(ADC1, &ADC_InitStructure);

	ADC_VBATCmd(DISABLE);
	ADC_TempSensorVrefintCmd(ENABLE);
	ADC_Cmd(ADC1, ENABLE);
}

static uint16_t adc_sample(void) {
	ADC_ClearFlag(ADC1, ADC_FLAG_EOC);
	ADC_SoftwareStartConv(ADC1);
	while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);
	return ADC_GetConversionValue(ADC1);
}

struct adc_telemetry_t adc_get_telemetry(void) {
	struct adc_telemetry_t telemetry;

	/* Measure Vrefint */
	ADC_RegularChannelConfig(ADC1, ADC_Channel_Vrefint, 1, ADC_SampleTime_480Cycles);
	const uint16_t vrefint_adu = adc_sample();

	/* Measure current temperature sensor value */
	ADC_RegularChannelConfig(ADC1, ADC_Channel_TempSensor, 1, ADC_SampleTime_480Cycles);
	const uint16_t temp_sensor_adu = adc_sample();

	/* Now read calibration values at 30°C and 110°C for this specific MCU,
	 * taken at Vref = 3.3V */
	const uint16_t *ts_cal = (const uint16_t*)0x1fff7a2c;
	const uint16_t adu_30_deg_c = ts_cal[0];
	const uint16_t adu_110_deg_c = ts_cal[1];

	calculate_telemetry(&telemetry, adu_30_deg_c, adu_110_deg_c, vrefint_adu, temp_sensor_adu);
	return telemetry;
}

