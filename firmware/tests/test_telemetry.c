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
#include <stm32f407_tempsensor.h>
#include "testbed.h"

static void test_vref_calculation(void) {
	subtest_start();
	struct adc_telemetry_t telemetry;
	calculate_telemetry(&telemetry, 100, 200, 1501, 0);
	test_assert_int_almost_eq(telemetry.vref_millivolts, 3300, 1);

	calculate_telemetry(&telemetry, 100, 200, 1652, 0);
	test_assert_int_almost_eq(telemetry.vref_millivolts, 3000, 1);

	calculate_telemetry(&telemetry, 100, 200, 1770, 0);
	test_assert_int_almost_eq(telemetry.vref_millivolts, 2800, 1);

	subtest_finished();
}

static void test_temperature_calculation(void) {
	subtest_start();

	struct adc_telemetry_t telemetry;
	calculate_telemetry(&telemetry, 942, 1196, 1662, 1064);
	test_assert_int_almost_eq(telemetry.vref_millivolts, 2981, 1);
	test_assert_int_almost_eq(telemetry.tempsensor_offset_microvolts_at_30_deg_c, 759121, 1);
	test_assert_int_almost_eq(telemetry.tempsensor_slope_microvolts_per_deg_c, 2559, 1);
	test_assert_int_almost_eq(telemetry.temperature_10th_deg_c, 360, 1);
	subtest_finished();
}

/* Make sure there're no wraparounds in a sensible temperature range (-39°C to 160°C) */
static void test_temperature_calculation_range(void) {
	subtest_start();
	struct adc_telemetry_t telemetry;
	int min_temp = -999999999;
	for (int i = 800; i < 1500; i++) {
		calculate_telemetry(&telemetry, 942, 1196, 1662, i);
		test_assert(telemetry.temperature_10th_deg_c > min_temp);
		min_temp = telemetry.temperature_10th_deg_c;
	}
	subtest_finished();
}

int main(int argc, char **argv) {
	test_start(argc, argv);
	test_vref_calculation();
	test_temperature_calculation();
	test_temperature_calculation_range();
	test_finished();
	return 0;
}
