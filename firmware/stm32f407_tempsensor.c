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

#include "stm32f407_tempsensor.h"

//#define RDIV(p, q)		((p) / (q))					/* No rounding */
#define RDIV(p, q)		(((p) + ((q) / 2)) / (q))		/* With rounding */

void calculate_telemetry(struct adc_telemetry_t *telemetry, const uint16_t adu_30_deg_c, const uint16_t adu_110_deg_c, const uint16_t adu_vrefint, const uint16_t adu_tempsensor) {
	/* Assume Vrefint is 1.21 V (typical according to datasheet) and calculate
	 * Vref from this (Vdd = Vref in our case). Since:
	 *    ADU / 0xfff * Vref = 1.21 V
	 * It follows:
	 *    Vref = 1.21 V / ADU * 0xfff
	 */
	telemetry->vref_millivolts = (0xfff * 1210) / adu_vrefint;

	/* Determine slope in µV/°C */
	const uint32_t microvolt_30_deg_c = 220000 * adu_30_deg_c / 273;
	const uint32_t microvolt_110_deg_c = 220000 * adu_110_deg_c / 273;
	telemetry->tempsensor_offset_microvolts_at_30_deg_c = microvolt_30_deg_c;
	telemetry->tempsensor_slope_microvolts_per_deg_c = (microvolt_110_deg_c - microvolt_30_deg_c) / 80;

	/* Then calculate the temperature in 1/10th °C */
	/* Sage calculation (see stm32f4-temp-sensor.sage)
	 * temperature_tenths_degc = -800 * adu_30 / (adu_110 - adu_30) + 1201200 * adu_temp / ((adu_110 - adu_30) * adu_vref) + 300
	*/
	const uint16_t adu_refdiff = adu_110_deg_c - adu_30_deg_c;
	telemetry->temperature_10th_deg_c = RDIV(1201200 * adu_tempsensor, adu_refdiff * adu_vrefint) - RDIV(800 * adu_30_deg_c, adu_refdiff) + 300;
}
